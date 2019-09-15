///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "EngineStandard.hpp"

#include "Component.hpp"

namespace Engine
{

//-------------------------------------------------------------------Composition
class Composition
{
public:
  Composition();
  virtual ~Composition();

  virtual void Initialize(const CompositionInitializer& initializer);
  virtual void Update(float dt);

  virtual void AddComponent(const String& componentName, Component* component);
  Component* FindComponent(const String& componentName);
  void DestroyComponents();

  template <typename ComponentType>
  ComponentType* HasComponentType(const String& componentName)
  {
    return (ComponentType*)FindComponent(componentName);
  }

  HashMap<String, Component*> mComponentMap;
  Array<Component*> mComponents;
  String mName;
};

#define Has(ComponentType) HasComponentType<ComponentType>(#ComponentType)

}//namespace Engine
