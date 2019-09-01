//#include "Precompiled.hpp"
//
#include "Texture.hpp"

Texture::Texture()
{
  mSizeX = mSizeY = 1;
  mType = TextureType::Texture2D;
  mFormat = TextureFormat::RGB32f;
  mMinFilter = TextureFiltering::Nearest;
  mMagFilter = TextureFiltering::Nearest;
  mAddressingX = TextureAddressing::Repeat;
  mAddressingY = TextureAddressing::Repeat;
}
