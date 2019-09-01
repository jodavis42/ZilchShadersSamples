#pragma once

#include "ZilchShadersStandard.hpp"
#include "FrameworkStandard.hpp"


class Shader
{
public:

  String mShaderSource[Zero::FragmentType::Size];
};

class UniformBuffer
{
public:
  String mBufferName;
  Array<byte> mData;
  int mId;
};

class Material;
class SampleZilchShaderIRGenerator : public Zero::SimpleZilchShaderIRGenerator
{
public:
  SampleZilchShaderIRGenerator(Zero::ZilchShaderSpirVSettings* settings);

  void CreateShader(Material& material, Shader& shader);
  static Zero::ZilchShaderSpirVSettings* CreateZilchShaderSettings(Zero::SpirVNameSettings& nameSettings);
};

