///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"

#include "ByteBuffer.hpp"

namespace Graphics
{

//-------------------------------------------------------------------ByteBuffer
ByteBuffer::ByteBuffer()
{
}

ByteBuffer::ByteBuffer(const ByteBuffer& rhs)
{
  CopyFrom(rhs);
}

ByteBuffer::~ByteBuffer()
{
  Clear();
}

ByteBuffer& ByteBuffer::operator=(const ByteBuffer& rhs)
{
  if(&rhs == this)
    return *this;

  CopyFrom(rhs);
  return *this;
}

void ByteBuffer::Set(const byte* data, size_t size)
{
  Clear();
  mData.Resize(size);
  memcpy(mData.Data(), data, size);
}

void ByteBuffer::CopyFrom(const ByteBuffer& rhs)
{
  Set(rhs.Data(), rhs.Size());
}

void ByteBuffer::Clear()
{
  mData.Clear();
}

void ByteBuffer::Resize(size_t size)
{
  mData.Resize(size);
}

size_t ByteBuffer::Size() const
{
  return mData.Size();
}

byte* ByteBuffer::Data()
{
  return mData.Data();
}

const byte* ByteBuffer::Data() const
{
  return mData.Data();
}

}//namespace Graphics
