///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"
//
//#include "Material.hpp"
//
//#include "Application.hpp"
//#include "Main/Support.hpp"
//#include "Geometry.hpp"
//#include "DebugDraw.hpp"
//#include "BspTree.hpp"
//#include "SimplePropertyBinding.hpp"
//
//Material::Material()
//{
//  mShader = nullptr;
//  mComputeShader = nullptr;
//  mUniformBindingId = 0;
//  mSsboId = 0;
//}
//
//Material::~Material()
//{
//
//}
//
//void Material::RunRenderPass(Application* application, Mesh* mesh)
//{
//  if(mShader == nullptr)
//    return;
//  mShader->Set();
//
//  int programId = mShader->mShaderProgramId;
//  application->mCamera.SetMatrix(programId);
//
//  SetShaderUniforms(programId, mShader);
//  
//  glBindVertexArray(mesh->mTriangleArray);
//  if(mesh->mElementType == GL_PATCHES)
//    glPatchParameteri(GL_PATCH_VERTICES, mesh->mPatchSize);
//  glDrawElements(mesh->mElementType, mesh->mIndices.size(), GL_UNSIGNED_INT, (void*)0);
//  glBindVertexArray(0);
//
//  mShader->Clear();
//  PrintOpenGLErrors();
//}
//
//void Material::RunComputePass(Application* application, int x, int y, int z)
//{
//  if(mComputeShader != nullptr)
//  {
//    PrintOpenGLErrors();
//    int programId = mComputeShader->mShaderProgramId;
//    glUseProgram(programId);
//    PrintOpenGLErrors();
//    SetShaderUniforms(programId, mComputeShader);
//    PrintOpenGLErrors();
//    //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, mSsboId);
//
//    glDispatchCompute(x, y, z);
//    //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
//    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_BUFFER_UPDATE_BARRIER_BIT);
//    glUseProgram(0);
//    PrintOpenGLErrors();
//  }
//}
//
//void Material::RunComputePass(Application* application, int vertexCount, int x, int y, int z)
//{
//  if(mComputeShader != nullptr)
//  {
//    int programId = mComputeShader->mShaderProgramId;
//    glUseProgram(programId);
//    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, mSsboId);
//
//    SetShaderUniforms(programId, mComputeShader);
//
//    glDispatchCompute(x, y, z);
//    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
//    glUseProgram(0);
//    PrintOpenGLErrors();
//  }
//
//  if(mShader != nullptr)
//  {
//    mShader->Set();
//    
//    int programId = mShader->mShaderProgramId;
//    SetShaderUniforms(programId, mShader);
//    
//    glVertexPointer(4, GL_FLOAT, 0, (void*)0);
//    glEnableClientState(GL_VERTEX_ARRAY);
//    glDrawArrays(GL_POINTS, 0, vertexCount);
//    glDisableClientState(GL_VERTEX_ARRAY);
//    
//    mShader->Clear();
//    PrintOpenGLErrors();
//  }
//}
//
//void Material::SetShaderUniforms(int programId, ShaderProgram* shader)
//{
//  mUniformBindingId = 0;
//  ShaderManager* shaderManager = ShaderManager::GetInstance();
//
//  for(size_t i = 0; i < Zero::FragmentType::Size; ++i)
//  {
//    ShaderStageInterfaceReflection& stageInfo = shader->mReflectionData.mStages[i];
//    for(size_t j = 0; j < stageInfo.mUniforms.Size(); ++j)
//    {
//      ShaderStageResource& uniformBuffer = stageInfo.mUniforms[j];
//      SetShaderUniforms(programId, shader, uniformBuffer);
//    }
//    // Map sampled images from properties
//    for(size_t j = 0; j < stageInfo.mSampledImages.Size(); ++j)
//    {
//      ShaderStageResource& samplerData = stageInfo.mSampledImages[j];
//
//      int textureSlot = samplerData.mReflectionData.mBinding;
//      GenericProperty* prop = mProperties.mProperties.FindValue(samplerData.mReflectionData.mInstanceName, nullptr);
//      if(prop == nullptr)
//        continue;
//
//      int* textureId = prop->Get<int>();
//      glActiveTexture(GL_TEXTURE0 + textureSlot);
//      glBindTexture(GL_TEXTURE_2D, *textureId);
//    }
//    // Map storage images from properties
//    for(size_t j = 0; j < stageInfo.mStorageImages.Size(); ++j)
//    {
//      ShaderStageResource& imageData = stageInfo.mStorageImages[j];
//
//      int textureSlot = imageData.mReflectionData.mBinding;
//      GenericProperty* prop = mProperties.mProperties.FindValue(imageData.mReflectionData.mInstanceName, nullptr);
//      if(prop == nullptr)
//        continue;
//
//      int* textureId = prop->Get<int>();
//      int textureIndex = textureSlot;
//      // Hardcoded rgba32f
//      glBindImageTexture(textureIndex, *textureId, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
//    }
//    // Map storage buffers from properties
//    for(size_t j = 0; j < stageInfo.mStructedStorageBuffers.Size(); ++j)
//    {
//      ShaderStageResource& ssboData = stageInfo.mStructedStorageBuffers[j];
//      int bindingId = ssboData.mReflectionData.mBinding;
//      GenericProperty* prop = mProperties.mProperties.FindValue(ssboData.mReflectionData.mInstanceName, nullptr);
//      if(prop == nullptr)
//        continue;
//
//      int* ssboId = prop->Get<int>();
//      glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingId, *ssboId);
//
//      glBindBuffer(GL_SHADER_STORAGE_BUFFER, *ssboId);
//      char* data = (char*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, 12, GL_MAP_READ_BIT);
//      char* data2 = data;
//      glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
//      glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
//      //char temp = data[0];
//    }
//  }
//}
//
//void Material::SetShaderUniforms(int programId, ShaderProgram* shader, ShaderStageResource& uniformBuffer)
//{
//  ShaderManager* shaderManager = ShaderManager::GetInstance();
//
//  byte* data = new byte[uniformBuffer.mReflectionData.mSizeInBytes];
//  memset(data, 0, uniformBuffer.mReflectionData.mSizeInBytes);
//
//  for(size_t i = 0; i < uniformBuffer.mMembers.Size(); ++i)
//  {
//    ShaderResourceReflectionData& memberReflectionData = uniformBuffer.mMembers[i];
//
//    GenericProperty* prop = mProperties.mProperties.FindValue(memberReflectionData.mInstanceName, nullptr);
//    if(prop == nullptr || prop->mData == nullptr)
//      continue;
//
//    byte* start = data + memberReflectionData.mOffsetInBytes;
//    memcpy(start, prop->mData, memberReflectionData.mSizeInBytes);
//  }
//
//  // Get the expected buffer id
//  GLuint bufferId = shader->mBufferObjectIds[uniformBuffer.mReflectionData.mTypeName];
//  // Get the actual uniform block's location
//  GLint blockIndex = glGetUniformBlockIndex(programId, uniformBuffer.mReflectionData.mTypeName.c_str());
//  // A block index can be invalid if it's optimized away
//  if(blockIndex == GL_INVALID_INDEX)
//    return;
//
//  // Bind the updated buffer data
//  glBindBuffer(GL_UNIFORM_BUFFER, bufferId);
//  glBufferData(GL_UNIFORM_BUFFER, uniformBuffer.mReflectionData.mSizeInBytes, data, GL_DYNAMIC_DRAW);
//  glBindBuffer(GL_UNIFORM_BUFFER, 0);
//
//  // There's an extra level of indirection here. You have to map the buffer block's index to an actual binding id.
//
//  // We need to map the uniform buffer's location to the binding id. This is defaultly set
//  // to a value in the shader, but can optionally be overridden. This is necessary if
//  // the binding ids overlap in different shader stages.
//  GLuint blockBinding = uniformBuffer.mReflectionData.mBinding;
//  if(!shaderManager->mGenerator->mSettings->mAllowUniformMaterialBufferIndexOverap)
//  {
//    blockBinding = mUniformBindingId;
//    glUniformBlockBinding(programId, blockIndex, blockBinding);
//  }
//
//  // Bind the binding point to the actual uniform buffer data
//  glBindBufferBase(GL_UNIFORM_BUFFER, blockBinding, bufferId);
//  delete data;
//  ++mUniformBindingId;
//}
