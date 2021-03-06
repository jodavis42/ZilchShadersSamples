///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"

#include "Model.hpp"
#include "Serializer.hpp"
#include "Mesh.hpp"
#include "Material.hpp"

namespace Graphics
{

//-------------------------------------------------------------------Model
Model::Model()
{
  mMaterial = nullptr;
  mMesh = nullptr;
}

void Model::Serialize(Serializer& serializer)
{
  __super::Serialize(serializer);
  serializer.SerializeResourceField<MeshLibrary>("Mesh", mMesh);
  serializer.SerializeResourceField<MaterialLibrary>("Material", mMaterial);
}
}//namespace Graphics
