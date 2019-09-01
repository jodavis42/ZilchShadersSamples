//#pragma once
//
//#include "Shapes.hpp"
//#include "Components.hpp"
//#include "Properties.hpp"
//
//class Mesh;
//class Application;
//
//class Material
//{
//public:
//  Material();
//  ~Material();
//
//  void RunRenderPass(Application* application, Mesh* mesh);
//  void RunComputePass(Application* application, int x, int y, int z);
//  void RunComputePass(Application* application, int vertexCount, int x, int y, int z);
//
//  void SetShaderUniforms(int programId, ShaderProgram* shader);
//  void SetShaderUniforms(int programId, ShaderProgram* shader, ShaderStageResource& uniformBuffer);
//
//  GenericProperties mProperties;
//  int mUniformBindingId;
//  ShaderProgram* mShader;
//  ShaderProgram* mComputeShader;
//
//  GLuint mSsboId;
//};

#include "FrameworkStandard.hpp"

class Material
{
public:
  Array<String> mFragmentNames;
  String mMaterialName;
};


