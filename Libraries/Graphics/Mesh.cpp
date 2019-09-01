///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"

#include "Mesh.hpp"

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

Mesh::Mesh()
{
  mElementType = MeshElementType::Triangles;
}

//
//int Mesh::mPositionLocation = 0;
//int Mesh::mNormalLocation = 1;
//int Mesh::mUvLocation = 4;
//
//void Mesh::Bind()
//{
//  glGenVertexArrays(1, &mTriangleArray);
//  glBindVertexArray(mTriangleArray);
//
//  glGenBuffers(1, &mTriangleVertex);
//  glBindBuffer(GL_ARRAY_BUFFER, mTriangleVertex);
//
//  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mVertices.size(), mVertices.data(), GL_STATIC_DRAW);
//
//  //glEnableVertexAttribArray(0);
//  //glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mColor));
//  glEnableVertexAttribArray(mPositionLocation);
//  glVertexAttribPointer(mPositionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mPosition));
//  glEnableVertexAttribArray(mNormalLocation);
//  glVertexAttribPointer(mNormalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mNormal));
//  glEnableVertexAttribArray(mUvLocation);
//  glVertexAttribPointer(mUvLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mUv));
//  
//  glGenBuffers(1, &mTriangleIndex);
//  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mTriangleIndex);
//  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mIndices.size(), mIndices.data(), GL_STATIC_DRAW);
//
//  glBindVertexArray(0);
//}
//
