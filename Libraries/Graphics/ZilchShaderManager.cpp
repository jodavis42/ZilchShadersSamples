///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"

#include "ZilchShaderManager.hpp"

#include "Engine/ResourceSystem.hpp"
#include "Texture.hpp"

namespace Graphics
{

using ShaderResourceReflectionData = Zero::ShaderResourceReflectionData;


void ZilchCompilerErrorCallback2(Zilch::ErrorEvent* e)
{
  String msg = e->GetFormattedMessage(Zilch::MessageFormat::MsvcCpp);
  ZPrint("%s", msg.c_str());
  ZeroDebugBreak();
}

void ZilchTranslationErrorCallback2(Zero::TranslationErrorEvent* e)
{
  ZPrint("%s", e->mFullMessage.c_str());
  __debugbreak();
}

ZilchShaderManager::ZilchShaderManager()
{
  mTextureLibrary = nullptr;
  mMaterialLibrary = nullptr;
  mGenerator = nullptr;
}

ZilchShaderManager::~ZilchShaderManager()
{

}

void ZilchShaderManager::Initialize(ResourceSystem* resourceSystem, ZilchShaderIRBackend* backend, const String& shaderDependenciesDir)
{
  mResourceSystem = resourceSystem;
  mTextureLibrary = mResourceSystem->HasResourceLibrary(TextureLibrary);
  mMaterialLibrary = mResourceSystem->HasResourceLibrary(MaterialLibrary);
  mShaderLibrary = mResourceSystem->HasResourceLibrary(ShaderLibrary);

  Zero::SpirVNameSettings nameSettings;
  ZilchShaderIRGenerator::LoadNameSettings(nameSettings);
  mGenerator = new Zero::SimpleZilchShaderIRGenerator(new Zero::ZilchSpirVFrontEnd(), CreateZilchShaderSettings(nameSettings));

  Zilch::EventConnect(mGenerator, Zilch::Events::CompilationError, ZilchCompilerErrorCallback2);
  Zilch::EventConnect(mGenerator, Zero::Events::TranslationError, ZilchTranslationErrorCallback2);

  mPipeline = new Zero::ShaderPipelineDescription();
  mPipeline->mDebugPasses.PushBack(new Zero::ZilchSpirVDisassemblerBackend());
  mPipeline->mBackend = backend;

  mDependencyDir = shaderDependenciesDir;
  mGenerator->SetupDependencies(mDependencyDir);
}

void ZilchShaderManager::AddMaterialDefinition(MaterialCreationData& materialCreationData)
{
  mMaterialDefinitions.PushBack(materialCreationData);
}

void ZilchShaderManager::ClearMaterialDefinitions()
{
  mMaterialDefinitions.Clear();
}

void ZilchShaderManager::AddFragmentFile(const String& shaderProjectDir)
{
  mFragmentFiles.PushBack(shaderProjectDir);
}

void ZilchShaderManager::ClearShaderFragmentFiles()
{
  mFragmentFiles.Clear();
}

void ZilchShaderManager::CreateShadersAndMaterials(Renderer* renderer)
{
  CreateShaders(renderer);
  CreateMaterials();
}

void ZilchShaderManager::ClearShadersAndMaterials(Renderer* renderer)
{
  for(auto range = mShaderLibrary->Values(); !range.Empty(); range.PopFront())
  {
    renderer->DestroyShader(range.Front());
  }
  mShaderLibrary->Destroy();
  mMaterialLibrary->Destroy();
}

void ZilchShaderManager::ClearAll(Renderer* renderer)
{
  ClearShadersAndMaterials(renderer);
  ClearMaterialDefinitions();
  ClearShaderFragmentFiles();
  mGenerator->ClearShadersProjectAndLibrary();
  mGenerator->ClearFragmentsProjectAndLibrary();
}

void ZilchShaderManager::CreateShaders(Renderer* renderer)
{
  LoadAndCompileFragments();
  CreateShaderLibrary();
  ExtractShaders();

  for(auto range = mShaderLibrary->Values(); !range.Empty(); range.PopFront())
  {
    renderer->CreateShader(range.Front());
  }
}

void ZilchShaderManager::LoadAndCompileFragments()
{
  for(size_t i = 0; i < mFragmentFiles.Size(); ++i)
    mGenerator->AddFragmentCode(Zero::ReadFileIntoString(mFragmentFiles[i]), mFragmentFiles[i], nullptr);
  mGenerator->CompileAndTranslateFragments();
}

void ZilchShaderManager::CreateShaderLibrary()
{
  // Create a shader for each material definition
  for(size_t i = 0; i < mMaterialDefinitions.Size(); ++i)
  {
    MaterialCreationData& materialCreationData = mMaterialDefinitions[i];
    CreateMaterialZilchShader(materialCreationData);
  }
  mGenerator->CompileAndTranslateShaders();
  mGenerator->CompilePipeline(*mPipeline);
}

void ZilchShaderManager::CreateMaterialZilchShader(MaterialCreationData& materialCreationData)
{
  // Create the shader definition for this material
  ShaderDefinition shaderDef;
  shaderDef.mShaderName = materialCreationData.mName;
  size_t fragmentTypeCounts[Zero::FragmentType::Size] = {0};
  for(size_t i = 0; i < materialCreationData.mFragmentNames.Size(); ++i)
  {
    ZilchShaderIRType* shaderType = mGenerator->FindFragmentType(materialCreationData.mFragmentNames[i]);
    if(shaderType->mMeta != nullptr)
      ++fragmentTypeCounts[shaderType->mMeta->mFragmentType];
    shaderDef.mFragments.PushBack(shaderType);
  }

  // Create the shader composition for this material
  Zero::ShaderCapabilities capabilities;
  if(fragmentTypeCounts[Zero::FragmentType::Compute] != 0)
    mGenerator->ComposeComputeShader(shaderDef, capabilities);
  else
    mGenerator->ComposeShader(shaderDef, capabilities);

  // Add all of the composited shader stages together into the shader library.
  for(size_t i = 0; i < Zero::FragmentType::Size; ++i)
  {
    Zero::ZilchShaderIRCompositor::ShaderStageDescription& shaderInfo = shaderDef.mResults[i];
    if(shaderInfo.mShaderCode.Empty())
      continue;

    mGenerator->AddShaderCode(shaderInfo.mShaderCode, shaderInfo.mClassName, nullptr);
  }
}

void ZilchShaderManager::ExtractShaders()
{
  typedef Zilch::Ref<Zero::ShaderTranslationPassResult> TranslationPassResultRef;
  for(size_t i = 0; i < mMaterialDefinitions.Size(); ++i)
  {
    MaterialCreationData& materialCreationData = mMaterialDefinitions[i];
    ShaderDefinition* shaderDef = mGenerator->mShaderDefinitionMap.FindPointer(materialCreationData.mName);

    // Create the shader from the shader definition
    Shader* shader = new Shader();
    shader->mName = shaderDef->mShaderName;

    // For each fragment stage, if the shader stage exists then extract it's source into the final shader
    for(size_t i = 0; i < Zero::FragmentType::Size; ++i)
    {
      ShaderStageDescription& shaderInfo = shaderDef->mResults[i];
      if(shaderInfo.mShaderCode.Empty())
        continue;

      ZilchShaderIRType* shaderType = mGenerator->FindShaderType(shaderDef->mResults[i].mClassName);
      Zero::ShaderTranslationPassResult* shaderTranslationResult = mGenerator->FindTranslationResult(shaderType);

      shader->mShaderSource[i] = shaderTranslationResult->ToString();
    }

    mShaderLibrary->Add(shader->mName, shader);
  }
}

void ZilchShaderManager::CreateMaterials()
{
  // Create all block templates.
  CreateMaterialBlockTemplates();
  // Create all materials from the final shaders
  for(auto range = mGenerator->mShaderDefinitionMap.All(); !range.Empty(); range.PopFront())
  {
    ShaderDefinition shaderDef = range.Front().second;
    Material* material = CreateMaterial(shaderDef);
    material->mShader = mShaderLibrary->Find(shaderDef.mShaderName);
  }
}

void ZilchShaderManager::CreateMaterialBlockTemplates()
{
  // Walk all of the types in the fragment library and create material blocks for them if they're actual fragment types.
  ZilchShaderIRLibrary* library = mGenerator->mFragmentLibraryRef;
  for(auto typeRange = library->mTypes.All(); !typeRange.Empty(); typeRange.PopFront())
  {
    ZilchShaderIRType* type = typeRange.Front().second;
    ShaderIRTypeMeta* meta = type->mMeta;
    // Only make a block if this is a valid fragment type (e.g. pixel, vertex, etc...). Don't make one for basic structs.
    if(meta == nullptr || meta->mFragmentType == Zero::FragmentType::None)
      continue;

    MaterialBlock* block = new MaterialBlock();
    block->mBlockName = meta->mZilchName;
    mMaterialLibrary->mFragmentBlockTemplates[block->mBlockName] = block;
    CreateMaterialBlockTemplate(block, type);
  }
}

void ZilchShaderManager::CreateMaterialBlockTemplate(MaterialBlock* block, ZilchShaderIRType* type)
{
  String propertyInputAttribute = mGenerator->mSettings->mNameSettings.mPropertyInputAttribute;
  Zero::ShaderIRTypeMeta* meta = type->mMeta;
  // Iterate all fields, creating a material property for each property on the shader.
  for(size_t i = 0; i < meta->mFields.Size(); ++i)
  {
    ShaderIRFieldMeta* fieldMeta = meta->mFields[i];
    // If this isn't a property then skip it
    Zero::ShaderIRAttribute* propertyAttribute = fieldMeta->mAttributes.FindFirstAttribute(propertyInputAttribute);
    if(propertyAttribute == nullptr)
      continue;

    // Make sure to get the actual property name (can be overridden in the attributes)
    String propertyName = fieldMeta->GetFieldAttributeName(propertyAttribute);
    ZilchShaderIRType* propertyType = mGenerator->mShaderLibraryRef->FindType(fieldMeta->mZilchType->Name);
    
    // Create a material property depending on the type of the property
    MaterialProperty* materialProp = nullptr;
    Zero::ZilchShaderIRRuntimeArrayType runtimeArrayType;
    if(propertyType->mBaseType == Zero::ShaderIRTypeBaseType::SampledImage)
    {
      MaterialTextureProperty* textureProp = new MaterialTextureProperty();
      // @JoshD: No good way to set texture defaults now (can't do it by string, have to inject types into zilch).
      textureProp->mResourceName = mTextureLibrary->GetDefault()->mName;
      textureProp->mLibrary = mTextureLibrary;
      materialProp = textureProp;
    }
    else if(runtimeArrayType.Load(propertyType))
    {
      MaterialSsboProperty* ssboProp = new MaterialSsboProperty();
      // @JoshD: No good way to set texture defaults now (can't do it by string, have to inject types into zilch).
      //ssboProp->m = mTextureLibrary->GetDefault()->mName;
      //ssboProp->mLibrary = mTextureLibrary;
      materialProp = ssboProp;
    }
    else
    {
      MaterialDataProperty* dataProp = new MaterialDataProperty();
      dataProp->mPropertyData.Set(fieldMeta->mDefaultValueVariant.GetData(), fieldMeta->mZilchType->Size);
      materialProp = dataProp;
    }
    materialProp->mPropertyName = propertyName;
    materialProp->mShaderType = propertyType;
    block->AddProperty(materialProp);
  }
}

Material* ZilchShaderManager::CreateMaterial(ShaderDefinition& shaderDef)
{
  Material* material = new Material();
  material->mName = shaderDef.mShaderName;
  
  HashMap<String, MaterialBlock*> blockNameMap;
  CreateMaterialFromBlockTemplates(shaderDef, blockNameMap, material);
  ExtractMaterialReflection(shaderDef, blockNameMap, material);

  mMaterialLibrary->Add(material->mName, material);
  return material;
}

void ZilchShaderManager::CreateMaterialFromBlockTemplates(ShaderDefinition& shaderDef, BlockNameMap& blockNameMap, Material* material)
{
  // Clone material blocks from the template blocks
  for(size_t i = 0; i < shaderDef.mFragments.Size(); ++i)
  {
    ZilchShaderIRType* shaderType = shaderDef.mFragments[i];
    ShaderIRTypeMeta* meta = shaderType->mMeta;

    MaterialBlock* templateBlock = mMaterialLibrary->mFragmentBlockTemplates.FindValue(meta->mZilchName, nullptr);
    ErrorIf(templateBlock == nullptr, "This should never happen");

    MaterialBlock* block = new MaterialBlock();
    block->mBlockName = meta->mZilchName;
    block->CopyFrom(*templateBlock);
    material->mMaterialBlocks.PushBack(block);
    blockNameMap[block->mBlockName] = block;
  }
}

void ZilchShaderManager::ExtractMaterialReflection(ShaderDefinition& shaderDef, BlockNameMap& blockNameMap, Material* material)
{
  String materialBufferName = mGenerator->mSettings->mDefaultUniformBufferDescription.mDebugName;

  // Fill out reflection data for this unique block (the actual binding is unique per material, not per block)
  for(size_t fragType = 0; fragType < Zero::FragmentType::Size; ++fragType)
  {
    ShaderStageDescription& stageDesc = shaderDef.mResults[fragType];
    ZilchShaderIRType* shaderType = mGenerator->FindShaderType(stageDesc.mClassName);
    // This stage didn't have a shader type (e.g. no geometry shader)
    if(shaderType == nullptr)
      continue;

    // Extract the material block's overall reflection data.
    // Note: This isn't exactly the safest since this is only a debug name. But in 99% of cases this should work as expected.
    ShaderReflectionData* reflectionData = mGenerator->FindSimplifiedReflectionResult(shaderType);
    for(size_t i = 0; i < reflectionData->mReflection.mUniforms.Size(); ++i)
    {
      Zero::ShaderStageResource& stageResource = reflectionData->mReflection.mUniforms[i];
      if(stageResource.mReflectionData.mInstanceName == materialBufferName)
        material->mMaterialStageBindingData[fragType].mReflectionData = stageResource.mReflectionData;
    }

    /// Extract the reflection data for each material block
    for(auto stageFragDescRange = stageDesc.mFragmentDescriptions->All(); !stageFragDescRange.Empty(); stageFragDescRange.PopFront())
    {
      ZilchShaderIRCompositor::ShaderFragmentDescription* fragmentDesc = stageFragDescRange.Front().second;
      ZilchShaderIRType* fragmentType = mGenerator->FindFragmentType(fragmentDesc->mMeta->mZilchName);

      MaterialBlock* block = blockNameMap.FindValue(fragmentDesc->mMeta->mZilchName, nullptr);
      ExtractMaterialBlockReflection(reflectionData, fragmentType, block);
    }
  }
}

void ZilchShaderManager::ExtractMaterialBlockReflection(ShaderReflectionData* reflectionData, ZilchShaderIRType* fragmentType, MaterialBlock* block)
{
  ShaderReflectionData::FragmentLookup* fragmentLookup = reflectionData->mFragmentLookup.FindPointer(fragmentType->mMeta->mZilchName);
  if(fragmentLookup == nullptr)
    return;

  // Copy out all uniforms for this fragment
  for(auto uniformRange = fragmentLookup->mMaterialUniforms.All(); !uniformRange.Empty(); uniformRange.PopFront())
  {
    String propName = uniformRange.Front().first;
    Zero::ShaderIRFieldMeta* fieldMeta = fragmentType->mMeta->FindField(propName);

    // Copy out the field's reflection data
    ShaderResourceReflectionData* fieldReflectionData = reflectionData->FindUniformReflectionData(fragmentType, propName);
    MaterialDataProperty* materialProp = (MaterialDataProperty*)block->FindProperty(propName);
    materialProp->mValidReflectionObject = true;
    materialProp->mFragmentType = fragmentType->mMeta->mFragmentType;
    materialProp->mBindingData.mReflectionData = *fieldReflectionData;
  }
  // Copy out all sampled image reflection data. This is more complicated as sampled images can be a one to many mapping.
  for(auto sampledImageRange = fragmentLookup->mSampledImages.All(); !sampledImageRange.Empty(); sampledImageRange.PopFront())
  {
    String propName = sampledImageRange.Front().first;
    Zero::ShaderIRFieldMeta* fieldMeta = fragmentType->mMeta->FindField(propName);

    Array<ShaderResourceReflectionData*> reflectionResults;
    reflectionData->FindSampledImageReflectionData(fragmentType, propName, reflectionResults);

    MaterialTextureProperty* materialProp = (MaterialTextureProperty*)block->FindProperty(propName);
    materialProp->mValidReflectionObject = true;
    materialProp->mFragmentType = fragmentType->mMeta->mFragmentType;
    materialProp->mBindingData.Resize(reflectionResults.Size());
    for(size_t i = 0; i < reflectionResults.Size(); ++i)
    {
      materialProp->mBindingData[i].mReflectionData = *reflectionResults[i];
    }
  }
  for(auto ssboRange = fragmentLookup->mStructedStorageBuffers.All(); !ssboRange.Empty(); ssboRange.PopFront())
  {
    String propName = ssboRange.Front().first;
    Zero::ShaderIRFieldMeta* fieldMeta = fragmentType->mMeta->FindField(propName);

    // Copy out the field's reflection data
    ShaderResourceReflectionData* fieldReflectionData = reflectionData->FindStructedStorageBuffer(fragmentType, propName);
    MaterialSsboProperty* ssboProp = (MaterialSsboProperty*)block->FindProperty(propName);
    ssboProp->mValidReflectionObject = true;
    ssboProp->mFragmentType = fragmentType->mMeta->mFragmentType;
    ssboProp->mBindingData.mReflectionData = *fieldReflectionData;
  }
}

ZilchShaderSpirVSettings* ZilchShaderManager::CreateZilchShaderSettings(SpirVNameSettings& nameSettings)
{
  using namespace Zero;
  ZilchShaderSpirVSettings* settings = new ZilchShaderSpirVSettings(nameSettings);

  Zilch::BoundType* realType = ZilchTypeId(Zilch::Real);
  Zilch::BoundType* real2Type = ZilchTypeId(Zilch::Real2);
  Zilch::BoundType* real3Type = ZilchTypeId(Zilch::Real3);
  Zilch::BoundType* real4Type = ZilchTypeId(Zilch::Real4);
  Zilch::BoundType* intType = ZilchTypeId(Zilch::Integer);
  Zilch::BoundType* int4Type = ZilchTypeId(Zilch::Integer4);
  Zilch::BoundType* real4x4Type = ZilchTypeId(Zilch::Real4x4);

  // Setup uniform buffers
  UniformBufferDescription frameData(0);
  frameData.mDebugName = "Frame Data";
  frameData.AddField(realType, "LogicTime");
  frameData.AddField(realType, "FrameTime");
  settings->AddUniformBufferDescription(frameData);

  UniformBufferDescription cameraData(1);
  cameraData.mDebugName = "CameraData";
  cameraData.AddField(realType, "NearPlane");
  cameraData.AddField(realType, "FarPlane");
  cameraData.AddField(real2Type, "ViewportSize");
  settings->AddUniformBufferDescription(cameraData);

  UniformBufferDescription transformData(2);
  transformData.mDebugName = "TransformData";
  transformData.AddField(real4x4Type, "LocalToWorld");
  transformData.AddField(real4x4Type, "WorldToView");
  transformData.AddField(real4x4Type, "ViewToPerspective");
  transformData.AddField(real4x4Type, nameSettings.mPerspectiveToApiPerspectiveName);
  settings->AddUniformBufferDescription(transformData);

  settings->AutoSetDefaultUniformBufferDescription();

  // Add some default vertex definitions (glsl attributes)
  settings->mVertexDefinitions.AddField(realType, "Scalar");
  settings->mVertexDefinitions.AddField(real2Type, "Uv");
  settings->mVertexDefinitions.AddField(real3Type, "LocalNormal");
  settings->mVertexDefinitions.AddField(real3Type, "LocalPosition");
  settings->mVertexDefinitions.AddField(real4Type, "Color");
  settings->mVertexDefinitions.AddField(real4Type, "Aux0");
  settings->mVertexDefinitions.AddField(int4Type, "BoneIndices");

  // Set zilch fragment names for spirv built-ins
  settings->SetHardwareBuiltInName(spv::BuiltInPosition, nameSettings.mApiPerspectivePositionName);

  settings->SetMaxSimultaneousRenderTargets(4);
  settings->SetRenderTargetName("Target0", 0);
  settings->SetRenderTargetName("Target1", 1);
  settings->SetRenderTargetName("Target2", 2);
  settings->SetRenderTargetName("Target3", 3);

  // Set custom callbacks in both the compositor and entry point code generation
  // for dealing with perspective position vs. api perspective position.
  settings->mCallbackSettings.SetCompositeCallback(&ZilchShaderIRCompositor::ApiPerspectivePositionCallback, nullptr);
  settings->mCallbackSettings.SetAppendCallback(&Zero::EntryPointGeneration::PerspectiveTransformAppendVertexCallback, nullptr);

  settings->Finalize();

  return settings;
}


}//namespace Graphics
