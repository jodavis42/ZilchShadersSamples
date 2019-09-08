///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GraphicsStandard.hpp"

#include "ByteBuffer.hpp"

namespace Graphics
{

//-------------------------------------------------------------------BufferType
struct BufferType
{
  enum Enum
  {
    None,
    Uniform,
    StructuredStorage,
  };
};

//-------------------------------------------------------------------BufferMappingType
struct BufferMappingType
{
  enum Enum
  {
    Read = 1 << 0,
    Write = 1 << 1,
    InvalidateRange = 1 << 2,
    InvalidateBuffer = 1 << 3
  };
};

//-------------------------------------------------------------------BufferCreationData
/// Data used to create a buffer.
class BufferCreationData
{
public:
  BufferCreationData();
  ByteBuffer* mBufferData;
  size_t mSizeInBytes;
  String mName;
};

//-------------------------------------------------------------------BufferData
/// An opaque type that represents a buffer. The renderer fills this out with
/// whatever data it needs and the user stores and returns it.
class BufferKey
{
public:
  BufferKey();

  union
  {
    void* mKey;
    int mId;
  };
};

//-------------------------------------------------------------------BufferRenderData
/// Runtime data for rendering a buffer. This is returned by the renderer as an
/// opaque key plus the binding index the user wants to bind this buffer to.
class BufferRenderData
{
public:
  BufferRenderData();
  BufferKey mKey;
  int mBindingIndex;
};

//-------------------------------------------------------------------EphemeralBuffer
/// A temporary buffer that is uploaded, used, then discarded. It's lifetime is no
/// longer than it's draw. Used for simplicity to upload data that is per-object.
class EphemeralBuffer
{
public:
  EphemeralBuffer();
  BufferType::Enum mBufferType;
  BufferCreationData mCreationData;
  BufferRenderData mRenderData;
};

}//namespace Graphics
