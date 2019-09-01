///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"

#include "Mesh.hpp"

namespace Graphics
{

//-------------------------------------------------------------------Vertex
Vertex::Vertex()
{
  Set(Vector3::cZero, Vector3::cZAxis);
}

Vertex::Vertex(const Vector3& pos, const Vector3& normal)
{
  Set(pos, normal);
}

Vertex::Vertex(const Vector3& pos, const Vector3& normal, const Vector2& uv)
{
  Set(pos, normal, uv);
}

Vertex::Vertex(const Vector3& pos, const Vector3& normal, const Vector2& uv, const Vector4& color)
{
  Set(pos, normal, uv, color);
}

void Vertex::Set(const Vector3& pos, const Vector3& normal, const Vector2& uv, const Vector4& color)
{
  mPosition = pos;
  mNormal = normal;
  mUv = uv;
  mColor = color;
}

//-------------------------------------------------------------------Mesh
Mesh::Mesh()
{
  mElementType = MeshElementType::Triangles;
}

}//namespace Graphics
