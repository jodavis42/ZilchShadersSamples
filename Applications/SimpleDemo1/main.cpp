#include "ZilchShadersStandard.hpp"
#include "JsonSerializer.hpp"
#include "ShaderSerialization.hpp"
#include "Callbacks.hpp"
#include "Helpers.hpp"

using namespace Engine;
using namespace Zero;


void AddFragments(const String& path, Zero::SimpleZilchShaderIRGenerator* generator)
{
  // Load all fragment files into the fragment project
  generator->RecursivelyLoadDirectory(path, generator->mFragmentProject);
}

void AddShader(const String& filePath, Zero::SimpleZilchShaderIRGenerator* generator)
{
  ZilchShaderIRCompositor::ShaderDefinition shaderDef;
  Array<String> fragmentNames;
  Demo::LoadShaderDef(filePath, shaderDef.mShaderName, fragmentNames);

  // Add all of the required fragments for this shader into the shader def
  size_t fragmentTypeCounts[Zero::FragmentType::Size] = {0};
  for(size_t i = 0; i < fragmentNames.Size(); ++i)
  {
    ZilchShaderIRType* fragmentType = generator->FindFragmentType(fragmentNames[i]);
    if(fragmentType != nullptr)
    {
      shaderDef.mFragments.PushBack(fragmentType);
      ++fragmentTypeCounts[fragmentType->mMeta->mFragmentType];
    }
  }

  // Unfortunately, the current pipeline requires different compositing logic for compute or render passes.
  // Determine which one to use now via the presence of a compute fragment (don't mix them)
  Zero::ShaderCapabilities capabilities;
  if(fragmentTypeCounts[Zero::FragmentType::Compute] != 0)
    generator->ComposeComputeShader(shaderDef, capabilities);
  else
    generator->ComposeShader(shaderDef, capabilities);

  // After compositing, add the generated zilch shader code for all of the relevant shader stages to the shader project
  for(size_t i = 0; i < FragmentType::Size; ++i)
  {
    ZilchShaderIRCompositor::ShaderStageDescription& stageDesc = shaderDef.mResults[i];
    if(!stageDesc.mClassName.Empty())
      generator->AddShaderCode(stageDesc.mShaderCode, stageDesc.mClassName, nullptr);
  }
}

void AddShaders(const String& path, Zero::SimpleZilchShaderIRGenerator* generator)
{
  // Load all material files
  FileRange fileRange(path);
  for(; !fileRange.Empty(); fileRange.PopFront())
  {
    auto entry = fileRange.FrontEntry();
    String fullPath = entry.GetFullPath();
    if(FilePath::GetExtension(fullPath) != "material")
      continue;

    AddShader(fullPath, generator);
  }
}

void DumpShader(const String& path, Zero::SimpleZilchShaderIRGenerator* generator, Zero::ZilchShaderIRType* shaderType, const String& outPath)
{
  ShaderIRTypeMeta* shaderMeta = shaderType->mMeta;
  // Some types aren't fragments (internal pointer types, etc...)
  if(shaderMeta == nullptr || shaderMeta->mFragmentType == FragmentType::None)
    return;
  
  SimpleZilchShaderIRGenerator::ShaderTranslationResult* translationResult = generator->mShaderResults.FindPointer(shaderType->mMeta->mZilchName);
  if(translationResult == nullptr)
    return;

  String shaderOutPath = FilePath::Combine(outPath, shaderMeta->mZilchName);
  CreateDirectory(shaderOutPath);

  // Output all debug pass results
  for(size_t i = 0; i < translationResult->mDebugResults.Size(); ++i)
  {
    ShaderTranslationPassResult* passResult = translationResult->mDebugResults[i];
    String fileName = BuildString("DebugPass", ToString(i));
    String filePath = Zero::FilePath::CombineWithExtension(shaderOutPath, fileName, ".txt");
    Zero::WriteToFile(filePath, passResult->ToString());
  }

  // Write out the backend final results
  String extension = BuildString(".", generator->mPipeline->mBackend->GetExtension());
  String filePath = Zero::FilePath::CombineWithExtension(shaderOutPath, shaderType->mName, extension);
  ShaderTranslationPassResult* finalPassResult = translationResult->mFinalPassData;
  Zero::WriteToFile(filePath, finalPassResult->ToString());
  
  // Also output the final reflection data
  String reflectionFilePath = Zero::FilePath::CombineWithExtension(shaderOutPath, shaderType->mName, ".json");
  Zilch::JsonBuilder serializer;
  Demo::ShaderSerialization saver(&serializer, generator);
  saver.SaveShaderReflection(shaderType);
  Zero::WriteToFile(reflectionFilePath, serializer.ToString());
}

void DumpProjectShaders(const String& path, Zero::SimpleZilchShaderIRGenerator* generator, const String& outPath)
{
  CreateDirectory(outPath);

  ZilchSpirVDisassemblerBackend backend;
  // Dump all types in the shader library that are fragments
  for(auto shaderRange = generator->mShaderLibraryRef->mTypes.Values(); !shaderRange.Empty(); shaderRange.PopFront())
  {
    Zero::ZilchShaderIRType* shaderType = shaderRange.Front();
    DumpShader(path, generator, shaderType, outPath);
  }
}

