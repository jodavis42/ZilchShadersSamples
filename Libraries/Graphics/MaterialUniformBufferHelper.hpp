///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GraphicsStandard.hpp"

#include "Renderer.hpp"
#include "UniformBuffer.hpp"

namespace Graphics
{

class TextureLibrary;
class Model;

class MaterialRendererData
{
public:
  Array<UniformBuffer> mMaterialBuffers;
  Array<TextureData> mTextures;
};

/// Helper to extract data from a model/material into the necessary renderer data (e.g. uniform buffers, etc...)
void ExtractMaterialData(Model* model, TextureLibrary* textureLibrary, MaterialRendererData& outputData);

}//namespace Graphics
