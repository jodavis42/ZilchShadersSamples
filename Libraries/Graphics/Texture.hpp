///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GraphicsStandard.hpp"
#include "ResourceLibrary.hpp"

namespace Graphics
{

//-------------------------------------------------------------------TextureFormat
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

//-------------------------------------------------------------------TextureType
struct TextureType
{
  enum Enum
  {
    Texture2D, TextureCube
  };
};

//-------------------------------------------------------------------TextureFiltering
struct TextureFiltering
{
  enum Enum
  {
    Nearest, Bilinear, Trilinear
  };
};

//-------------------------------------------------------------------TextureAddressing
struct TextureAddressing
{
  enum Enum
  {
    Clamp, Repeat, Mirror
  };
};

//-------------------------------------------------------------------Texture
class Texture : public Resource
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
  String mName;
};

//-------------------------------------------------------------------TextureLibrary
class TextureLibrary : public ResourceLibrary<Texture>
{
public:
};

}//namespace Graphics
