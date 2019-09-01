//#include "Precompiled.hpp"
//
#include "OpenGlRenderer.hpp"

#define GLEW_STATIC
#include "glew.h"

#include "Mesh.hpp"
#include "Texture.hpp"
#include "ShaderProgram.hpp"
#include "FrameworkZilchShaderGlslBackend.hpp"

class GlMeshData
{
public:
  size_t mIndexCount;
  size_t mVertexCount;

  unsigned int mTriangleArray;
  unsigned int mTriangleVertex;
  unsigned int mTriangleIndex;
};

class GlTextureData
{
public:
  unsigned int mTextureId;
};

class GlShaderData
{
public:
  int mVertexShaderId;
  int mPixelShaderId;
  int mProgramId;
};

class GlUniformBufferData
{
public:
  int mBufferId;
  int mBlockBinding;
};


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

static int mPositionLocation = 0;
static int mNormalLocation = 1;
static int mUvLocation = 4;
static int mColorLocation = 5;

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
    //const GLubyte* errorStr = gluErrorString(errorCode);
    //ZPrint("OpenGL Error %s(%d): %s\n", fileLastSlash, line, errorStr);
    return true;
  }

  // No error occurred!
  return false;
}
#define PrintOpenGLErrors() ReportOpenGLErrors(__FILE__, __LINE__)

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

  glGenVertexArrays(1, &glMesh->mTriangleArray);
  glBindVertexArray(glMesh->mTriangleArray);

  glGenBuffers(1, &glMesh->mTriangleVertex);
  glBindBuffer(GL_ARRAY_BUFFER, glMesh->mTriangleVertex);

  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh->mVertices.Size(), mesh->mVertices.Data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(mPositionLocation);
  glVertexAttribPointer(mPositionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mPosition));
  glEnableVertexAttribArray(mNormalLocation);
  glVertexAttribPointer(mNormalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mNormal));
  glEnableVertexAttribArray(mUvLocation);
  glVertexAttribPointer(mUvLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mUv));
  glEnableVertexAttribArray(mColorLocation);
  glVertexAttribPointer(mColorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mColor));

  glGenBuffers(1, &glMesh->mTriangleIndex);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glMesh->mTriangleIndex);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->mIndices.Size(), mesh->mIndices.Data(), GL_STATIC_DRAW);

  glBindVertexArray(0);

  mGlMeshMap[mesh] = glMesh;
}

void OpenGlRenderer::DestroyMesh(Mesh* mesh)
{
  GlMeshData* glMesh = mGlMeshMap[mesh];
  mGlMeshMap.Erase(mesh);

  glDeleteBuffers(1, &glMesh->mTriangleVertex);
  glDeleteBuffers(1, &glMesh->mTriangleIndex);
  glDeleteVertexArrays(1, &glMesh->mTriangleArray);

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

  glTexParameteri(textureType, GL_TEXTURE_WRAP_S, texture->mAddressingX);
  glTexParameteri(textureType, GL_TEXTURE_WRAP_T, texture->mAddressingY);
  glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, texture->mMinFilter);
  glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, texture->mMagFilter);

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

  Array<int> ids;

  if(CompileShaderInternal(shader->mShaderSource[Zero::FragmentType::Vertex], GL_VERTEX_SHADER, glShader->mVertexShaderId))
    ids.PushBack(glShader->mVertexShaderId);

  if(CompileShaderInternal(shader->mShaderSource[Zero::FragmentType::Pixel], GL_FRAGMENT_SHADER, glShader->mPixelShaderId))
    ids.PushBack(glShader->mPixelShaderId);

  LinkInternal(ids, glShader->mProgramId);

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

void OpenGlRenderer::CreateBuffer(UniformBuffer* buffer)
{
  GlUniformBufferData* glBuffer = new GlUniformBufferData();
  mUniformBufferMap[buffer] = glBuffer;

  GLuint id;
  glGenBuffers(1, &id);
  glBuffer->mBufferId = id;

  UpdateBufferDataInternal(buffer, glBuffer);
}

void OpenGlRenderer::UpdateBufferData(UniformBuffer* buffer)
{
  GlUniformBufferData* glBuffer = mUniformBufferMap[buffer];
  UpdateBufferDataInternal(buffer, glBuffer);
}

