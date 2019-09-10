///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Serializer.hpp"

//-------------------------------------------------------------------JsonSerializer
class JsonSerializer : public Serializer
{
public:
  using JsonValue = Zilch::JsonValue;

  JsonSerializer();
  ~JsonSerializer();

  virtual bool Load(const String& filePath) override;
  virtual bool BeginObject(const String& name) override;
  virtual size_t BeginArray(const String& name) override;
  virtual bool BeginArrayItem(size_t index) override;
  virtual bool BeginField(const String& name) override;
  virtual void End() override;

  virtual void SerializeValue(String& value) override;
  virtual void SerializeValue(bool& value) override;
  virtual void SerializeValue(int& value) override;
  virtual void SerializeValue(float& value) override;
  virtual void SerializeValue(Vector2& value) override;
  virtual void SerializeValue(Vector3& value) override;
  virtual void SerializeValue(Vector4& value) override;

  void Read(bool& value);
  void Read(int& value);
  void Read(float& value);
  void Read(String& value);

  JsonValue* GetCurrent();

  JsonValue* mRoot;
  Array<JsonValue*> mStack;
};