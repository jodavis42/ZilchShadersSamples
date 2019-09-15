///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"

#include "ComputePass.hpp"
#include "Serializer.hpp"
#include "Material.hpp"

namespace Graphics
{

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
