/////////////////////////////////////////////////////////////////////////////////
/////
///// Authors: Joshua Davis
///// Copyright 2015, DigiPen Institute of Technology
/////
/////////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"
//#include "Model.hpp"
//
//#include "Application.hpp"
//#include "Main/Support.hpp"
//#include "Geometry.hpp"
//#include "DebugDraw.hpp"
//#include "BspTree.hpp"
//#include "SimplePropertyBinding.hpp"
//
//GenericProperty* MakeOrCreateProperty(GenericProperties& properties, Model* model, ZilchShaderIRType* fragmentType, StringParam propertyName)
//{
//  ShaderProgram* shader = model->mShader;
//  String fullShaderPropertyName = shader->FindReflectionName(fragmentType, propertyName);
//
//  GenericProperty* prop = properties.mProperties.FindValue(fullShaderPropertyName, nullptr);
//  if(prop == nullptr)
//  {
//    prop = new GenericProperty();
//    properties.mProperties[fullShaderPropertyName] = prop;
//
//    for(size_t i = 0; i < fragmentType->mMeta->mFields.Size(); ++i)
//    {
//      Zero::ShaderIRFieldMeta* fieldMeta = fragmentType->mMeta->mFields[i];
//      if(fieldMeta->mZilchName == propertyName)
//      {
//        size_t size = Math::Max(fieldMeta->mZilchType->Size, 4u);
//        prop->Set(size);
//        break;
//      }
//    }
//  }
//
// 
//  return prop;
//}
//
//void SetDefaultProperties(GenericProperties& properties, Model* model, ZilchShaderIRType* fragmentType)
//{
//  Zero::ShaderIRTypeMeta* typeMeta = fragmentType->mMeta;
//  if(typeMeta == nullptr)
//    return;
//
//  ShaderProgram* shader = model->mShader;
//
//  for(size_t i = 0; i < typeMeta->mFields.Size(); ++i)
//  {
//    Zero::ShaderIRFieldMeta* fieldMeta = typeMeta->mFields[i];
//
//    String fullShaderPropertyName = shader->FindReflectionName(fragmentType, fieldMeta->mZilchName);
//    GenericProperty* metaProp = properties.FindOrCreate(fullShaderPropertyName);
//
//    Zilch::Type* zilchType = fieldMeta->mDefaultValueVariant.StoredType;
//    size_t size = zilchType->GetAllocatedSize();
//    const byte* data = fieldMeta->mDefaultValueVariant.GetData();
//
//    if(zilchType == ZilchTypeId(bool))
//    {
//      int intBool = *data != 0;
//      size = 4;
//      metaProp->Set((byte*)&intBool, size);
//    }
//    else
//      metaProp->Set(data, size);
//  }
//}
//
//
//
////-----------------------------------------------------------------------------Model
//Model::Model()
//{
//  mOverlap = 0;
//  mMidPhase = nullptr;
//  mMidphaseDrawLevel = 0;
//  mMesh = nullptr;
//  mShader = nullptr;
//}
//
//
//void Model::Initialize()
//{
//  Application* application = mOwner->mApplication;
//  ShaderManager* shaderManager = ShaderManager::GetInstance();
//
//  Zero::ZilchShaderIRLibrary* fragmentLibrary = shaderManager->mGenerator->mFragmentLibraryRef;
//  ZilchShaderIRType* forwardPass = fragmentLibrary->FindType("ForwardPass");
//  ZilchShaderIRType* albedoValue = fragmentLibrary->FindType("AlbedoValue");
//  ZilchShaderIRType* albedoMap = fragmentLibrary->FindType("AlbedoMap");
//  ZilchShaderIRType* metallicMap = fragmentLibrary->FindType("MetallicMap");
//  ZilchShaderIRType* specularValue = fragmentLibrary->FindType("SpecularValue");
//  ZilchShaderIRType* metallicValue = fragmentLibrary->FindType("MetallicValue");
//  ZilchShaderIRType* roughnessValue = fragmentLibrary->FindType("RoughnessValue");
//  ZilchShaderIRType* wireFrameGeometry = fragmentLibrary->FindType("WireFrameGeometry");
//  ZilchShaderIRType* wireFramePixel = fragmentLibrary->FindType("WireFramePixel");
//  ZilchShaderIRType* debugPixel = fragmentLibrary->FindType("DebugPixel");
//  ZilchShaderIRType* particlePixel = fragmentLibrary->FindType("ParticlePixel");
//  {
//    auto& fragments = mShader->mShaderDef.mFragments;
//    for(size_t i = 0; i < fragments.Size(); ++i)
//    {
//      ZilchShaderIRType* fragment = fragments[i];
//      SetDefaultProperties(mProperties, this, fragment);
//    }
//
//    // Override certain properties
//    SetProperty(mProperties, this, wireFramePixel, "Pixels", 3);
//    SetProperty(mProperties, this, roughnessValue, "RoughnessValue", 1.0f);
//    Array<Vector4> colors;
//    colors.PushBack(Vector4(1, 0, 0, 1));
//    colors.PushBack(Vector4(0, 1, 0, 1));
//    colors.PushBack(Vector4(0, 0, 1, 1));
//    colors.PushBack(Vector4(1, 1, 1, 1));
//    colors.PushBack(Vector4(0, 0, 0, 1));
//    SetProperty(mProperties, this, debugPixel, "Colors", colors);
//
//
//    int albedoMapId = shaderManager->mTextureIds["AlbedoMap"];
//    SetImageProperty(mProperties, this, albedoMap, "AlbedoMap", albedoMapId);
//
//    int metallicMapId = shaderManager->mTextureIds["MetallicMap"];
//    SetImageProperty(mProperties, this, metallicMap, "MetallicMap", metallicMapId);
//
//    int textureId = shaderManager->mTextureIds["AlbedoMap"];
//    SetImageProperty(mProperties, this, particlePixel, "Texture", textureId);
//  }
//
//
//  {
//    GenericProperties& properties = mMaterial.mProperties;
//    // Set default values for all fragments in our shader
//    auto& fragments = mShader->mShaderDef.mFragments;
//    for(size_t i = 0; i < fragments.Size(); ++i)
//    {
//      ZilchShaderIRType* fragment = fragments[i];
//      SetDefaultProperties(properties, this, fragment);
//    }
//
//    // Override certain properties
//    SetProperty(properties, this, wireFramePixel, "Pixels", 3);
//    SetProperty(properties, this, roughnessValue, "RoughnessValue", 1.0f);
//    Array<Vector4> colors;
//    colors.PushBack(Vector4(1, 0, 0, 1));
//    colors.PushBack(Vector4(0, 1, 0, 1));
//    colors.PushBack(Vector4(0, 0, 1, 1));
//    colors.PushBack(Vector4(1, 1, 1, 1));
//    colors.PushBack(Vector4(0, 0, 0, 1));
//    SetProperty(properties, this, debugPixel, "Colors", colors);
//
//
//    int albedoMapId = shaderManager->mTextureIds["AlbedoMap"];
//    SetImageProperty(properties, this, albedoMap, "AlbedoMap", albedoMapId);
//
//    int metallicMapId = shaderManager->mTextureIds["MetallicMap"];
//    SetImageProperty(properties, this, metallicMap, "MetallicMap", metallicMapId);
//
//    int textureId = shaderManager->mTextureIds["AlbedoMap"];
//    SetImageProperty(properties, this, particlePixel, "Texture", textureId);
//  }
//}
//
//void Model::TransformUpdate(TransformUpdateFlags::Enum flags)
//{
//  bool recompute = (flags & TransformUpdateFlags::Scale) != 0;
//  UpdateBoundingVolumes(recompute);
//}
//
//void Model::DebugDraw()
//{
//  if(mMidPhase)
//    mMidPhase->DebugDraw(mMidphaseDrawLevel, mOwner->has(Transform)->GetTransform());
//
//  //mBoundingSphere.DebugDraw();
//}
//
//void Model::DisplayProperties(TwBar* bar)
//{
//  std::string name = mOwner->mName;
//  std::string groupName = "group=" + name + ".Model";
//
//  TwAddVarRW(bar, (name + ".MidphaseDrawLevel").c_str(), TW_TYPE_INT32, &mMidphaseDrawLevel, (groupName + " label=MidphaseDrawLevel").c_str());
//  BindSimpleProperty(bar, name, Model, MeshType, mOwner->mApplication->mMeshTypesEnum, int);
//  BindSimpleProperty(bar, name, Model, ShaderType, mOwner->mApplication->mShaderTypesEnum, int);
//
//  ShaderManager* shaderManager = ShaderManager::GetInstance();
//  Zero::ZilchShaderIRLibrary* fragmentLibrary = shaderManager->mGenerator->mFragmentLibraryRef;
//  std::string objName = name;
//  for(size_t stageType = 0; stageType < Zero::FragmentType::Size; ++stageType)
//  {
//    ZilchShaderIRCompositor::ShaderStageDescription stageDesc = mShader->mShaderDef.mResults[stageType];
//    if(stageDesc.mShaderCode.Empty())
//      continue;
//    for(auto fragRange = stageDesc.mFragmentDescriptions->All(); !fragRange.Empty(); fragRange.PopFront())
//    {
//      
//      ZilchShaderIRCompositor::ShaderFragmentDescription* fragDesc = fragRange.Front().second;
//      std::string subGroup = groupName + "." + std::string(fragDesc->mMeta->mZilchName.c_str());
//      for(auto fieldRange = fragDesc->mFieldDescriptions.All(); !fieldRange.Empty(); fieldRange.PopFront())
//      {
//        ZilchShaderIRCompositor::ShaderFieldDescription& fieldDesc = fieldRange.Front().second;
//        if(fieldDesc.mLinkedType != Zero::LinkedFieldType::Property && fieldDesc.mLinkedType != Zero::LinkedFieldType::SpecConstant)
//          continue;
//
//        ZilchShaderIRType* fragmentType = fragmentLibrary->FindType(fragDesc->mMeta->mZilchName);
//        GenericProperty* prop = MakeOrCreateProperty(mProperties, this, fragmentType, fieldDesc.mMeta->mZilchName);
//        
//
//        //BindProperty(bar, name, fieldDesc->mMeta->mZilchName, TW_TYPE_INT32, Model, int, nullptr, nullptr);
//        std::string propertyName = fieldDesc.mMeta->mZilchName.c_str();
//        
//        std::string classType = "Model";
//        std::string labelName = "label=" + propertyName;
//        
//       
//        int twType = TW_TYPE_INT32;
//        String zilchType = fieldDesc.mMeta->mZilchType->ToString();
//        if(zilchType == "Boolean")
//          twType = TW_TYPE_BOOL32;
//        else if(zilchType == "Real")
//          twType = TW_TYPE_FLOAT;
//        //else if(zilchType == "Real2")
//        //  twType = TW_TYPE_DIR2F;
//        else  if(zilchType == "Real3")
//          twType = TW_TYPE_DIR3F;
//        else if(zilchType == "Real4")
//          twType = TW_TYPE_COLOR4F;
//        
//        //BindFullProperty(bar, (objName + "." + propertyName), TW_TYPE_INT32, classType, int, nullptr, nullptr, "group=" + objName + "." + classType, "label=" + propertyName);
//        TwAddVarRW(bar, propertyName.c_str(), (TwType)twType, prop->mData, (subGroup + " " + labelName).c_str());
//      }
//
//      std::string fName(fragDesc->mMeta->mZilchName.c_str());
//      std::string gName = "Model" + std::string(".") + fName;
//      //DeclareObjectGroup(bar, objName, gName, "");
//      TwDefine((TwGetBarName(bar) + std::string("/") + objName + "." + gName + " group=" + objName + " label=" + gName + " " + "").c_str());
//      //DeclareObjectMaterialGroup(bar, name, Model, fName);
//      //TwDefine((TwGetBarName(bar) + std::string("/") + objName + "." + subGroup + " group=" + objName + " label=" + groupName).c_str());
//    }
//  }
//  
//  BindSimpleProperty(bar, name, Model, ShaderType, mOwner->mApplication->mShaderTypesEnum, int);
//
//  DeclareObjectComponentGroup(bar, name, Model);
//}
//
//void Model::UpdateBoundingVolumes(bool recompute)
//{
//  if(recompute)
//  {
//    ComputeBoundingSphere();
//    ComputeAabb();
//  }
//
//  UpdateAabb();
//  UpdateBoundingSphere();
//
//  mOwner->mApplication->UpdateGameObject(mOwner);
//}
//
//void Model::ComputeAabb()
//{
//  mLocalAabb = Aabb();
//  for(size_t i = 0; i < mMesh->mVertices.size(); ++i)
//    mLocalAabb.Expand(mMesh->mVertices[i].mPosition);
//}
//
//void Model::ComputeBoundingSphere()
//{
//  std::vector<Vector3> vertices = mMesh->GetVertexPositions();
//  int boundingSphereType = mOwner->mApplication->mBoundSphereType;
//  if(boundingSphereType == BoundingSphereType::Centroid)
//    mLocalSphere.ComputeCentroid(vertices);
//  else if(boundingSphereType == BoundingSphereType::RitterSphere)
//    mLocalSphere.ComputeRitter(vertices);
//  else if(boundingSphereType == BoundingSphereType::PCA)
//    mLocalSphere.ComputePCA(vertices);
//}
//
//void Model::UpdateAabb()
//{
//  mAabb = mLocalAabb;
//
//  Transform* transform = mOwner->has(Transform);
//  mAabb.Transform(transform->GetTransform());
//}
//
//void Model::UpdateBoundingSphere()
//{
//  Vector3 scale = mOwner->has(Transform)->mScale;
//  float maxScale = Math::Max(Math::Max(scale.x, scale.y), scale.z);
//  mBoundingSphere.mCenter = mOwner->has(Transform)->mTranslation + mLocalSphere.mCenter * maxScale;
//
//  mBoundingSphere.mRadius = maxScale * mLocalSphere.mRadius;
//}
//
//void Model::CheckTriangle(const Ray& ray, const Triangle& tri, float& minT)
//{
//  float t;
//  if(RayTriangle(ray.mStart, ray.mDirection, tri.mPoints[0], tri.mPoints[1], tri.mPoints[2], t, 0) == false)
//    return;
//
//  if(t < minT)
//    minT = t;
//}
//
//bool Model::CastRayMidphase(const Ray& localRay, CastResult& castInfo)
//{
//  CastResults results;
//  mMidPhase->CastRay(localRay, results);
//
//  float minT = Math::PositiveMax();
//  for(size_t i = 0; i < results.mResults.size(); ++i)
//  {
//    CastResult& result = results.mResults[i];
//    size_t triangleIndex = (size_t)result.mClientData;
//
//    Triangle tri = mMesh->TriangleAt(triangleIndex);
//    CheckTriangle(localRay, tri, minT);
//  }
//
//  if(minT >= Math::PositiveMax())
//    return false;
//
//  castInfo.mTime = minT;
//  return true;
//}
//
//bool Model::CastRay(const Ray& worldRay, CastResult& castInfo)
//{
//  Math::Matrix4 toWorldMat = mOwner->has(Transform)->GetTransform();
//  Math::Matrix4 toLocalMat = toWorldMat.Inverted();
//
//  Ray localRay = worldRay.Transform(toLocalMat);
//
//  if(mMidPhase != nullptr)
//    return CastRayMidphase(worldRay, castInfo);
//
//  float minT = Math::PositiveMax();
//  for(size_t i = 0; i < mMesh->TriangleCount(); ++i)
//  {
//    Triangle tri = mMesh->TriangleAt(i);
//    CheckTriangle(localRay, tri, minT);
//  }
//
//  if(minT >= Math::PositiveMax())
//    return false;
//
//  castInfo.mTime = minT;
//  return true;
//}
//
//void Model::SetMidPhase(SpatialPartition* midPhase)
//{
//  if(mMidPhase != nullptr)
//    delete mMidPhase;
//
//  mMidPhase = midPhase;
//
//  SpatialPartitionKey dummyKey;
//  for(size_t i = 0; i < mMesh->TriangleCount(); ++i)
//  {
//    Triangle tri = mMesh->TriangleAt(i);
//    Aabb aabb;
//    aabb.Expand(tri.mPoints[0]);
//    aabb.Expand(tri.mPoints[1]);
//    aabb.Expand(tri.mPoints[2]);
//
//    SpatialPartitionData data;
//    //fix this later!
//    //data.mBoundingSphere = Sphere(aabb;
//    data.mAabb = aabb;
//    data.mClientData = (void*)i;
//
//    mMidPhase->InsertData(dummyKey, data);
//  }
//}
//
//int Model::GetMeshType()
//{
//  return mMesh->mType;
//}
//
//void Model::SetMeshType(const int& meshIndex)
//{
//  mMesh = mOwner->mApplication->mMeshes[meshIndex];
//  UpdateBoundingVolumes(true);
//}
//
//int Model::GetShaderType()
//{
//  return mShader->mTypeId;
//}
//
//void Model::SetShaderType(const int& shaderIndex)
//{
//  Application* application = mOwner->mApplication;
//  ShaderManager* shaderManager = ShaderManager::GetInstance();
//  mShader = shaderManager->mShaderList[shaderIndex];
//  mMaterial.mShader = mShader;
//  UpdateBoundingVolumes(true);
//}
//
//void Model::Draw()
//{
//  int programId = mShader->mShaderProgramId;
//  Application* application = mOwner->mApplication;
//  application->mCamera.SetMatrix(programId);
//
//  Camera& camera = mOwner->mApplication->mCamera;
//  Transform* transform = mOwner->has(Transform);
//  Vector3 axis;
//  float radians;
//  Math::ToAxisAngle(transform->mRotation, &axis, &radians);
//
//  ShaderManager* shaderManager = ShaderManager::GetInstance();
//  Zero::ZilchShaderIRLibrary* fragmentLibrary = shaderManager->mGenerator->mFragmentLibraryRef;
//  
//  ZilchShaderIRType* vertexShaderType = mShader->GetShaderType(Zero::FragmentType::Vertex);
//  ZilchShaderIRType* pixelShaderType = mShader->GetShaderType(Zero::FragmentType::Pixel);
//  
//  Matrix4 localToWorld = transform->GetTransform();
//  Matrix4 worldToView = camera.mWorldToView.Transposed();
//  Matrix4 viewToPerspective = camera.mViewToPerspective;
//  Matrix4 localToView = Math::Multiply(worldToView, localToWorld);
//  Matrix4 localToPerspective = Math::Multiply(viewToPerspective, localToView);
//
//  {
//    GenericProperties& properties = mMaterial.mProperties;
//
//    properties.Set("LocalToWorld", localToWorld);
//    properties.Set("WorldToView", worldToView);
//    properties.Set("LocalToView", localToView);
//    properties.Set("LocalToViewNormal", Matrix3x4(localToView));
//    properties.Set("ViewToPerspective", viewToPerspective);
//    properties.Set("PerspectiveToApiPerspective", Matrix4::cIdentity);
//    properties.Set("LocalToPerspective", localToPerspective);
//    properties.Set("ViewportSize", application->mSize);
//    ZilchShaderIRType* forwardPass = fragmentLibrary->FindType("ForwardPass");
//    SetProperty(properties, this, forwardPass, "DirectLightColor", application->mLightDiffuse);
//    SetProperty(properties, this, forwardPass, "DirectLightDirection", application->mLightDirection);
//  }
//  mMaterial.RunRenderPass(application, mMesh);
//}
//
//void Model::DrawTriangle(Triangle& tri)
//{
//  Math::Vector3 normal = Math::Cross(tri.mPoints[1] - tri.mPoints[0], tri.mPoints[2] - tri.mPoints[0]);
//  normal.AttemptNormalize();
//
//  glNormal3f(normal.x, normal.y, normal.z);
//  for(int i = 0; i < 3; ++i)
//    glVertex3f(tri.mPoints[i].x, tri.mPoints[i].y, tri.mPoints[i].z);
//}
//
//std::vector<Triangle> Model::GetWorldTriangles()
//{
//  std::vector<Triangle> results;
//  Math::Matrix4 worldMat = mOwner->has(Transform)->GetTransform();
//  for(size_t i = 0; i < mMesh->mIndices.size(); i += 3)
//  {
//    Vector3 p0 = Math::TransformPoint(worldMat, mMesh->mVertices[mMesh->mIndices[i + 0]].mPosition);
//    Vector3 p1 = Math::TransformPoint(worldMat, mMesh->mVertices[mMesh->mIndices[i + 1]].mPosition);
//    Vector3 p2 = Math::TransformPoint(worldMat, mMesh->mVertices[mMesh->mIndices[i + 2]].mPosition);
//    results.push_back(Triangle(p0, p1, p2));
//  }
//  
//  return results;
//}
//
//void Model::SetWorldTriangles(const std::vector<Triangle>& tris)
//{
//  //if(mMesh->mDynamic == true)
//  //  delete mMesh;
//  //
//  //mMesh = new Mesh();
//  //mMesh->mDynamic = true;
//  //mMesh->mType = -1;
//  //
//  //Math::Matrix4 localMat = mOwner->has(Transform)->GetTransform().Inverted();
//  //
//  //mMesh->mIndices.clear();
//  //mMesh->mVertices.clear();
//  //mMesh->mIndices.reserve(tris.size() * 3);
//  //mMesh->mVertices.reserve(tris.size() * 3);
//  //
//  //for(size_t i = 0; i < tris.size(); ++i)
//  //{
//  //  mMesh->mIndices.push_back(i * 3 + 0);
//  //  mMesh->mIndices.push_back(i * 3 + 1);
//  //  mMesh->mIndices.push_back(i * 3 + 2);
//  //
//  //  const Triangle& tri = tris[i];
//  //  Vector3 p0 = Math::TransformPoint(localMat, tri.mPoints[0]);
//  //  Vector3 p1 = Math::TransformPoint(localMat, tri.mPoints[1]);
//  //  Vector3 p2 = Math::TransformPoint(localMat, tri.mPoints[2]);
//  //
//  //  mMesh->mVertices.push_back(p0);
//  //  mMesh->mVertices.push_back(p1);
//  //  mMesh->mVertices.push_back(p2);
//  //}
//}