void BuildProject(const String& projectPath, Zero::SimpleZilchShaderIRGenerator* generator, const String& outPath)
{
  CreateDirectory(outPath);

  generator->ClearAll();
  generator->ClearShadersProjectAndLibrary();

  // Find all fragments and add them to the project
  AddFragments(projectPath, generator);
  // Compile all fragments into native spirv (only needs to be done once for all shaders)
  generator->CompileAndTranslateFragments();

  // Now find all shader definitions and composite the final shaders
  AddShaders(projectPath, generator);
  // Translate each shader. This effectively links all the fragments together
  generator->CompileAndTranslateShaders();

  // Compile the full pipeline (caches debug info, backend results, reflection, etc...)
  generator->CompilePipeline();

  // Save the results out
  String shaderOutPath = Zero::FilePath::Combine(outPath, "Shaders");
  DumpProjectShaders(projectPath, generator, shaderOutPath);
}

void SpecializationConstantsCallback(SpecializationConstantEvent* e)
{
  SpirVSpecializationConstantPass* specConstantPass = new SpirVSpecializationConstantPass();

  // Write out the spec constant for the language name (could be controlled via an enum, arbitrarily chosen here)
  String languageSpecializationName = ZilchShaderSpirVSettings::GetLanguageSpecializationName();
  int languageSpecializationId = e->GetFirstId(languageSpecializationName);
  if(languageSpecializationId != -1)
    e->mSpecializationOverridesById[languageSpecializationId] = "2";

  // Also write out the language's version id
  String languageVersionSpecializationName = ZilchShaderSpirVSettings::GetLanguageVersionSpecializationName();
  int languageVersionSpecializationId = e->GetFirstId(languageVersionSpecializationName);
  if(languageVersionSpecializationId != -1)
    e->mSpecializationOverridesById[languageVersionSpecializationId] = ToString(440);
}

ShaderPipelineDescription* CreatePipeline()
{
  // Build the pipeline used for all translations
  ShaderPipelineDescription* pipeline = new ShaderPipelineDescription();

  // Create the backend to translate the final results to glsl
  ZilchShaderGlslBackend* backend = new ZilchShaderGlslBackend();
  backend->mTargetVersion = 440;
  pipeline->mBackend = backend;

  // Create various debug passes (these run but aren't part of the
  // final pipeline so they're only used to output extra data)
  pipeline->mDebugPasses.PushBack(new SpirVValidatorPass());
  pipeline->mDebugPasses.PushBack(new ZilchSpirVDisassemblerBackend());
  //pipeline->mDebugPasses.PushBack(new SpirVFileWriterPass());
  
  // Tool passes are chained together to create the input for the final pass

  // Create a pass to freeze specialization constants. This has to be a callback as a shader can define it's own constants
  SpirVSpecializationConstantPass* specConstantPass = new SpirVSpecializationConstantPass();
  Zilch::EventConnect(specConstantPass, Zero::Events::CollectSpecializationConstants, &SpecializationConstantsCallback);
  pipeline->mToolPasses.PushBack(specConstantPass);

  // Optimize the results (disabled just because)
  //pipeline->mToolPasses.PushBack(new SpirVOptimizerPass());

  return pipeline;
}

SimpleZilchShaderIRGenerator* CreateShaderGenerator(const String& shaderCoreDir)
{
  // Create the settings used for translation (use zero's settings for simplicity)
  SpirVNameSettings nameSettings;
  SimpleZilchShaderIRGenerator::LoadNameSettings(nameSettings);
  ZilchShaderSpirVSettings* settings = Zero::SimpleZilchShaderIRGenerator::CreateZeroSettings(nameSettings);
  // Construct the generator using the spriv front-end
  SimpleZilchShaderIRGenerator* generator = new SimpleZilchShaderIRGenerator(new Zero::ZilchSpirVFrontEnd(), settings);

  // Hook up error callback handler
  Zilch::EventConnect(generator, Zilch::Events::CompilationError, Demo::ZilchCompilerErrorCallback);
  Zilch::EventConnect(generator, Zero::Events::TranslationError, Demo::ZilchTranslationErrorCallback);

  // Setup the dependencies library (contains a bunch of helper functions defined in zilch)
  generator->SetupDependencies(shaderCoreDir);
  
  // Build the pipeline used for all translations
  generator->SetPipeline(CreatePipeline());
  return generator;
}

void BuildProjects(const String& outPath)
{
  CreateDirectory(outPath);

  // Load some folder locations
  JsonSerializer serializer;
  serializer.Load("BuildConfig.data");
  String ProjectsDir;
  serializer.SerializeField(ProjectsDir);
  String ShaderCoreDir;
  serializer.SerializeField(ShaderCoreDir);

  Zero::SimpleZilchShaderIRGenerator* generator = CreateShaderGenerator(ShaderCoreDir);

  // Recursively build and translate each project
  Zero::FileRange range(ProjectsDir);
  for(; !range.Empty(); range.PopFront())
  {
    auto entry = range.FrontEntry();
    String dirPath = entry.GetFullPath();
    if(!Zero::DirectoryExists(dirPath))
      continue;

    String projectDir = FilePath::Combine(outPath, entry.mFileName);
    BuildProject(dirPath, generator, projectDir);
  }

  delete generator;
}

int main()
{
  Zilch::ZilchSetup zilchSetup;

  Zilch::Module module;
  Zilch::ExecutableState::CallingState = module.Link();

  ShaderSettingsLibrary::InitializeInstance();
  ShaderSettingsLibrary::GetInstance().GetLibrary();
  
  BuildProjects("Projects");

  ShaderSettingsLibrary::GetInstance().Destroy();
  delete Zilch::ExecutableState::CallingState;

  return 0;
}
