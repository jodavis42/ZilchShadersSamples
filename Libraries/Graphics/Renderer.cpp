///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"

#include "Renderer.hpp"

namespace Graphics
{

//-------------------------------------------------------------------TextureData
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

}//namespace Graphics
