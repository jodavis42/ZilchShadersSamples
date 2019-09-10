///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "FrameworkStandard.hpp"

using Math::IntVec3;
using Math::Quaternion;

//-------------------------------------------------------------------Serializer
class Serializer
{
public:
  ~Serializer() {};

  virtual bool Load(const String& filePath) abstract;

  virtual bool BeginObject(const String& name) abstract;
  virtual size_t BeginArray(const String& name) abstract;
  virtual bool BeginArrayItem(size_t index) abstract;
  virtual bool BeginField(const String& name) abstract;
  virtual void End() abstract;

  void SerializeField(const String& name, String& value) { if(BeginField(name)) { SerializeValue(value); End(); } }
  void SerializeField(const String& name, bool& value) { if(BeginField(name)) { SerializeValue(value); End(); } }
  void SerializeField(const String& name, int& value) { if(BeginField(name)) { SerializeValue(value); End(); } }
  void SerializeField(const String& name, float& value) { if(BeginField(name)) { SerializeValue(value); End(); } }
  void SerializeField(const String& name, Vector2& value) { if(BeginArray(name)) { SerializeValue(value); End(); } }
  void SerializeField(const String& name, Vector3& value) { if(BeginArray(name)) { SerializeValue(value); End(); } }
  void SerializeField(const String& name, Vector4& value) { if(BeginArray(name)) { SerializeValue(value); End(); } }
  void SerializeField(const String& name, Quaternion& value) { if(BeginArray(name)) { SerializeValue(value); End(); } }
  void SerializeField(const String& name, IntVec3& value) { if(BeginArray(name)) { SerializeValue(value); End(); } }

  template <typename EnumType, typename EnumValueType = EnumType::Enum>
  void SerializeEnumField(const String& name, EnumValueType& value) { if(BeginField(name)) { SerializeEnumValue<EnumType, EnumValueType>(value); End(); } }

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

  virtual void SerializeValue(String& value) abstract;
  virtual void SerializeValue(bool& value) abstract;
  virtual void SerializeValue(int& value) abstract;
  virtual void SerializeValue(float& value) abstract;
  virtual void SerializeValue(Vector2& value) abstract;
  virtual void SerializeValue(Vector3& value) abstract;
  virtual void SerializeValue(Vector4& value) abstract;
  virtual void SerializeValue(Quaternion& value) abstract;
  virtual void SerializeValue(IntVec3& value) abstract;

  template <typename EnumType, typename EnumValueType = EnumType::Enum>
  void SerializeEnumValue(EnumValueType& value)
  {
    String enumName;
    SerializeValue(enumName);
    value = EnumType::FromString(enumName);
  }
};