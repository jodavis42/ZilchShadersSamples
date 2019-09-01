///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GraphicsStandard.hpp"

namespace Graphics
{

struct TextureFormat
{
  enum Enum
  {
    None,
    R8, RG8, RGB8, RGBA8,                  // byte
    R16, RG16, RGB16, RGBA16,              // short
    R16f, RG16f, RGB16f, RGBA16f,          // half float
    R32f, RG32f, RGB32f, RGBA32f,          // float
    SRGB8, SRGB8A8,                        // gamma
    Depth16, Depth24, Depth32, Depth32f,   // depth
    Depth24Stencil8, Depth32fStencil8Pad24 // depth-stencil
  };
};

struct TextureType
{
  enum Enum
  {
    Texture2D, TextureCube
  };
};

struct TextureFiltering
{
  enum Enum
  {
    Nearest, Bilinear, Trilinear
  };
};

struct TextureAddressing
{
  enum Enum
  {
    Clamp, Repeat, Mirror
  };
};

class Texture
{
public:
  Texture();

  int mSizeX;
  int mSizeY;
  TextureType::Enum mType;
  TextureFormat::Enum mFormat;
  TextureFiltering::Enum mMinFilter;
  TextureFiltering::Enum mMagFilter;
  TextureAddressing::Enum mAddressingX;
  TextureAddressing::Enum mAddressingY;
  Array<float> mTextureData;
};

}//namespace Graphics
