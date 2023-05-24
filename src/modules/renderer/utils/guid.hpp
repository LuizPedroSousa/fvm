#pragma once
#include <cstddef>

using ObjectID = size_t;
using TypeID = size_t;

using EntityID = ObjectID;
using EntityTypeID = TypeID;

using ComponentID = ObjectID;
using ComponentTypeID = TypeID;

using SystemTypeID = TypeID;

template <typename T>
class FamilyTypeID {
  private:
  static TypeID s_count;

  public:
  template <typename U>
  static TypeID get() {
    static const TypeID STATIC_TYPE_ID{s_count++};
    return STATIC_TYPE_ID;
  }

  static TypeID get() {
    return s_count;
  }
};

template <typename T>
class FamilyObjectID {
  private:
  FamilyObjectID() {}
  static TypeID s_count;

  public:
  constexpr static TypeID get() {
    return s_count++;
  }
};