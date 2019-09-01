///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GraphicsStandard.hpp"

namespace Graphics
{

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

}//namespace Graphics
