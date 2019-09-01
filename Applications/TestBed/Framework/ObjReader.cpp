/////////////////////////////////////////////////////////////////////////////////
/////
///// Authors: Joshua Davis
///// Copyright 2015, DigiPen Institute of Technology
/////
/////////////////////////////////////////////////////////////////////////////////
////#include "Precompiled.hpp"
//#include "ObjReader.hpp"
//
//#include <vector>
//
//namespace Helpers
//{
//
//struct Vertex
//{
//  Math::Vector4 mPosition;
//};
//
//struct TextureCoords
//{
//  Math::Vector2 mUvs;
//};
//
//struct Normals
//{
//  Math::Vector3 mNormal;
//};
//
//struct Parameters
//{
//
//};
//
//struct Face
//{
//  struct Indices
//  {
//    int mVertexIndex;
//    int mTextureIndex;
//    int mNormalIndex;
//  };
//
//  std::vector<Indices> mIndices;
//};
//
//struct Parser
//{
//  float ReadFloat(const char*& pos)
//  {
//    const char* startPos = pos;
//    if(*pos == '+' || *pos == '-')
//      ++pos;
//    while(*pos >= '0' && *pos <= '9')
//      ++pos;
//    if(*pos == '.')
//    {
//      ++pos;
//      while(*pos >= '0' && *pos <= '9')
//        ++pos;
//    }
//    if(*pos == 'E' || *pos == 'e')
//    {
//      ++pos;
//      if(*pos == '-' || *pos == '+')
//        ++pos;
//      while(*pos >= '0' && *pos <= '9')
//        ++pos;
//    }
//
//    if(startPos == pos)
//      return 1.0f;
//
//    std::string buffer;
//    while(startPos != pos)
//    {
//      buffer.push_back(*startPos);
//      ++startPos;
//    }
//
//    return (float)atof(buffer.c_str());
//  }
//
//  int ReadInteger(const char*& pos, int defaultValue)
//  {
//    const char* startPos = pos;
//    while(*pos >= '0' && *pos <= '9')
//      ++pos;
//
//    if(startPos == pos)
//      return defaultValue;
//
//    std::string buffer;
//    while(startPos != pos)
//    {
//      buffer.push_back(*startPos);
//      ++startPos;
//    }
//    return atoi(buffer.c_str());
//  }
//
//  void ReadWhitespace(const char*& pos)
//  {
//    while(*pos == ' ' || *pos == '\t')
//      ++pos;
//  }
//
//
//
//  bool IsLineEnd(const char* pos)
//  {
//    return *pos == '\n' || *pos == NULL;
//
//  }
//  void ReadTillNewline(const char*& pos)
//  {
//    while(IsLineEnd(pos) == false)
//      ++pos;
//    ++pos;
//  }
//
//  Face::Indices ReadIndex(const char*& pos)
//  {
//    Face::Indices index0;
//    ReadWhitespace(pos);
//    index0.mVertexIndex = ReadInteger(pos, -1);
//    if(*pos == '\\')
//    {
//      ++pos;
//      if(*pos == '\\')
//      {
//        ++pos;
//        index0.mNormalIndex = ReadInteger(pos, -1);
//      }
//      else
//      {
//        index0.mTextureIndex = ReadInteger(pos, -1);
//        if(*pos == '\\')
//        {
//          ++pos;
//          index0.mNormalIndex = ReadInteger(pos, -1);
//        }
//      }
//    }
//    return index0;
//  }
//
//  void Parse(std::string str, std::vector<Vertex>& vertices, std::vector<Face>& faces)
//  {
//    const char* pos = str.c_str();
//
//    while(pos < str.c_str() + str.size())
//    {
//      switch(*pos)
//      {
//        case '#':
//        {
//          ++pos;
//          ReadTillNewline(pos);
//          break;
//        }
//        case 'v':
//        {
//          ++pos;
//
//          ReadWhitespace(pos);
//          Vertex v;
//          v.mPosition.x = ReadFloat(pos);
//          ReadWhitespace(pos);
//          v.mPosition.y = ReadFloat(pos);
//          ReadWhitespace(pos);
//          v.mPosition.z = ReadFloat(pos);
//          ReadWhitespace(pos);
//          v.mPosition.w = ReadFloat(pos);
//          ReadTillNewline(pos);
//          vertices.push_back(v);
//          break;
//        }
//        case 'f':
//        {
//          ++pos;
//
//          Face f;
//          ReadWhitespace(pos);
//          while(!IsLineEnd(pos))
//          {
//            f.mIndices.push_back(ReadIndex(pos));
//            ReadWhitespace(pos);
//          }
//          ++pos;
//          faces.push_back(f);
//          break;
//        }
//        case 'g':
//        {
//          ReadTillNewline(pos);
//          break;
//        }
//        default:
//        {
//          ReadTillNewline(pos);
//          break;
//        }
//      }
//    }
//  }
//
//  void Load(std::string str, Vector3*& outVertices, size_t& outVertexCount, size_t*& outIndices, size_t& outIndexCount)
//  {
//    std::vector<Vertex> vertices;
//    std::vector<Face> faces;
//
//    Parse(str, vertices, faces);
//
//    outVertexCount = vertices.size();
//    outVertices = new Vector3[outVertexCount];
//    for(size_t i = 0; i < vertices.size(); ++i)
//      outVertices[i] = Vector3(vertices[i].mPosition.array);
//
//    outIndexCount = faces.size() * 3;
//    outIndices = new size_t[outIndexCount];
//    for(size_t i = 0; i < faces.size(); ++i)
//    {
//      Face& face = faces[i];
//
//      outIndices[i * 3 + 0] = face.mIndices[0].mVertexIndex - 1;
//      outIndices[i * 3 + 1] = face.mIndices[1].mVertexIndex - 1;
//      outIndices[i * 3 + 2] = face.mIndices[2].mVertexIndex - 1;
//    }
//  }
//};
//
//std::string LoadFileData(const char* filePath)
//{
//  const size_t bufferSize = 1000;
//  char buffer[bufferSize];
//  size_t size;
//
//  std::string fileData;
//  FILE* file;
//  fopen_s(&file, filePath, "r");
//  if(file == nullptr)
//    return std::string();
//
//  do
//  {
//    size = fread_s(buffer, bufferSize - 1, 1, bufferSize - 1, file);
//    buffer[size] = NULL;
//    fileData += buffer;
//  } while(size != 0);
//  fclose(file);
//
//  return fileData;
//}
//
//void LoadObjFile(const char* filePath, Vector3*& outVertices, size_t& outVertexCount, size_t*& outIndices, size_t& outIndexCount)
//{
//  Parser parser;
//  std::string fileData = LoadFileData(filePath);
//  parser.Load(fileData, outVertices, outVertexCount, outIndices, outIndexCount);
//}
//
//void LoadObjFile(const std::string& filePath, std::vector<Vector3>& vertices, std::vector<size_t>& indices)
//{
//  Parser parser;
//  std::string fileData = LoadFileData(filePath.c_str());
//
//  Vector3* outVertices;
//  size_t* outIndices;
//  size_t outIndexCount;
//  size_t outVertexCount;
//  parser.Load(fileData, outVertices, outVertexCount, outIndices, outIndexCount);
//
//  vertices.resize(outVertexCount);
//  for(size_t i = 0; i < outVertexCount; ++i)
//    vertices[i] = outVertices[i];
//
//  indices.resize(outIndexCount);
//  for(size_t i = 0; i < outIndexCount; ++i)
//    indices[i] = outIndices[i];
//}
//
//void SaveObjFile(const char* filePath, const Vector3* vertices, size_t vertexCount, const size_t* indices, size_t indicesCount)
//{
//  FILE* file;
//  fopen_s(&file, filePath, "w");
//  for(size_t i = 0; i < vertexCount; ++i)
//  {
//    Vector3 vert = vertices[i];
//    fprintf(file, "v %g %g %g\n", vert.x, vert.y, vert.z);
//  }
//  for(size_t i = 0; i < indicesCount; i += 3)
//  {
//    fprintf(file, "f %d %d %d\n", indices[i + 0] + 1, indices[i + 1] + 1, indices[i + 2] + 1);
//  }
//  fclose(file);
//}
//
//void FreeVertexArray(Vector3* vertices)
//{
//  delete[] vertices;
//}
//
//void FreeIndexArray(size_t* indices)
//{
//  delete[] indices;
//}
//
//}//namespace Helpers
