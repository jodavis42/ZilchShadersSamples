///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"

#include "OpenGlRenderer.hpp"

#define GLEW_STATIC
#include "glew.h"

#include "Mesh.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

namespace Graphics
{

static int mNormalLocation = 2;
static int mPositionLocation = 3;
static int mUvLocation = 1;
static int mColorLocation = 4;

bool ReportOpenGLErrors(const char* file, int line)
{
  // Get the error code from OpenGL
  GLenum errorCode = glGetError();

  // If there actually was an error...
  if(errorCode != GL_NO_ERROR)
  {
    // Store the last slash (if we don't find one, just start at the beginning!)
    const char* fileLastSlash = file;

    // We only want to get the last slash in the file to just show the file name
    while(*file != NULL)
    {
      // Get the current character
      char c = *file;

      // If the character is a slash...
      if(c == '\\' || c == '/')
      {
        // Update the last slash we've seen (move ahead of the slash by 1)
        fileLastSlash = file + 1;
      }

      // Move forward in the string
      ++file;
    }

    // Print the error out to the console and return that an error occurred
    const GLubyte* errorStr = gluErrorString(errorCode);
    ZPrint("OpenGL Error %s(%d): %s\n", fileLastSlash, line, errorStr);
    return true;
  }

  // No error occurred!
  return false;
}
#define PrintOpenGLErrors() ReportOpenGLErrors(__FILE__, __LINE__)

//-------------------------------------------------------------------GlMeshData
class GlMeshData
{
public:
  size_t mIndexCount;
  size_t mVertexCount;

  unsigned int mVertexArray;
  unsigned int mVertexBuffer;
  unsigned int mIndexBuffer;
  unsigned int mElementType;
};

//-------------------------------------------------------------------GlTextureData
class GlTextureData
{
public:
  unsigned int mTextureId;
};

//-------------------------------------------------------------------GlShaderData
class GlShaderData
{
public:
  int mVertexShaderId;
  int mPixelShaderId;
  int mProgramId;

