///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"

#include "JsonSerializer.hpp"

namespace Engine
{

//-------------------------------------------------------------------JsonSerializer
JsonSerializer::JsonSerializer()
{
  mRoot = nullptr;
}

JsonSerializer::~JsonSerializer()
{
  delete mRoot;
}

bool JsonSerializer::Load(const String& filePath)
{
  Zilch::JsonReader jsonReader;
  Zilch::CompilationErrors errors;
  mRoot = jsonReader.ReadIntoTreeFromFile(errors, filePath, nullptr);
  mStack.PushBack(mRoot);
  return mRoot != nullptr;
}

bool JsonSerializer::BeginObject(const String& name)
{
  JsonValue* currNode = GetCurrent();
  JsonValue* objNode = currNode->GetMember(name, Zilch::JsonErrorMode::DefaultValue);
  if(objNode == nullptr || objNode->Type != Zilch::JsonValueType::Object)
    return false;
  mStack.PushBack(objNode);
  return true;
}

size_t JsonSerializer::BeginArray(const String& name)
{
  JsonValue* currNode = GetCurrent();
  JsonValue* arrayNode = currNode->GetMember(name, Zilch::JsonErrorMode::DefaultValue);
  if(arrayNode == nullptr || arrayNode->Type != Zilch::JsonValueType::Array)
    return 0;

  mStack.PushBack(arrayNode);
  return arrayNode->ArrayElements.Size();
}

bool JsonSerializer::BeginArrayItem(size_t index)
{
  JsonValue* arrayNode = GetCurrent();
  JsonValue* itemNode = arrayNode->ArrayElements[index];
  if(itemNode != nullptr)
    mStack.PushBack(itemNode);
  return itemNode != nullptr;
}

bool JsonSerializer::BeginField(const String& name)
{
  JsonValue* currNode = GetCurrent();
  JsonValue* objNode = currNode->GetMember(name, Zilch::JsonErrorMode::DefaultValue);
  if(objNode == nullptr)
    return false;
  mStack.PushBack(objNode);
  return true;
}

void JsonSerializer::End()
{
  mStack.PopBack();
}

void JsonSerializer::SerializeValue(String& value)
{
  Read(value);
}

void JsonSerializer::SerializeValue(bool& value)
{
  Read(value);
}

void JsonSerializer::SerializeValue(int& value)
{
  Read(value);
}

void JsonSerializer::SerializeValue(size_t& value)
{
  int intVal;
  Read(intVal);
  value = intVal;
}

void JsonSerializer::SerializeValue(float& value)
{
  Read(value);
}

void JsonSerializer::SerializeValue(Vector2& value)
{
  SerializeStaticArrayValue<Vector2, 2>(value);
}

void JsonSerializer::SerializeValue(Vector3& value)
{
  SerializeStaticArrayValue<Vector3, 3>(value);
}

void JsonSerializer::SerializeValue(Vector4& value)
{
  SerializeStaticArrayValue<Vector4, 4>(value);
}

void JsonSerializer::SerializeValue(Quaternion& value)
{
  SerializeStaticArrayValue<Quaternion, 4>(value);
}

void JsonSerializer::SerializeValue(Integer2& value)
{
  SerializeStaticArrayValue<Integer2, 2>(value);
}

void JsonSerializer::SerializeValue(Integer3& value)
{
  SerializeStaticArrayValue<Integer3, 3>(value);
}

void JsonSerializer::SerializeValue(Integer4& value)
{
  SerializeStaticArrayValue<Integer4, 4>(value);
}

JsonSerializer::JsonValue* JsonSerializer::GetCurrent()
{
  return mStack.Back();
}

void JsonSerializer::Read(bool& value)
{
  JsonValue* valueNode = GetCurrent();
  if(valueNode->Type == Zilch::JsonValueType::Integer)
    value = valueNode->AsInteger() == 0 ? false : true;
  else if(valueNode->Type == Zilch::JsonValueType::Real)
    value = valueNode->AsFloat() == 0 ? false : true;
  else if(valueNode->Type == Zilch::JsonValueType::String)
    value = valueNode->AsString() == "false" ? false : true;
  else
    value = valueNode->AsBool();
}

void JsonSerializer::Read(int& value)
{
  JsonValue* valueNode = GetCurrent();
  if(valueNode->Type == Zilch::JsonValueType::False)
    value = 0;
  else if(valueNode->Type == Zilch::JsonValueType::True)
    value = 1;
  else if(valueNode->Type == Zilch::JsonValueType::Integer)
    value = valueNode->AsInteger();
  else if(valueNode->Type == Zilch::JsonValueType::Real)
    value = (int)valueNode->AsFloat();
  else
    value = 0;
}

void JsonSerializer::Read(float& value)
{
  JsonValue* valueNode = GetCurrent();
  if(valueNode->Type == Zilch::JsonValueType::False)
    value = 0.0f;
  else if(valueNode->Type == Zilch::JsonValueType::True)
    value = 1.0f;
  else if(valueNode->Type == Zilch::JsonValueType::Integer)
    value = (float)valueNode->AsInteger();
  else if(valueNode->Type == Zilch::JsonValueType::Real)
    value = valueNode->AsFloat();
  else
    value = 0;
}

void JsonSerializer::Read(String& value)
{
  JsonValue* valueNode = GetCurrent();
  if(valueNode->Type == Zilch::JsonValueType::False)
    value = "false";
  else if(valueNode->Type == Zilch::JsonValueType::True)
    value = "true";
  else if(valueNode->Type == Zilch::JsonValueType::Integer)
    value = Zero::ToString(valueNode->AsInteger());
  else if(valueNode->Type == Zilch::JsonValueType::Real)
    value = Zero::ToString(valueNode->AsFloat());
  else if(valueNode->Type == Zilch::JsonValueType::String)
    value = valueNode->AsString();
}

} // namespace Engine
