///////////////////////////////////////////////////////////////////////////////
///
/// Authors: Joshua Davis
/// Copyright 2018, DigiPen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////
#pragma once

//#include ""
#include "ZilchShadersStandard.hpp"

namespace Zero
{

//-------------------------------------------------------------------FrameworkZilchShaderGlslBackend
/// Zero's version of the glsl backend. Needed to
/// set specific options on the compiler for zero.
class FrameworkZilchShaderGlslBackend : public Zero::ZilchShaderGlslBackend
{
public:
  FrameworkZilchShaderGlslBackend();

  String GetExtension() override;
  bool RunTranslationPass(ShaderTranslationPassResult& inputData, ShaderTranslationPassResult& outputData) override;
  String GetErrorLog() override;

  int mTargetVersion;
  bool mTargetGlslEs;
  String mErrorLog;
};

}//namespace Zero