  int mComputeShaderId;
  int mComputeProgramId;
};

//-------------------------------------------------------------------GlBufferData
class GlBufferData
{
public:
  int mBufferId;
  int mBufferType;
  int mUsage;
  size_t mSizeInBytes;
};

//-------------------------------------------------------------------GlTextureEnums
struct GlTextureEnums
{
  GLint mInternalFormat;
  GLint mFormat;
  GLint mType;
};

GlTextureEnums gTextureEnums[] =
{
  {/* internalFormat    , format            , type */                          }, // None
  {GL_R8                , GL_RED            , GL_UNSIGNED_BYTE                 }, // R8
  {GL_RG8               , GL_RG             , GL_UNSIGNED_BYTE                 }, // RG8
  {GL_RGB8              , GL_RGB            , GL_UNSIGNED_BYTE                 }, // RGB8
  {GL_RGBA8             , GL_RGBA           , GL_UNSIGNED_BYTE                 }, // RGBA8
  {GL_R16               , GL_RED            , GL_UNSIGNED_SHORT                }, // R16
  {GL_RG16              , GL_RG             , GL_UNSIGNED_SHORT                }, // RG16
  {GL_RGB16             , GL_RGB            , GL_UNSIGNED_SHORT                }, // RGB16
  {GL_RGBA16            , GL_RGBA           , GL_UNSIGNED_SHORT                }, // RGBA16
  {GL_R16F              , GL_RED            , GL_HALF_FLOAT                    }, // R16f
  {GL_RG16F             , GL_RG             , GL_HALF_FLOAT                    }, // RG16f
  {GL_RGB16F            , GL_RGB            , GL_HALF_FLOAT                    }, // RGB16f
  {GL_RGBA16F           , GL_RGBA           , GL_HALF_FLOAT                    }, // RGBA16f
  {GL_R32F              , GL_RED            , GL_FLOAT                         }, // R32f
  {GL_RG32F             , GL_RG             , GL_FLOAT                         }, // RG32f
  {GL_RGB32F            , GL_RGB            , GL_FLOAT                         }, // RGB32f
  {GL_RGBA32F           , GL_RGBA           , GL_FLOAT                         }, // RGBA32f
  {GL_SRGB8             , GL_RGB            , GL_UNSIGNED_BYTE                 }, // SRGB8
  {GL_SRGB8_ALPHA8      , GL_RGBA           , GL_UNSIGNED_BYTE                 }, // SRGB8A8
  {GL_DEPTH_COMPONENT16 , GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT                }, // Depth16
  {GL_DEPTH_COMPONENT24 , GL_DEPTH_COMPONENT, GL_UNSIGNED_INT                  }, // Depth24
  {GL_DEPTH_COMPONENT32 , GL_DEPTH_COMPONENT, GL_UNSIGNED_INT                  }, // Depth32
  {GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT                         }, // Depth32f
  {GL_DEPTH24_STENCIL8  , GL_DEPTH_STENCIL  , GL_UNSIGNED_INT_24_8             }, // Depth24Stencil8
  {GL_DEPTH32F_STENCIL8 , GL_DEPTH_STENCIL  , GL_FLOAT_32_UNSIGNED_INT_24_8_REV}  // Depth32fStencil8Pad24
};

//-------------------------------------------------------------------OpenGlRenderer
OpenGlRenderer::OpenGlRenderer()
{
  glewInit();
}

OpenGlRenderer::~OpenGlRenderer()
{

}

void OpenGlRenderer::Initialize()
{
  glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
}

void OpenGlRenderer::CreateMesh(Mesh* mesh)
{
  GlMeshData* glMesh = new GlMeshData();

  glMesh->mIndexCount = mesh->mIndices.Size();
  glMesh->mVertexCount = mesh->mVertices.Size();
  glMesh->mElementType = GetElementType(mesh->mElementType);

  glGenVertexArrays(1, &glMesh->mVertexArray);
  glBindVertexArray(glMesh->mVertexArray);

  glGenBuffers(1, &glMesh->mVertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, glMesh->mVertexBuffer);

  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh->mVertices.Size(), mesh->mVertices.Data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(mPositionLocation);
  glVertexAttribPointer(mPositionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mPosition));
  glEnableVertexAttribArray(mNormalLocation);
  glVertexAttribPointer(mNormalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mNormal));
  glEnableVertexAttribArray(mUvLocation);
  glVertexAttribPointer(mUvLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mUv));
  glEnableVertexAttribArray(mColorLocation);
  glVertexAttribPointer(mColorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mColor));
  
  glMesh->mIndexCount = mesh->mIndexCount;
  glMesh->mIndexBuffer = -1;
  if(!mesh->mIndices.Empty())
  {
    glMesh->mIndexCount = mesh->mIndices.Size();
    glGenBuffers(1, &glMesh->mIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glMesh->mIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->mIndices.Size(), mesh->mIndices.Data(), GL_STATIC_DRAW);
  }
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  mGlMeshMap[mesh] = glMesh;
}

void OpenGlRenderer::DestroyMesh(Mesh* mesh)
{
  GlMeshData* glMesh = mGlMeshMap[mesh];
  mGlMeshMap.Erase(mesh);

  glDeleteBuffers(1, &glMesh->mVertexBuffer);
  glDeleteBuffers(1, &glMesh->mIndexBuffer);
  glDeleteVertexArrays(1, &glMesh->mVertexArray);

  delete glMesh;
}

void OpenGlRenderer::CreateTexture(Texture* texture)
{
  GlTextureData* glTexture = new GlTextureData();

  int sizeX = texture->mSizeX;
  int sizeY = texture->mSizeY;

  GlTextureEnums textureFormat = gTextureEnums[texture->mFormat];
  GLint textureType = GetTextureType(texture->mType);

  glGenTextures(1, &glTexture->mTextureId);
  glBindTexture(GL_TEXTURE_2D, glTexture->mTextureId);

  glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GetTextureAddressing(texture->mAddressingX));
  glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GetTextureAddressing(texture->mAddressingY));
  glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GetTextureMinFiltering(texture->mMinFilter));
  glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GetTextureMagFiltering(texture->mMagFilter));

  glTexImage2D(textureType, 0, textureFormat.mInternalFormat, sizeX, sizeY, 0, textureFormat.mFormat, textureFormat.mType, texture->mTextureData.Data());
  glGenerateMipmap(textureType);
  glBindTexture(textureType, 0);

  mTextureMap[texture] = glTexture;
}

