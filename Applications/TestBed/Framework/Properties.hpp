//#pragma once
//
//// Using directives
//using Math::Vector3;
//using Math::Vector4;
//
//class Application;
//class Mesh;
//class Model;
//class ShaderProgram;
//struct ShaderStageResource;
//
//struct Matrix3x4
//{
//  Matrix3x4() {}
//  Matrix3x4(const Matrix3& mat3)
//  {
//    mData[0] = Vector4(mat3.m00, mat3.m01, mat3.m02, 1);
//    mData[1] = Vector4(mat3.m10, mat3.m11, mat3.m12, 1);
//    mData[2] = Vector4(mat3.m20, mat3.m21, mat3.m22, 1);
//  }
//  Matrix3x4(const Matrix4& mat4)
//  {
//    mData[0] = Vector4(mat4.m00, mat4.m01, mat4.m02, 1);
//    mData[1] = Vector4(mat4.m10, mat4.m11, mat4.m12, 1);
//    mData[2] = Vector4(mat4.m20, mat4.m21, mat4.m22, 1);
//  }
//  Vector4 mData[3];
//};
//
//struct ObjectTransform_Uniforms
//{
//  Matrix4 LocalToWorld;
//  Matrix4 WorldToLocal;
//  Matrix4 WorldToView;
//  Matrix4 ViewToWorld;
//  Matrix4 LocalToView;
//  Matrix4 ViewToLocal;
//  Matrix3x4 LocalToViewNormal;
//  Matrix3x4 ViewToLocalNormal;
//  Matrix3x4 LocalToWorldNormal;
//  Matrix3x4 WorldToLocalNormal;
//  Matrix4 LocalToPerspective;
//  Matrix4 ViewToPerspective;
//  Matrix4 PerspectiveToView;
//  Matrix4 PerspectiveToApiPerspective;
//};
//
//class UniformByteBuffer
//{
//public:
//  UniformByteBuffer();
//  UniformByteBuffer(const UniformByteBuffer& rhs);
//  ~UniformByteBuffer();
//  UniformByteBuffer& operator=(const UniformByteBuffer& rhs);
//
//
//  void Set(byte* data, size_t size);
//  void CopyFrom(const UniformByteBuffer& rhs);
//  void Clear();
//
//  template <typename T>
//  void Set(T& data)
//  {
//    Set((byte*)&data, sizeof(T));
//  }
//
//  byte* mData;
//  size_t mSize;
//};
//
//class UniformBufferData
//{
//public:
//  String mBufferName;
//  UniformByteBuffer mBuffer;
//};
//
//class GenericProperty
//{
//public:
//  GenericProperty();
//  GenericProperty(const GenericProperty& rhs);
//  ~GenericProperty();
//  GenericProperty& operator=(const GenericProperty& rhs);
//
//
//  void Set(const byte* data, size_t size);
//  void Set(size_t size);
//  void CopyFrom(const GenericProperty& rhs);
//  void Clear();
//
//  template <typename T>
//  T* Get()
//  {
//    return (T*)mData;
//  }
//  template <typename T>
//  void SetData(T& data)
//  {
//    Set((byte*)&data, sizeof(T));
//  }
//
//  byte* mData;
//  size_t mSize;
//
//};
//
//class GenericProperties
//{
//public:
//  ~GenericProperties()
//  {
//    for(auto range = mProperties.Values(); !range.Empty(); range.PopFront())
//      delete range.Front();
//  }
//
//  template<typename T>
//  T* Get(StringParam propName)
//  {
//    GenericProperty* prop = mProperties.FindValue(propName, nullptr);
//    if(prop == nullptr)
//      return nullptr;
//
//    return prop->Get<T>();
//  }
//  template<typename T>
//  void Set(StringParam propName, const T& value)
//  {
//    GenericProperty* prop = FindOrCreate(propName);
//    prop->SetData(value);
//  }
//  GenericProperty* FindOrCreate(StringParam propName)
//  {
//    GenericProperty* prop = mProperties.FindValue(propName, nullptr);
//    if(prop == nullptr)
//    {
//      prop = new GenericProperty();
//      mProperties[propName] = prop;
//    }
//    return prop;
//  }
//
//  HashMap<String, GenericProperty*> mProperties;
//};
//
//template <typename T>
//void SetProperty(GenericProperties& properties, Model* model, ZilchShaderIRType* fragmentType, StringParam propertyName, const T& propertyValue)
//{
//  ShaderProgram* shader = model->mShader;
//  String fullShaderPropertyName = shader->FindReflectionName(fragmentType, propertyName);
//
//  Zero::ShaderIRFieldMeta* fieldMeta = fragmentType->mMeta->FindField(propertyName);
//  properties.Set(fullShaderPropertyName, propertyValue);
//}
//
//template <typename T>
//void SetProperty(GenericProperties& properties, Model* model, ZilchShaderIRType* fragmentType, StringParam propertyName, const Array<T>& propertyValues)
//{
//  ShaderProgram* shader = model->mShader;
//  String fullShaderPropertyName = shader->FindReflectionName(fragmentType, propertyName);
//
//  Zero::ShaderIRFieldMeta* fieldMeta = fragmentType->mMeta->FindField(propertyName);
//  GenericProperty* prop = properties.FindOrCreate(fullShaderPropertyName);
//  size_t byteSize = propertyValues.Size() * sizeof(T);
//  prop->Set((byte*)propertyValues.Data(), byteSize);
//}
//
//template <typename T>
//void SetProperty(GenericProperties& properties, ShaderProgram* shader, ZilchShaderIRType* fragmentType, StringParam propertyName, const T& propertyValue)
//{
//  String fullShaderPropertyName = shader->FindReflectionName(fragmentType, propertyName);
//
//  Zero::ShaderIRFieldMeta* fieldMeta = fragmentType->mMeta->FindField(propertyName);
//  properties.Set(fullShaderPropertyName, propertyValue);
//}
//
//template <typename T>
//void SetProperty(GenericProperties& properties, ShaderProgram* shader, ZilchShaderIRType* fragmentType, StringParam propertyName, const Array<T>& propertyValues)
//{
//  String fullShaderPropertyName = shader->FindReflectionName(fragmentType, propertyName);
//
//  Zero::ShaderIRFieldMeta* fieldMeta = fragmentType->mMeta->FindField(propertyName);
//  GenericProperty* prop = properties.FindOrCreate(fullShaderPropertyName);
//  size_t byteSize = propertyValues.Size() * sizeof(T);
//  prop->Set((byte*)propertyValues.Data(), byteSize);
//}
//
//template <typename T>
//void SetImageProperty(GenericProperties& properties, Model* model, ZilchShaderIRType* fragmentType, StringParam propertyName, const T& propertyValue)
//{
//  ShaderProgram* shader = model->mShader;
//  Array<String> names;
//  shader->FindImageReflectionNames(fragmentType, propertyName, names);
//  for(size_t i = 0; i < names.Size(); ++i)
//  {
//    String name = names[i];
//    properties.Set(name, propertyValue);
//  }
//}
//
//template <typename T>
//void SetImageProperty(GenericProperties& properties, ShaderProgram* shader, ZilchShaderIRType* fragmentType, StringParam propertyName, const T& propertyValue)
//{
//  Array<String> names;
//  shader->FindImageReflectionNames(fragmentType, propertyName, names);
//  for(size_t i = 0; i < names.Size(); ++i)
//  {
//    String name = names[i];
//    properties.Set(name, propertyValue);
//  }
//}
//
//template <typename T>
//void SetStorageImageProperty(GenericProperties& properties, ShaderProgram* shader, ZilchShaderIRType* fragmentType, StringParam propertyName, const T& propertyValue)
//{
//  //Array<String> names;
//  //shader->FindStorageImageReflectionName(fragmentType, propertyName);
//  //for(size_t i = 0; i < names.Size(); ++i)
//  //{
//  //  String name = names[i];
//  //  properties.Set(name, propertyValue);
//  //}
//  //
//
//  String fullShaderPropertyName = shader->FindStorageImageReflectionName(fragmentType, propertyName);
//
//  Zero::ShaderIRFieldMeta* fieldMeta = fragmentType->mMeta->FindField(propertyName);
//  properties.Set(fullShaderPropertyName, propertyValue);
//}
//
//template <typename T>
//void SetSsboProperty(GenericProperties& properties, ShaderProgram* shader, ZilchShaderIRType* fragmentType, StringParam propertyName, const T& propertyValue)
//{
//  //Array<String> names;
//  //shader->FindSsboReflectionName(fragmentType, propertyName);
//  //for(size_t i = 0; i < names.Size(); ++i)
//  //{
//  //  String name = names[i];
//  //  properties.Set(name, propertyValue);
//  //}
//  if(shader == nullptr)
//    return;
//
//  String fullShaderPropertyName = shader->FindSsboReflectionName(fragmentType, propertyName);
//
//  Zero::ShaderIRFieldMeta* fieldMeta = fragmentType->mMeta->FindField(propertyName);
//  properties.Set(fullShaderPropertyName, propertyValue);
//}
