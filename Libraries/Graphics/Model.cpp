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

//-------------------------------------------------------------------ComputePass
ComputePass::ComputePass()
{
  mMaterial = nullptr;
  mDispatchSize = Integer3(1, 1, 1);
}

void ComputePass::Serialize(Serializer& serializer)
{
  __super::Serialize(serializer);
  serializer.SerializeResourceField<MaterialLibrary>("Material", mMaterial);
  serializer.SerializeField(mDispatchSize);
}

}//namespace Graphics