void OpenGlRenderer::DestroyTexture(Texture* texture)
{
  GlTextureData* glTexture = mTextureMap[texture];
  mTextureMap.Erase(texture);
  delete glTexture;
}

void OpenGlRenderer::CreateShader(Shader* shader)
{
  GlShaderData* glShader = new GlShaderData();

  {
    Array<int> ids;

    if(CompileShaderInternal(shader->mShaderSource[Zero::FragmentType::Vertex], GL_VERTEX_SHADER, glShader->mVertexShaderId))
      ids.PushBack(glShader->mVertexShaderId);
    if(CompileShaderInternal(shader->mShaderSource[Zero::FragmentType::Pixel], GL_FRAGMENT_SHADER, glShader->mPixelShaderId))
      ids.PushBack(glShader->mPixelShaderId);

    LinkInternal(ids, glShader->mProgramId);
  }
  {
    Array<int> ids;

    if(CompileShaderInternal(shader->mShaderSource[Zero::FragmentType::Compute], GL_COMPUTE_SHADER, glShader->mComputeShaderId))
      ids.PushBack(glShader->mComputeShaderId);

    LinkInternal(ids, glShader->mProgramId);
  }
  mShaderMap[shader] = glShader;
}

void OpenGlRenderer::DestroyShader(Shader* shader)
{
  GlShaderData* glShader = mShaderMap[shader];
  mShaderMap.Erase(shader);

  glDeleteProgram(glShader->mProgramId);
  glDeleteShader(glShader->mPixelShaderId);
  glDeleteShader(glShader->mVertexShaderId);

  delete glShader;
}

BufferRenderData OpenGlRenderer::CreateBuffer(BufferCreationData& creationData, BufferType::Enum bufferType)
{
  BufferRenderData result;
  GlBufferData* glBuffer = new GlBufferData();
  result.mKey.mKey = glBuffer;

  GLuint id;
  glGenBuffers(1, &id);
  glBuffer->mBufferId = id;
  glBuffer->mBufferType = GetBufferType(bufferType);
  if(bufferType == BufferType::Uniform)
    glBuffer->mUsage = GL_DYNAMIC_DRAW;
  else if(bufferType == BufferType::StructuredStorage)
    glBuffer->mUsage = GL_STATIC_DRAW;
  glBuffer->mSizeInBytes = creationData.mSizeInBytes;

  if(creationData.mBufferData != nullptr)
  {
    UploadBuffer(result, *creationData.mBufferData);
  }

  PrintOpenGLErrors();
  return result;
}

void OpenGlRenderer::UploadBuffer(BufferRenderData& renderData, ByteBuffer& data)
{
  GlBufferData* glBuffer = (GlBufferData*)renderData.mKey.mKey;
  glBindBuffer(glBuffer->mBufferType, glBuffer->mBufferId);
  glBufferData(glBuffer->mBufferType, data.Size(), data.Data(), glBuffer->mUsage);
  glBindBuffer(glBuffer->mBufferType, 0);
}

void* OpenGlRenderer::MapBuffer(BufferRenderData& renderData, size_t offset, size_t sizeInBytes, BufferMappingType::Enum mappingTypes)
{
  PrintOpenGLErrors();
  int access = 0;
  if(mappingTypes & BufferMappingType::Read)
    access |= GL_MAP_READ_BIT;
  if(mappingTypes & BufferMappingType::Write)
    access |= GL_MAP_WRITE_BIT;
  if(mappingTypes & BufferMappingType::InvalidateRange)
    access |= GL_MAP_INVALIDATE_RANGE_BIT;
  if(mappingTypes & BufferMappingType::InvalidateBuffer)
    access |= GL_MAP_INVALIDATE_BUFFER_BIT;

  GlBufferData* glBuffer = (GlBufferData*)renderData.mKey.mKey;
  glBindBuffer(glBuffer->mBufferType, glBuffer->mBufferId);
  if(mappingTypes != BufferMappingType::Read)
    glBufferData(glBuffer->mBufferType, sizeInBytes, NULL, glBuffer->mUsage);
  return glMapBufferRange(glBuffer->mBufferType, offset, sizeInBytes, access);
}

