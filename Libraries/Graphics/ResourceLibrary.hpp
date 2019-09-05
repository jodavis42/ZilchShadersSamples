///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GraphicsStandard.hpp"

namespace Graphics
{

//-------------------------------------------------------------------Resource
class Resource
{
public:
  virtual ~Resource() {};
};

//-------------------------------------------------------------------BaseResourceLibrary
class BaseResourceLibrary
{
public:
  typedef HashMap<String, Resource*> ResourceMap;
  virtual ~BaseResourceLibrary();
  virtual void Destroy();

protected:
  virtual void AddInternal(const String& resourceName, Resource* resource);
  virtual Resource* FindInternal(const String& resourceName);
  virtual Resource* GetDefaultInternal();

  // Base range to iterate the resource map. It's expected to derive from this to define Front().
  struct BaseRange
  {
    typedef ResourceMap::range RangeType;
    BaseRange() {};
    BaseRange(const RangeType& range) { mRange = range; }
    void PopFront() { mRange.PopFront(); }
    bool Empty() { return mRange.Empty(); }
    RangeType mRange;
  };

public:
  ResourceMap mResourceMap;
};

//-------------------------------------------------------------------ResourceLibrary
template <typename ResourceType>
class ResourceLibrary : public BaseResourceLibrary
{
public:
  virtual void Add(const String& resourceName, ResourceType* resource) { AddInternal(resourceName, resource); }
  virtual ResourceType* Find(const String& resourceName) { return (ResourceType*)FindInternal(resourceName); }
  virtual ResourceType* GetDefault() { return (ResourceType*)GetDefaultInternal(); }

  struct KeyRange : public BaseRange
  {
    KeyRange(const RangeType& range) : BaseRange(range) {}
    String Front() { return mRange.Front().first; }
    KeyRange All() { return *this; }
  };
  struct ValueRange : public BaseRange
  {
    ValueRange(const RangeType& range) : BaseRange(range) {}
    ResourceType* Front() { return (ResourceType*)mRange.Front().second; }
    KeyRange All() { return *this; }
  };

  KeyRange Keys() { return KeyRange(mResourceMap.All()); }
  ValueRange Values() { return ValueRange(mResourceMap.All()); }
};

}//namespace Graphics
