///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GraphicsStandard.hpp"

#include "ShaderProperties.hpp"
#include "ResourceLibrary.hpp"
#include "MaterialProperties.hpp"

namespace Graphics
{

class Shader;

//-------------------------------------------------------------------MaterialCreationData
class MaterialCreationData
{
public:
  Array<String> mFragmentNames;
  String mMaterialName;
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
class Material : public Resource
{
public:
  Material();
  ~Material();

  void Destroy();

  Array<MaterialBlock*> mMaterialBlocks;
  ShaderPropertyBindingData mMaterialStageBindingData[Zero::FragmentType::Size];

  String mMaterialName;
  Shader* mShader;
};

//-------------------------------------------------------------------MaterialLibrary
class MaterialLibrary : public ResourceLibrary<Material>
{
public:
  void Destroy() override;

  HashMap<String, MaterialBlock*> mFragmentBlockTemplates;
};

}//namespace Graphics
