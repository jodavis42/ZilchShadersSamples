///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"

#include "Mesh.hpp"
#include "Serializer.hpp"

namespace Graphics
{

//-------------------------------------------------------------------MeshElementType
MeshElementType::Enum MeshElementType::FromString(const String& enumName)
{
  if(enumName == "Points")
    return MeshElementType::Points;
  if(enumName == "Lines")
    return MeshElementType::Lines;
  if(enumName == "Triangles")
    return MeshElementType::Triangles;
  return MeshElementType::None;
}

String MeshElementType::ToString(Enum enumValue)
{
  if(enumValue == Points)
    return "Points";
  if(enumValue == Lines)
    return "Lines";
  if(enumValue == Triangles)
    return "Triangles";
  return "None";
}

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
  mIndexCount = 0;
}

void Mesh::Serialize(Serializer& serializer)
{
  __super::Serialize(serializer);
  serializer.SerializeEnumField("ElementType", mElementType);
  
  size_t vertexCount = serializer.BeginArray("Vertices");
  if(vertexCount != 0)
  {
    for(size_t i = 0; i < vertexCount; ++i)
    {
      Graphics::Vertex& vertex = mVertices.PushBack();
      serializer.BeginArrayItem(i);
      serializer.SerializeNamedField("Position", vertex.mPosition);
      serializer.SerializeNamedField("Normal", vertex.mNormal);
      serializer.SerializeNamedField("Uv", vertex.mUv);
      serializer.SerializeNamedField("Color", vertex.mColor);
      serializer.End();
    }
    serializer.End();
  }
  size_t indexCount = serializer.BeginArray("Indices");
  if(indexCount != 0)
  {
    for(size_t i = 0; i < indexCount; ++i)
    {
      int index;
      serializer.BeginArrayItem(i);
      serializer.SerializeValue(index);
      mIndices.PushBack(index);
      serializer.End();
    }
    serializer.End();
  }
  serializer.SerializeField(mIndexCount);
}

}//namespace Graphics
