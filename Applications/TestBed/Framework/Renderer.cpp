//#include "Precompiled.hpp"
//
#include "Renderer.hpp"

TextureData::TextureData()
{
  mTexture = nullptr;
  mTextureSlot = 0;
}

TextureData::TextureData(Texture* texture, int textureSlot)
{
  mTexture = texture;
  mTextureSlot = textureSlot;
}