void OpenGlRenderer::UnMapBuffer(BufferRenderData& renderData)
{
  GlBufferData* glBuffer = (GlBufferData*)renderData.mKey.mKey;
  glUnmapBuffer(glBuffer->mBufferType);
  glBindBuffer(glBuffer->mBufferType, 0);
}

void OpenGlRenderer::DestroyBuffer(BufferRenderData& renderData)
{
  GlBufferData* glBuffer = (GlBufferData*)renderData.mKey.mKey;
  GLuint id = glBuffer->mBufferId;
  glDeleteBuffers(1, &id);
  delete glBuffer;
}

void OpenGlRenderer::ClearTarget()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGlRenderer::Draw(ObjectData& objData)
{
  GlMeshData* glMesh = mGlMeshMap.FindValue(objData.mMesh, nullptr);
  GlShaderData* glShader = mShaderMap.FindValue(objData.mShader, nullptr);

  glUseProgram(glShader->mProgramId);
  
  BindInternal(glShader, objData);

  glBindVertexArray(glMesh->mVertexArray);
  if(glMesh->mIndexBuffer == -1)
    // If nothing is bound, glDrawArrays will invoke the shader pipeline the given number of times
    glDrawArrays(glMesh->mElementType, 0, glMesh->mIndexCount);
  else
    glDrawElements(glMesh->mElementType, glMesh->mIndexCount, GL_UNSIGNED_INT, (void*)0);
  glBindVertexArray(0);

  UnBindInternal(glShader, objData);
  
  glUseProgram(0);
  PrintOpenGLErrors();
}

void OpenGlRenderer::DispatchCompute(ObjectData& objData, int x, int y, int z)
{
  GlShaderData* glShader = mShaderMap.FindValue(objData.mShader, nullptr);

  glUseProgram(glShader->mProgramId);

  BindInternal(glShader, objData);

  glDispatchCompute(x, y, z);

  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_BUFFER_UPDATE_BARRIER_BIT);
  glUseProgram(0);
}

void OpenGlRenderer::Reshape(int width, int height, float aspectRatio)
{
  glViewport(0, 0, width, height);
}

Matrix4 OpenGlRenderer::BuildPerspectiveMatrix(float verticalFov, float aspectRatio, float nearDistance, float farDistance)
{
  Matrix4 matrix = Matrix4::cIdentity;

  // Near and far distances are expected to be positive
  float depth = farDistance - nearDistance;
  if(depth < 0.0001f || aspectRatio < 0.0001f)
    return matrix;

  // horizontal+ (fixed vertical fov)
  //
  //            /|  |
  //          /  | top
  // fov/2__/    |  |
  //      /_|____|  |
  //      --near--

  // tan(fov/2) = t/n
  float n_t = Math::Cot(verticalFov * 0.5f);

  // r = t*(r/t) = t*aspect
  // n/r = n/(t*aspect) = (n/t)/aspect
  float n_r = n_t / aspectRatio;

  matrix.m00 = n_r;
  matrix.m11 = n_t;
  matrix.m22 = -(farDistance + nearDistance) / depth;
  matrix.m33 = 0.0f;
  matrix.m23 = -2.0f * farDistance * nearDistance / depth;
  matrix.m32 = -1.0f;
  return matrix;
}

ZilchShaderIRBackend* OpenGlRenderer::CreateBackend()
{
  auto backend = new Zero::ZilchShaderGlslBackend();
  backend->mTargetVersion = 440;
  return backend;
}

bool OpenGlRenderer::CompileShaderInternal(const String& shaderSource, int shaderType, int& shaderId)
{
  return CompileShaderInternal(shaderSource.c_str(), shaderSource.SizeInBytes(), shaderType, shaderId);
}

