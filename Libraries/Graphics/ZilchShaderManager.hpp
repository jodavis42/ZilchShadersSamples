///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GraphicsStandard.hpp"

#include "Renderer.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "ResourceSystem.hpp"

namespace Graphics
{

using ZilchShaderIRCompositor = Zero::ZilchShaderIRCompositor;
using ZilchShaderIRGenerator = Zero::SimpleZilchShaderIRGenerator;
using ShaderReflectionData = Zero::SimplifiedShaderReflectionData;
using ZilchShaderIRType = Zero::ZilchShaderIRType;
using ShaderDefinition = Zero::ZilchShaderIRCompositor::ShaderDefinition;
using ShaderPipelineDescription = Zero::ShaderPipelineDescription;
using ShaderIRTypeMeta = Zero::ShaderIRTypeMeta;
using ZilchShaderIRLibrary = Zero::ZilchShaderIRLibrary;
using ShaderIRFieldMeta = Zero::ShaderIRFieldMeta;
using SpirVNameSettings = Zero::SpirVNameSettings;
using ZilchShaderSpirVSettings = Zero::ZilchShaderSpirVSettings;
using ShaderStageDescription = Zero::ZilchShaderIRCompositor::ShaderStageDescription;
class TextureLibrary;

//-------------------------------------------------------------------ZilchShaderGlslBackend
/// Manages the creation of shaders and materials from zilch. Fragments are compiled to
/// create the available material blocks. These material blocks are used to create shaders.
/// The reflection data of the shaders are used to actually create materials
/// (knowing what properties turn into what uniform buffers, etc...)
class ZilchShaderManager
{
public:
  typedef HashMap<String, MaterialBlock*> BlockNameMap;

  ZilchShaderManager();
  ~ZilchShaderManager();

  void Initialize(ResourceSystem* resourceSystem, ZilchShaderIRBackend* backend, const String& shaderDependenciesDir);

  /// Adds a material definition that will be used when creating shaders.
  void AddMaterialDefinition(MaterialCreationData& materialCreationData);
  void ClearMaterialDefinitions();

  /// Adds a file to load fragments from.
  void AddFragmentFile(const String& shaderProjectDir);
  void ClearShaderFragmentFiles();
  
  /// Create all shaders and materials from the given fragment libraries and material definitions.
  void CreateShadersAndMaterials(Renderer* renderer);
  void ClearShadersAndMaterials(Renderer* renderer);

  void ClearAll(Renderer* renderer);

  TextureLibrary* mTextureLibrary;
  MaterialLibrary* mMaterialLibrary;
  ShaderLibrary* mShaderLibrary;
  ResourceSystem* mResourceSystem;
  ZilchShaderIRGenerator* mGenerator;
  Zilch::Ref<ShaderPipelineDescription> mPipeline;

private:
  /// Helper to create all available shaders from the material definitions.
  void CreateShaders(Renderer* renderer);
  /// Creates all the fragment projects and compiles them.
  void LoadAndCompileFragments();
  /// Creates the shader library from all of the material definitions.
  void CreateShaderLibrary();
  void CreateMaterialZilchShader(MaterialCreationData& materialCreationData);
  /// Extracts the final shader results from the zilch shader library.
  void ExtractShaders();

  /// Creates all materials from the compiled shaders.
  void CreateMaterials();
  /// Creates a material block template for each shader fragment and fills it out with the basic
  /// reflection data. This makes it easy to clone a material block for any material it's used in.
  void CreateMaterialBlockTemplates();
  /// Creates a material block template from the given shader type.
  void CreateMaterialBlockTemplate(MaterialBlock* block, ZilchShaderIRType* type);

  /// Creates a material from the given shader definition.
  Material* CreateMaterial(ShaderDefinition& shaderDef);
  /// Popuplates a material with material blocks from the previously built template blocks.
  void CreateMaterialFromBlockTemplates(ShaderDefinition& shaderDef, BlockNameMap& blockNameMap, Material* material);
  /// Extract reflection data for the entire material for this shader.
  void ExtractMaterialReflection(ShaderDefinition& shaderDef, BlockNameMap& blockNameMap, Material* material);
  /// Extract reflection data for the material block for a specified shader.
  void ExtractMaterialBlockReflection(ShaderReflectionData* reflectionData, ZilchShaderIRType* fragmentType, MaterialBlock* block);

  ZilchShaderSpirVSettings* CreateZilchShaderSettings(Zero::SpirVNameSettings& nameSettings);

  String mDependencyDir;
  Array<String> mFragmentFiles;

  Array<MaterialCreationData> mMaterialDefinitions;
};


}//namespace Graphics