void OpenGlRenderer::DestroyBuffer(UniformBuffer* buffer)
{
  GlUniformBufferData* glBuffer = mUniformBufferMap[buffer];
  mUniformBufferMap.Erase(buffer);

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

  for(size_t j = 0; j < objData.mBuffersToBind.Size(); ++j)
  {
    UniformBuffer* buffer = objData.mBuffersToBind[j];
    CreateBuffer(buffer);
    GlUniformBufferData* glBuffer = mUniformBufferMap[buffer];
    BindBufferInternal(glShader, buffer, glBuffer);
  }
  for(size_t j = 0; j < objData.mPreBoundBuffers.Size(); ++j)
  {
    UniformBuffer* buffer = objData.mPreBoundBuffers[j];
    GlUniformBufferData* glBuffer = mUniformBufferMap[buffer];
    BindBufferInternal(glShader, buffer, glBuffer);
  }
  for(size_t j = 0; j < objData.mTextures.Size(); ++j)
  {
    TextureData& textureData = objData.mTextures[j];
    GlTextureData* glTexture = mTextureMap[textureData.mTexture];
    BindTextureInternal(&textureData, glTexture);
  }

  glBindVertexArray(glMesh->mTriangleArray);

  glDrawElements(GL_TRIANGLES, glMesh->mIndexCount, GL_UNSIGNED_INT, (void*)0);

  glBindVertexArray(0);
  glUseProgram(0);

  for(size_t j = 0; j < objData.mBuffersToBind.Size(); ++j)
  {
    UniformBuffer* buffer = objData.mBuffersToBind[j];
    DestroyBuffer(buffer);
  }
}

void OpenGlRenderer::Draw(RenderData& renderData)
{
  for(size_t i = 0; i < renderData.mObjects.Size(); ++i)
  {
    ObjectData& objData = renderData.mObjects[i];
    Draw(objData);
    //GlMeshData* glMesh = mGlMeshMap.FindValue(objData.mMesh, nullptr);
    //GlShaderData* glShader = mShaderMap.FindValue(objData.mShader, nullptr);
    //
    //glUseProgram(glShader->mProgramId);
    //
    //GLuint blockBinding = 0;
    //for(size_t j = 0; j < objData.mBuffersToBind.Size(); ++j)
    //{
    //  UniformBuffer* buffer = objData.mBuffersToBind[j];
    //  CreateBuffer(buffer);
    //  GlUniformBufferData* glBuffer = mUniformBufferMap[buffer];
    //  BindBufferInternal(glShader, buffer, glBuffer);
    //}
    //for(size_t j = 0; j < objData.mPreBoundBuffers.Size(); ++j)
    //{
    //  UniformBuffer* buffer = objData.mPreBoundBuffers[j];
    //  GlUniformBufferData* glBuffer = mUniformBufferMap[buffer];
    //  BindBufferInternal(glShader, buffer, glBuffer);
    //}
    //
    //glBindVertexArray(glMesh->mTriangleArray);
    //
    //glDrawElements(GL_TRIANGLES, glMesh->mIndexCount, GL_UNSIGNED_INT, (void*)0);
    //
    //glBindVertexArray(0);
    //glUseProgram(0);
    //
    //for(size_t j = 0; j < objData.mBuffersToBind.Size(); ++j)
    //{
    //  UniformBuffer* buffer = objData.mBuffersToBind[j];
    //  DestroyBuffer(buffer);
    //}
  }
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
  return new Zero::ZilchShaderGlslBackend();
}


//void GlRenderer::Reshape(int width, int height, float aspectRatio)
//{
//  mInternalData->mWidth = width;
//  mInternalData->mHeight = height;
//
//  // Resize the render targe textures (probably slow, figure out later)
//  for(size_t i = 0; i < mInternalData->mTextureIds.Size(); ++i)
//  {
//    glBindTexture(GL_TEXTURE_2D, mInternalData->mTextureIds[i]);
//    glTexImage2D(GL_TEXTURE_2D, 0, gTextureEnums[0].mInternalFormat, width, height, 0, gTextureEnums[0].mFormat, gTextureEnums[0].mType, NULL);
//  }
//  glBindTexture(GL_TEXTURE_2D, mInternalData->mDepthId);
//  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
//
//  glBindTexture(GL_TEXTURE_2D, 0);
//}
//
//
bool OpenGlRenderer::CompileShaderInternal(const String& shaderSource, int shaderType, int& shaderId)
{
  return CompileShaderInternal(shaderSource.c_str(), shaderSource.SizeInBytes(), shaderType, shaderId);
}

bool OpenGlRenderer::CompileShaderInternal(const char* shaderSource, size_t sourceLength, int shaderType, int& shaderId)
{
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

  //
  //// Report errors if compilation failed
  if(status == GL_FALSE)
  {
    Warn("%s: %s", ShaderTypeToString(shaderType), strInfoLog);
    return false;
  }
  //PrintOpenGLErrors();
  return true;
}

