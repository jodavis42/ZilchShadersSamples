//#include "Precompiled.hpp"
//
#include "ShaderProgram.hpp"

void ZilchCompilerErrorCallback(Zilch::ErrorEvent* e)
{
  ZPrint("%s", e->GetFormattedMessage(Zilch::MessageFormat::MsvcCpp).c_str());
  ZeroDebugBreak();
}

void ZilchTranslationErrorCallback(Zero::TranslationErrorEvent* e)
{
  ZPrint("%s", e->mFullMessage.c_str());
  __debugbreak();
}

SampleZilchShaderIRGenerator::SampleZilchShaderIRGenerator(Zero::ZilchShaderSpirVSettings* settings)
  : Zero::SimpleZilchShaderIRGenerator(new Zero::ZilchSpirVFrontEnd(), settings)
{
  Zilch::EventConnect(this, Zilch::Events::CompilationError, ZilchCompilerErrorCallback);
  Zilch::EventConnect(this, Zero::Events::TranslationError, ZilchTranslationErrorCallback);
}

void SampleZilchShaderIRGenerator::CreateShader(Material& material, Shader& shader)
{
  // Zero::ZilchShaderIRCompositor::ShaderDefinition shaderDef;
  //shaderDef.mShaderName = material.mMaterialName;
  //for(size_t i = 0; i < material.mFragmentNames.Size(); ++i)
  //{
  //  Zero::ZilchShaderIRType* shaderType = generator.FindFragmentType(material.mFragmentNames[i]);
  //  shaderDef.mFragments.PushBack(shaderType);
  //}
  //
  //Zero::ShaderCapabilities capabilities;
  //generator.ComposeShader(shaderDef, capabilities);
  //
  //// Add all of the composited shader stages together into the shader library.
  //for(size_t i = 0; i < Zero::FragmentType::Size; ++i)
  //{
  //  Zero::ZilchShaderIRCompositor::ShaderStageDescription& shaderInfo = shaderDef.mResults[i];
  //  if(shaderInfo.mShaderCode.Empty())
  //    continue;
  //
  //  generator.AddShaderCode(shaderInfo.mShaderCode, shaderInfo.mClassName, nullptr);
  //}
  //generator.CompileAndTranslateShaders();
  //
  //typedef Zilch::Ref<Zero::ShaderTranslationPassResult> TranslationPassResultRef;
  //for(size_t i = 0; i < Zero::FragmentType::Size; ++i)
  //{
  //  Zero::ZilchShaderIRCompositor::ShaderStageDescription& shaderInfo = shaderDef.mResults[i];
  //  if(shaderInfo.mShaderCode.Empty())
  //    continue;
  //
  //  Zero::ZilchShaderIRType* shaderType = generator.FindShaderType(shaderDef.mResults[i].mClassName);
  //  Array<TranslationPassResultRef> pipelineResults, debugResults;
  //  generator.CompilePipeline(shaderType, pipeline, pipelineResults, debugResults);
  //
  //  TranslationPassResultRef passResult = pipelineResults.Back();
  //
  //  shader.mShaderSource[i] = passResult->ToString();
  //}
}

Zero::ZilchShaderSpirVSettings* SampleZilchShaderIRGenerator::CreateZilchShaderSettings(Zero::SpirVNameSettings& nameSettings)
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

  //// Add some default vertex definitions (glsl attributes)
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
  //settings->mCallbackSettings.SetAppendCallback(&EntryPointGeneration::PerspectiveTransformAppendVertexCallback, nullptr);

  settings->Finalize();

  return settings;
}
