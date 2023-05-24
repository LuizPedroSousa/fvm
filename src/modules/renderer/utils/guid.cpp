#include "guid.hpp"

class IEntity;
class IComponent;
class ISystem;

// Specialize the static members using 'template<>'
template <>
TypeID FamilyTypeID<IEntity>::s_count = 0u;
template <>
TypeID FamilyTypeID<IComponent>::s_count = 0u;
template <>
TypeID FamilyTypeID<ISystem>::s_count = 0u;

// Explicitly instantiate the specialized members
template class FamilyTypeID<IEntity>;
template class FamilyTypeID<IComponent>;
template class FamilyTypeID<ISystem>;

// Specialize the static members using 'template<>'
template <>
TypeID FamilyObjectID<IEntity>::s_count = 0u;
template <>
TypeID FamilyObjectID<IComponent>::s_count = 0u;
template <>
TypeID FamilyObjectID<ISystem>::s_count = 0u;

// Explicitly instantiate the specialized members
template class FamilyObjectID<IEntity>;
template class FamilyObjectID<IComponent>;
template class FamilyObjectID<ISystem>;