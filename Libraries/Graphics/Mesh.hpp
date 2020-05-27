///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GraphicsStandard.hpp"
#include "Resource.hpp"

namespace Graphics
{

//-------------------------------------------------------------------MeshElementType
struct MeshElementType
{
  enum Enum
  {
    None,
    Points,
    Lines,
    Triangles,
    Count
  };

  DeclareEnumType(MeshElementType);

  static Enum FromString(const String& enumName);
  static String ToString(Enum enumValue);
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
class Mesh : public Engine::Resource
{
public:
  Mesh();

  void Serialize(Serializer& serializer) override;

  typedef Array<Vertex> Vertices;
  Vertices mVertices;
  typedef Array<uint32_t> Indices;
  Indices mIndices;

  size_t mIndexCount;
  MeshElementType mElementType;
  int mPatchSize;
};

//-------------------------------------------------------------------MeshLibrary
class MeshLibrary : public Engine::ResourceLibrary<Mesh>
{
public:
  DeclareResourceLibraryType(MeshLibrary);
};

}//namespace Graphics