bool OpenGlRenderer::LinkInternal(const Array<int>& shaderIds, int& programId)
{
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

void OpenGlRenderer::BindBufferInternal(GlShaderData* glShader, UniformBuffer* buffer, GlUniformBufferData* glBuffer)
{
  GLint blockIndex = glGetUniformBlockIndex(glShader->mProgramId, buffer->mBufferName.c_str());

  GLint blockBinding = buffer->mId;
  //glUniformBlockBinding(glShader->mProgramId, blockIndex, blockBinding);

  glBindBufferBase(GL_UNIFORM_BUFFER, blockBinding, glBuffer->mBufferId);
  ++blockBinding;
}

void OpenGlRenderer::UpdateBufferDataInternal(UniformBuffer* buffer, GlUniformBufferData* glBuffer)
{
  glBindBuffer(GL_UNIFORM_BUFFER, glBuffer->mBufferId);
  glBufferData(GL_UNIFORM_BUFFER, buffer->mData.Size(), buffer->mData.Data(), GL_DYNAMIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OpenGlRenderer::BindTextureInternal(TextureData* textureData, GlTextureData* glTexture)
{
  int textureType = GetTextureType(textureData->mTexture->mType);
  // Clear anything bound to this texture unit
  glActiveTexture(GL_TEXTURE0 + textureData->mTextureSlot);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  //if(samplerObjects)
  //  glBindSampler(textureData->mTextureSlot, 0);
  // Bind texture
  glBindTexture(textureType, glTexture->mTextureId);
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

//
//void GlRenderer::SetBuffer(UniformBufferData& bufferData)
//{
//
//  // Get the expected buffer id
//  GLuint bufferId = bufferData.mBufferId;
//  // Get the actual uniform block's location
//  //GLint blockIndex =  glGetUniformBlockIndex(programId, uniformBuffer.mReflectionData.mTypeName.c_str());
//
//  // A block index can be invalid if it's optimized away
//  //if(blockIndex == GL_INVALID_INDEX)
//  //  return;
//
//  // Bind the updated buffer data
//  glBindBuffer(GL_UNIFORM_BUFFER, bufferId);
//  glBufferData(GL_UNIFORM_BUFFER, bufferData.mData.mSize, bufferData.mData.mData, GL_DYNAMIC_DRAW);
//  glBindBuffer(GL_UNIFORM_BUFFER, 0);
//  PrintOpenGLErrors();
//
//  // There's an extra level of indirection here. You have to map the buffer block's index to an actual binding id.
//
//  // We need to map the uniform buffer's location to the binding id. This is defaultly set
//  // to a value in the shader, but can optionally be overridden. This is necessary if
//  // the binding ids overlap in different shader stages.
//  GLuint blockBinding = bufferData.mBlockBinding; //uniformBuffer.mReflectionData.mBinding;
//                                                  //if(!shaderManager->mGenerator->mSettings->mAllowUniformMaterialBufferIndexOverap)
//                                                  //{
//                                                  //  blockBinding = mUniformBindingId;
//                                                  //  glUniformBlockBinding(programId, blockIndex, blockBinding);
//                                                  //}
//
//                                                  // Bind the binding point to the actual uniform buffer data
//  glBindBufferBase(GL_UNIFORM_BUFFER, blockBinding, bufferId);
//  //delete data;
//  //++mUniformBindingId;
//}
//
//
//const char* GlRenderer::ShaderTypeToString(int shaderType)
//{
//  switch(shaderType)
//  {
//  case GL_VERTEX_SHADER:
//    return "Vertex";
//  case GL_TESS_CONTROL_SHADER:
//    return "Tessalation Control";
//  case GL_TESS_EVALUATION_SHADER:
//    return "Tessalation Evaluation";
//  case GL_GEOMETRY_SHADER:
//    return "Geometry";
//  case GL_FRAGMENT_SHADER:
//    return "Pixel";
//  default:
//    return "Unknown";
//    break;
//  }
//}
//
//int GlRenderer::MeshTypeToGlMeshType(MeshElementType meshElementType)
//{
//  switch(meshElementType)
//  {
//  case Graphics::MeshElementType::Lines:
//    return GL_LINES;
//    break;
//  case Graphics::MeshElementType::Triangles:
//    return GL_TRIANGLES;
//    break;
//  default:
//    return -1;
//    break;
//  }
//}

