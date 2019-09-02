///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GraphicsStandard.hpp"

#include "ByteBuffer.hpp"

namespace Graphics
{

//-------------------------------------------------------------------UniformBuffer
class UniformBuffer
{
public:
  UniformBuffer();

  String mBufferName;
  ByteBuffer mBufferData;
  int mId;
};

}//namespace Graphics
