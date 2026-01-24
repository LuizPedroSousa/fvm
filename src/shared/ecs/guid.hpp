#pragma once
#include "cstddef"
#include "string"
#include <cstddef>
#include <stdint.h>
#include <variant>

namespace astralix {

using SerializableValue = std::variant<int, float, std::string, bool>;
using SerializableKey = std::variant<int, float, std::string, bool>;

using DeserializableValue = std::variant<int, float, std::string, bool>;
using DeserializableKey = std::variant<int, float, std::string, bool>;

class Guid {
public:
  Guid();
  Guid(uint64_t value);
  Guid(const Guid &) = default;

  operator uint64_t() const { return m_value; }
  operator std::string() const { return std::to_string(m_value); }

private:
  uint64_t m_value;
};

using ObjectID = Guid;
using SceneID = Guid;
using TypeID = size_t;

using EntityID = Guid;
using EntityFamilyID = size_t;
using EntityTypeID = TypeID;

using ComponentID = ObjectID;
using ComponentTypeID = TypeID;
using SchedulerID = Guid;

using SystemTypeID = TypeID;
using ResourceID = std::string;
using ProjectID = Guid;
using PathID = Guid;
using ListenerId = Guid;
using MeshID = std::size_t;
using MeshGroupID = std::size_t;

template <typename T> class FamilyTypeID {
private:
  static TypeID s_count;

public:
  template <typename U> static TypeID get() {
    static const TypeID STATIC_TYPE_ID{s_count++};
    return STATIC_TYPE_ID;
  }

  static TypeID get() { return s_count; }
};

template <typename T> class FamilyObjectID {
private:
  FamilyObjectID() {}
  static TypeID s_count;

public:
  constexpr static TypeID get() { return s_count++; }
};

} // namespace astralix

namespace std {
template <typename T> struct hash;

template <> struct hash<astralix::Guid> {
  std::size_t operator()(const astralix::Guid &guid) const {
    return (uint64_t)guid;
  }
};
}; // namespace std
