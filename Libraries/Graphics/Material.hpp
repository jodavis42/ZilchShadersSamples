///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GraphicsStandard.hpp"

#include "ShaderProperties.hpp"
#include "Resource.hpp"
#include "MaterialProperties.hpp"

namespace Graphics
{

class Shader;

//-------------------------------------------------------------------MaterialCreationData
class MaterialCreationData
{
public:
  void Serialize(Serializer& serializer);

  Array<String> mFragmentNames;
  String mName;
};

//-------------------------------------------------------------------MaterialBlock
class MaterialBlock
{
public:
  ~MaterialBlock();

  void AddProperty(MaterialProperty* prop);
  MaterialProperty* FindProperty(const String& propertyName);

  void CopyFrom(const MaterialBlock& rhs);

  void Destroy();

  HashMap<String, MaterialProperty*> mPropertyMap;
  Array<MaterialProperty*> mPropertyList;
  String mBlockName;
};

//-------------------------------------------------------------------Material
class Material : public Engine::Resource
{
public:
  Material();
  ~Material();

  void Destroy();

  Array<MaterialBlock*> mMaterialBlocks;
  ShaderPropertyBindingData mMaterialStageBindingData[Zero::FragmentType::Size];

  Shader* mShader;
};

//-------------------------------------------------------------------MaterialLibrary
class MaterialLibrary : public Engine::ResourceLibrary<Material>
{
public:
  DeclareResourceLibraryType(MaterialLibrary);

  void Destroy() override;

  HashMap<String, MaterialBlock*> mFragmentBlockTemplates;
};

}//namespace Graphics
