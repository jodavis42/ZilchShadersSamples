///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"

#include "Composition.hpp"

namespace Engine
{

//-------------------------------------------------------------------Composition
Composition::Composition()
{

}

Composition::~Composition()
{
  DestroyComponents();
}

void Composition::Initialize(const CompositionInitializer& initializer)
{
  for(auto range = mComponents.All(); !range.Empty(); range.PopFront())
    range.Front()->Initialize(initializer);
}

void Composition::Update(float dt)
{
  for(auto range = mComponents.All(); !range.Empty(); range.PopFront())
    range.Front()->Update(dt);
}

void Composition::AddComponent(const String& componentName, Component* component)
{
  mComponentMap[componentName] = component;
  mComponents.PushBack(component);
}

Component* Composition::FindComponent(const String& componentName)
{
  return mComponentMap.FindValue(componentName, nullptr);
}

void Composition::DestroyComponents()
{
  for(size_t i = 0; i < mComponents.Size(); ++i)
    delete mComponents[i];
  mComponents.Clear();
  mComponentMap.Clear();
}

}//namespace Engine
