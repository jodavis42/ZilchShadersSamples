///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "EngineStandard.hpp"
#include "ResourceSystem.hpp"
#include "Enums.hpp"

namespace Engine
{

class ResourceSystem;

//-------------------------------------------------------------------StreamDirection
struct StreamDirection
{
  enum Enum
  {
    None,
    Loading,
    Saving,
  };

  DeclareEnumType(StreamDirection);

  static Enum FromString(const String& enumName);
  static String ToString(Enum enumValue);
};

#define SerializeField(FieldName) SerializeNamedField(#FieldName, FieldName)

#define DeclareSerializeFieldType(Type)                     \
  void SerializeNamedField(const String& name, Type& value) \
  {                                                         \
    String cleanName = CleanupName(name);                   \
    if(BeginField(cleanName))                               \
    {                                                       \
      SerializeValue(value);                                \
      End();                                                \
    }                                                       \
  }

//-------------------------------------------------------------------Serializer
/// Base serializer to handle saving/loading data
class Serializer
{
public:
  Serializer();
  ~Serializer() {};

  String CleanupName(const String& name);

  virtual bool Load(const String& filePath) abstract;

  virtual bool BeginObject(const String& name) abstract;
  virtual size_t BeginArray(const String& name) abstract;
  virtual bool BeginArrayItem(size_t index) abstract;
  virtual bool BeginField(const String& name) abstract;
  virtual void End() abstract;

  DeclareSerializeFieldType(bool);
  DeclareSerializeFieldType(int);
  DeclareSerializeFieldType(size_t);
  DeclareSerializeFieldType(float);
  DeclareSerializeFieldType(Vector2);
  DeclareSerializeFieldType(Vector3);
  DeclareSerializeFieldType(Vector4);
  DeclareSerializeFieldType(Quaternion);
  DeclareSerializeFieldType(Integer2);
  DeclareSerializeFieldType(Integer3);
  DeclareSerializeFieldType(Integer4);
  DeclareSerializeFieldType(String);

  template <typename ArrayType, size_t ElementCount>
  void SerializeStaticArrayValue(ArrayType& value)
  {
    for(size_t i = 0; i < ElementCount; ++i)
    {
      BeginArrayItem(i);
      SerializeValue(value[i]);
      End();
    }
  }
  
  template <typename EnumType>
  void SerializeEnumField(const String& name, EnumType& value)
  {
    if(BeginField(name))
    {
      SerializeEnumValue(value);
      End();
    }
  }

  template <typename ResourceLibraryType, typename ResourceType = ResourceLibraryType::ResourceType>
  void SerializeResourceField(const String& name, ResourceType*& resource)
  {
    if(BeginField(name))
    {
      SerializeResourceValue<ResourceLibraryType>(resource);
      End();
    }
  }

  virtual void SerializeValue(bool& value) abstract;
  virtual void SerializeValue(int& value) abstract;
  virtual void SerializeValue(size_t& value) abstract;
  virtual void SerializeValue(float& value) abstract;
  virtual void SerializeValue(Vector2& value) abstract;
  virtual void SerializeValue(Vector3& value) abstract;
  virtual void SerializeValue(Vector4& value) abstract;
  virtual void SerializeValue(Quaternion& value) abstract;
  virtual void SerializeValue(Integer2& value) abstract;
  virtual void SerializeValue(Integer3& value) abstract;
  virtual void SerializeValue(Integer4& value) abstract;
  virtual void SerializeValue(String& value) abstract;

  template <typename EnumType>
  void SerializeEnumValue(EnumType& value)
  {
    String enumName;
    SerializeValue(enumName);
    value = EnumType::FromString(enumName);
  }

  template <typename ResourceLibraryType, typename ResourceType = ResourceLibraryType::ResourceType>
  void SerializeResourceValue(ResourceType*& resource)
  {
    if(mResourceSystem == nullptr)
      return;

    String resourceLibraryName = ResourceLibraryType::StaticGetName();
    ResourceLibraryType* resourceLibrary = mResourceSystem->FindTypedResourceLibrary<ResourceLibraryType>(resourceLibraryName);
    if(resourceLibrary == nullptr)
      return;

    String resourceName;
    SerializeValue(resourceName);
    resource = resourceLibrary->Find(resourceName);
  }

  ResourceSystem* mResourceSystem;
  StreamDirection::Enum mDirection;
};

}//namespace Engine