bool OpenGlRenderer::CompileShaderInternal(const char* shaderSource, size_t sourceLength, int shaderType, int& shaderId)
{
  if(sourceLength == 0)
    return false;

  shaderId = glCreateShader(shaderType);

  // Load the code
  const char* code = shaderSource;
  GLint length = sourceLength;
  glShaderSource(shaderId, 1, &code, &length);

  // Compile the shader
  glCompileShader(shaderId);

  // Pull the info log from the shader compilation (do this regardless of compilation error)
  GLint infoLogLength;
  glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
  GLchar* strInfoLog = (char*)alloca(infoLogLength + 1);
  glGetShaderInfoLog(shaderId, infoLogLength, NULL, strInfoLog);
  String log = strInfoLog;

  // Check for compiler errors
  GLint status = 0;
  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);

  // Report errors if compilation failed
  if(status == GL_FALSE)
  {
    Warn("%s: %s", ShaderTypeToString(shaderType), strInfoLog);
    return false;
  }
  PrintOpenGLErrors();
  return true;
}

bool OpenGlRenderer::LinkInternal(const Array<int>& shaderIds, int& programId)
{
  if(shaderIds.Empty())
    return false;

  //create the shader program
  programId = glCreateProgram();

  glBindAttribLocation(programId, mPositionLocation, "LocalPosition");
  glBindAttribLocation(programId, mNormalLocation, "LocalNormal");
  glBindAttribLocation(programId, mUvLocation, "Uv");
  glBindAttribLocation(programId, mColorLocation, "Color");

  // Attach shaders to program
  for(size_t i = 0; i < shaderIds.Size(); ++i)
  {
    glAttachShader(programId, shaderIds[i]);
  }

  // Link the program vertex shader and pixel shader
  glLinkProgram(programId);

  // Check for linker errors
  GLint status;
  glGetProgramiv(programId, GL_LINK_STATUS, &status);
  if(status == GL_FALSE)
  {
    // Print out linker errors
    GLint infoLogLength;
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);

    GLchar* strInfoLog = (GLchar*)alloca(infoLogLength + 1);
    glGetProgramInfoLog(programId, infoLogLength, NULL, strInfoLog);

    Warn("Linking: %s", strInfoLog);
    return false;
  }
  return true;
}

void OpenGlRenderer::BindInternal(GlShaderData* glShader, ObjectData& objData)
{
  for(size_t j = 0; j < objData.mEphemeralBuffers.Size(); ++j)
  {
    EphemeralBuffer& ephemeralBuffer = objData.mEphemeralBuffers[j];
    BufferRenderData renderData = CreateBuffer(ephemeralBuffer.mCreationData, ephemeralBuffer.mBufferType);
    ephemeralBuffer.mRenderData.mKey = renderData.mKey;
    GlBufferData* glBuffer = (GlBufferData*)ephemeralBuffer.mRenderData.mKey.mKey;
    BindInternal(glBuffer, &ephemeralBuffer.mRenderData);
  }
  for(size_t j = 0; j < objData.mTextures.Size(); ++j)
  {
    TextureData& textureData = objData.mTextures[j];
    GlTextureData* glTexture = mTextureMap.FindValue(textureData.mTexture, nullptr);
    if(glTexture != nullptr)
      BindTextureInternal(&textureData, glTexture);
  }
  for(size_t j = 0; j < objData.mBuffers.Size(); ++j)
  {
    BufferRenderData& bufferRenderData = objData.mBuffers[j];
    GlBufferData* glBuffer = (GlBufferData*)bufferRenderData.mKey.mKey;
    if(glBuffer != nullptr)
      BindInternal(glBuffer, &bufferRenderData);
  }
}

