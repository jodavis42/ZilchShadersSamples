///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"

#include "Buffers.hpp"

namespace Graphics
{

//-------------------------------------------------------------------BufferCreationData
BufferCreationData::BufferCreationData()
{
  mBufferData = nullptr;
  mSizeInBytes = 0;
}

//-------------------------------------------------------------------BufferData
BufferKey::BufferKey()
{
  mId = -1;
}

//-------------------------------------------------------------------BufferRenderData
BufferRenderData::BufferRenderData()
{
  mBindingIndex = 0;
}

//-------------------------------------------------------------------EphemeralBuffer
EphemeralBuffer::EphemeralBuffer()
{
  mBufferType = BufferType::Uniform;
}

}//namespace Graphics
