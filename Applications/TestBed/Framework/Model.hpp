/////////////////////////////////////////////////////////////////////////////////
/////
///// Authors: Joshua Davis
///// Copyright 2015, DigiPen Institute of Technology
/////
/////////////////////////////////////////////////////////////////////////////////
//#pragma once
//
//#include "Math/Math.hpp"
//#include <vector>
//#include "SpatialPartition.hpp"
//#include "Shapes.hpp"
//#include "Components.hpp"
//#include "Material.hpp"
//
//class Model : public Component
//{
//public:
//  Model();
//
//  // Component Interface
//  DeclareComponent(Model);
//  void Initialize();
//  void TransformUpdate(TransformUpdateFlags::Enum flags) override;
//  void DebugDraw() override;
//  void DisplayProperties(TwBar* bar) override;
//
//  // Update all world space bounding volumes (and recompute local if recompute is true)
//  void UpdateBoundingVolumes(bool recompute);
//  // Recompute the local space aabb
//  void ComputeAabb();
//  // Recompute the local space bounding sphere
//  void ComputeBoundingSphere();
//  // Update the world space aabb
//  void UpdateAabb();
//  // Update the world space bounding sphere
//  void UpdateBoundingSphere();
//
//  // Cast a ray against the mesh of the model (should use the midphase if it exists)
//  void CheckTriangle(const Ray& ray, const Triangle& tri, float& minT);
//  bool CastRayMidphase(const Ray& localRay, CastResult& castInfo);
//  bool CastRay(const Ray& worldRay, CastResult& castInfo);
//
//
//  void SetMidPhase(SpatialPartition* midPhase);
//
//  int GetMeshType();
//  void SetMeshType(const int& meshIndex);
//
//  int GetShaderType();
//  void SetShaderType(const int& shaderIndex);
//
//  void Draw();
//  
//  void DrawTriangle(Triangle& tri);
//
//  
//
//  std::vector<Triangle> GetWorldTriangles();
//  void SetWorldTriangles(const std::vector<Triangle>& tris);
//  
//
//  Mesh* mMesh;
//  ShaderProgram* mShader;
//  Aabb mLocalAabb;
//  Aabb mAabb;
//  Sphere mLocalSphere;
//  Sphere mBoundingSphere;
//  SpatialPartitionKey mSpatialPartitionKey;
//
//  int mOverlap;
//  int mMidphaseDrawLevel;
//
//  SpatialPartition* mMidPhase;
//
//  GenericProperties mProperties;
//  int mUniformBindingId;
//  Material mMaterial;
//};
