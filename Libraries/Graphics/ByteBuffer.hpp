///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GraphicsStandard.hpp"

namespace Graphics
{

//-------------------------------------------------------------------ByteBuffer
class ByteBuffer
{
public:
  ByteBuffer();
  ByteBuffer(const ByteBuffer& rhs);
  ~ByteBuffer();

  ByteBuffer& operator=(const ByteBuffer& rhs);

  void Set(const byte* data, size_t size);
  void CopyFrom(const ByteBuffer& rhs);
  void Clear();

  size_t Size() const;
  void Resize(size_t size);

  byte* Data();
  const byte* Data() const;

  template <typename T>
  void Set(T& data)
  {
    Set((byte*)&data, sizeof(T));
  }

  Array<byte> mData;
};

}//namespace Graphics
