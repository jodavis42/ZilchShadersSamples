///////////////////////////////////////////////////////////////////////////////
///
/// Authors: Joshua Davis
/// Copyright 2015, DigiPen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "FrameworkStandard.hpp"

struct MeshElementType
{
  enum Enum
  {
    Points,
    Lines,
    Triangles
  };
};

struct Vertex
{
  Vertex();
  Vertex(const Vector3& pos, const Vector3& normal);
  Vertex(const Vector3& pos, const Vector3& normal, const Vector2& uv);
  Vertex(const Vector3& pos, const Vector3& normal, const Vector2& uv, const Vector4& color);

  void Set(const Vector3& pos, const Vector3& normal, const Vector2& uv = Vector2::cZero, const Vector4& color = Vector4(1, 1, 1, 1));

  Vector3 mPosition;
  Vector3 mNormal;
  Vector2 mUv;
  Vector4 mColor;
};

class Mesh
{
public:
  Mesh();

  typedef Array<Vertex> Vertices;
  Vertices mVertices;
  typedef Array<size_t> Indices;
  Indices mIndices;

  MeshElementType::Enum mElementType;
  int mPatchSize;
  String mName;
};
//
//class Mesh
//{
//public:
//  Mesh() { mDynamic = false; mType = 0; }
//  Mesh(const std::string& name, int type)
//  {
//    mName = name;
//    mDynamic = false;
//    mType = type;
//  }
//
//  size_t TriangleCount() const
//  {
//    return mIndices.size() / 3;
//  }
//
//  Triangle TriangleAt(size_t triangleIndex) const
//  {
//    Triangle tri;
//    tri.mPoints[0] = mVertices[mIndices[triangleIndex * 3 + 0]].mPosition;
//    tri.mPoints[1] = mVertices[mIndices[triangleIndex * 3 + 1]].mPosition;
//    tri.mPoints[2] = mVertices[mIndices[triangleIndex * 3 + 2]].mPosition;
//    return tri;
//  }
//
//  std::vector<Vector3> GetVertexPositions()
//  {
//    std::vector<Vector3> vertices;
//    for(size_t i = 0; i < mVertices.size(); ++i)
//      vertices.push_back(mVertices[i].mPosition);
//    return vertices;
//  }
//
//  void Bind();
//
//  typedef std::vector<Vertex> Vertices;
//  Vertices mVertices;
//  typedef std::vector<size_t> Indices;
//  Indices mIndices;
//
//  std::string mName;
//  bool mDynamic;
//  int mType;
//  int mElementType;
//  int mPatchSize;
//  
//  unsigned int mTriangleArray;
//  unsigned int mTriangleVertex;
//  unsigned int mTriangleIndex;
//
//  static int mPositionLocation;
//  static int mNormalLocation;
//  static int mUvLocation;
//};