void OpenGlRenderer::UnBindInternal(GlShaderData* glShader, ObjectData& objData)
{
  for(size_t i = 0; i < objData.mEphemeralBuffers.Size(); ++i)
  {
    EphemeralBuffer& ephemeralBuffer = objData.mEphemeralBuffers[i];
    GlBufferData* glBuffer = (GlBufferData*)ephemeralBuffer.mRenderData.mKey.mKey;
    if(glBuffer != nullptr)
      UnBindInternal(glBuffer, &ephemeralBuffer.mRenderData);
  }
  for(size_t j = 0; j < objData.mBuffers.Size(); ++j)
  {
    BufferRenderData& bufferRenderData = objData.mBuffers[j];
    GlBufferData* glBuffer = (GlBufferData*)bufferRenderData.mKey.mKey;
    if(glBuffer != nullptr)
      UnBindInternal(glBuffer, &bufferRenderData);
  }
  DestroyTemporaryBindingsInternal(glShader, objData);
}

void OpenGlRenderer::DestroyTemporaryBindingsInternal(GlShaderData* glShader, ObjectData& objData)
{
  for(size_t j = 0; j < objData.mEphemeralBuffers.Size(); ++j)
  {
    EphemeralBuffer& ephemeralBuffer = objData.mEphemeralBuffers[j];
    DestroyBuffer(ephemeralBuffer.mRenderData);
  }
}

void OpenGlRenderer::BindTextureInternal(TextureData* textureData, GlTextureData* glTexture)
{
  int textureType = GetTextureType(textureData->mTexture->mType);
  // Clear anything bound to this texture unit
  glActiveTexture(GL_TEXTURE0 + textureData->mTextureSlot);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

  glBindTexture(textureType, glTexture->mTextureId);
}

void OpenGlRenderer::BindInternal(GlBufferData* glBufferData, BufferRenderData* renderData)
{
  glBindBufferBase(glBufferData->mBufferType, renderData->mBindingIndex, glBufferData->mBufferId);
}

void OpenGlRenderer::UnBindInternal(GlBufferData* glBufferData, BufferRenderData* renderData)
{
  glBindBufferBase(glBufferData->mBufferType, renderData->mBindingIndex, 0);
}

int OpenGlRenderer::GetElementType(MeshElementType::Enum elementType)
{
  switch(elementType)
  {
  case MeshElementType::Points: return GL_POINTS;
  case MeshElementType::Lines: return GL_LINES;
  case MeshElementType::Triangles: return GL_TRIANGLES;
  default: return 0;
  }
}

int OpenGlRenderer::GetTextureType(TextureType::Enum type)
{
  switch(type)
  {
  case TextureType::Texture2D: return GL_TEXTURE_2D;
  case TextureType::TextureCube: return GL_TEXTURE_CUBE_MAP;
  default: return 0;
  }
}

int OpenGlRenderer::GetTextureAddressing(TextureAddressing::Enum addressing)
{
  switch(addressing)
  {
  case TextureAddressing::Clamp: return GL_CLAMP;
  case TextureAddressing::Mirror: return GL_MIRRORED_REPEAT;
  case TextureAddressing::Repeat: return GL_REPEAT;
  default: return 0;
  }
}

int OpenGlRenderer::GetTextureMinFiltering(TextureFiltering::Enum filtering)
{
  switch(filtering)
  {
  case TextureFiltering::Nearest:   return GL_NEAREST_MIPMAP_NEAREST;
  case TextureFiltering::Bilinear:  return GL_LINEAR_MIPMAP_NEAREST;
  case TextureFiltering::Trilinear: return GL_LINEAR_MIPMAP_LINEAR;
  default: return 0;
  }
}

int OpenGlRenderer::GetTextureMagFiltering(TextureFiltering::Enum filtering)
{
  switch(filtering)
  {
  case TextureFiltering::Nearest:   return GL_NEAREST;
  case TextureFiltering::Bilinear:  return GL_LINEAR;
  case TextureFiltering::Trilinear: return GL_LINEAR;
  default: return 0;
  }
}

int OpenGlRenderer::GetBufferType(BufferType::Enum bufferType)
{
  switch(bufferType)
  {
  case BufferType::Uniform:   return GL_UNIFORM_BUFFER;
  case BufferType::StructuredStorage:   return GL_SHADER_STORAGE_BUFFER;
  default: return 0;
  }
}

}//namespace Graphics
