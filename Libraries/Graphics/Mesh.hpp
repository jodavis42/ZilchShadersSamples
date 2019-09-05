///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GraphicsStandard.hpp"
#include "ResourceLibrary.hpp"

namespace Graphics
{

//-------------------------------------------------------------------MeshElementType
struct MeshElementType
{
  enum Enum
  {
    Points,
    Lines,
    Triangles
  };
};

//-------------------------------------------------------------------Vertex
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

//-------------------------------------------------------------------Mesh
class Mesh : public Resource
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

//-------------------------------------------------------------------MeshLibrary
class MeshLibrary : public ResourceLibrary<Mesh>
{
public:
};

}//namespace Graphics
