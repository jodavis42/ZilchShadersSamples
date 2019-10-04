///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "EngineStandard.hpp"

#include "Serializer.hpp"

namespace Engine
{

//-------------------------------------------------------------------JsonSerializer
class JsonSerializer : public Engine::Serializer
{
public:
  using JsonValue = Zilch::JsonValue;

  JsonSerializer();
  ~JsonSerializer();

  bool Load(const String& filePath) override;
  bool BeginObject(const String& name) override;
  size_t BeginArray(const String& name) override;
  bool BeginArrayItem(size_t index) override;
  bool BeginField(const String& name) override;
  void End() override;

  void SerializeValue(bool& value) override;
  void SerializeValue(int& value) override;
  void SerializeValue(size_t& value) override;
  void SerializeValue(float& value) override;
  void SerializeValue(Vector2& value) override;
  void SerializeValue(Vector3& value) override;
  void SerializeValue(Vector4& value) override;
  void SerializeValue(Quaternion& value) override;
  void SerializeValue(Integer2& value) override;
  void SerializeValue(Integer3& value) override;
  void SerializeValue(Integer4& value) override;
  void SerializeValue(String& value) override;

  void Read(bool& value);
  void Read(int& value);
  void Read(float& value);
  void Read(String& value);

  JsonValue* GetCurrent();

  JsonValue* mRoot;
  Array<JsonValue*> mStack;
};

} // namespace Engine
