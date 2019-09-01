//#include "Precompiled.hpp"
//
//UniformByteBuffer::UniformByteBuffer()
//{
//  mData = nullptr;
//  mSize = 0;
//}
//
//UniformByteBuffer::UniformByteBuffer(const UniformByteBuffer& rhs)
//{
//  CopyFrom(rhs);
//}
//
//UniformByteBuffer::~UniformByteBuffer()
//{
//  Clear();
//}
//
//UniformByteBuffer& UniformByteBuffer::operator=(const UniformByteBuffer& rhs)
//{
//  if(&rhs == this)
//    return *this;
//
//  CopyFrom(rhs);
//  return *this;
//}
//
//void UniformByteBuffer::Set(byte* data, size_t size)
//{
//  Clear();
//  mSize = size;
//  mData = new byte[mSize]();
//  memcpy(mData, data, mSize);
//}
//
//void UniformByteBuffer::CopyFrom(const UniformByteBuffer& rhs)
//{
//  Set(rhs.mData, rhs.mSize);
//}
//
//void UniformByteBuffer::Clear()
//{
//  delete mData;
//  mData = nullptr;
//  mSize = 0;
//}
//
//GenericProperty::GenericProperty()
//{
//  mData = nullptr;
//  mSize = 0;
//}
//
//GenericProperty::GenericProperty(const GenericProperty& rhs)
//{
//  CopyFrom(rhs);
//}
//
//GenericProperty::~GenericProperty()
//{
//  Clear();
//}
//
//GenericProperty& GenericProperty::operator=(const GenericProperty& rhs)
//{
//  if(&rhs == this)
//    return *this;
//
//  CopyFrom(rhs);
//  return *this;
//}
//
//void GenericProperty::Set(const byte* data, size_t size)
//{
//  if(size != mSize)
//  {
//    Clear();
//    mSize = size;
//    mData = new byte[mSize]();
//  }
//  memcpy(mData, data, mSize);
//}
//
//void GenericProperty::Set(size_t size)
//{
//  Clear();
//  mSize = size;
//  mData = new byte[mSize]();
//  memset(mData, 0, mSize);
//}
//
//void GenericProperty::CopyFrom(const GenericProperty& rhs)
//{
//  Set(rhs.mData, rhs.mSize);
//}
//
//void GenericProperty::Clear()
//{
//  delete mData;
//  mData = nullptr;
//  mSize = 0;
//}
