///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GraphicsStandard.hpp"

#include "Renderer.hpp"

namespace Graphics
{

class TextureLibrary;
class Model;
class Material;

//-------------------------------------------------------------------MaterialBufferData
struct MaterialBufferData
{
  MaterialBufferData();

  String mName;
  BufferType::Enum mBufferType;
  ByteBuffer mBufferData;
  int mBindingIndex;
};

//-------------------------------------------------------------------MaterialRendererData
class MaterialRendererData
{
public:
  Array<MaterialBufferData> mMaterialBuffers;
  Array<TextureData> mTextures;
};

/// Helper to extract data from a model/material into the necessary renderer data (e.g. uniform buffers, etc...)
void ExtractMaterialData(Model* model, TextureLibrary* textureLibrary, MaterialRendererData& outputData);
void ExtractMaterialData(Material* material, TextureLibrary* textureLibrary, MaterialRendererData& outputData);

}//namespace Graphics
