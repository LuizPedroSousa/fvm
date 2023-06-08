#pragma once
#include "components/base/component.hpp"
#include "either.hpp"
#include "iostream"
#include "memory"
#include "unordered_map"
#include "utils/guid.hpp"

using IComponent_ptr = std::shared_ptr<IComponent>;

class IComponentContainer {
  public:
  virtual ~IComponentContainer() {}

  virtual void destroy_component(const ComponentID &component_id) = 0;
};

template <typename T>
class ComponentContainer : public IComponentContainer {
  using TypedComponentContainer = std::unordered_map<ComponentID, std::shared_ptr<Component<T>>>;

  private:
  TypedComponentContainer m_typed_container;

  public:
  ComponentContainer() {}
  TypedComponentContainer &get_typed_component_container() { return m_typed_container; }
  void destroy_component(const ComponentID &component_id) override {
    m_typed_container.erase(component_id);
  }
};

class ComponentManager {
  using IComponentContainer_ptr = std::shared_ptr<IComponentContainer>;

  public:
  ComponentManager() = default;
  ~ComponentManager() = default;

  template <typename T, typename... Args>
  T *add_component(const EntityID &entity_id, Args &&...params) {
    ComponentID id = FamilyObjectID<IComponent>::get();

    std::shared_ptr<T> component_ptr = std::make_shared<T>(entity_id, id, std::forward<Args>(params)...);
    const ComponentTypeID component_type_id = T::component_type_id();

    m_entity_component_table[entity_id][component_type_id] = id;

    auto created_component = m_component_table.try_emplace(id, component_ptr);

    ASSERT_THROW(!created_component.second, "Can't create a new Component !");

    Either<BaseException, ComponentContainer<T> *> container_ptr = get_component_container<T>();

    ASSERT_COMPARE_THROW(container_ptr);

    auto component = container_ptr.right()->get_typed_component_container().try_emplace(id, component_ptr);

    ASSERT_THROW(!component.second, "Can't insert Component in typed containers !");

    return dynamic_cast<T *>(m_component_table[id].get());
  }

  template <typename T>
  void remove_component(const EntityID &entity_id) {
    const ComponentTypeID type_id = T::component_type_id();
    const ComponentID id = this->m_entity_component_table.at(entity_id).at(type_id);

    m_component_type_table[type_id]->destroy_component(id);
    m_component_table.erase(id);
    m_entity_component_table.at(entity_id).erase(type_id);
  }

  void clean_components(const EntityID entity_id);

  template <typename T>
  std::vector<T *> get_components() {
    const ComponentTypeID type_id = T::component_type_id();

    auto type_container_iter = m_component_type_table.find(type_id);
    if (type_container_iter == m_component_type_table.end()) {
      return {};
    }

    ComponentContainer<T> *component_container = dynamic_cast<ComponentContainer<T> *>(type_container_iter->second.get());
    if (component_container == nullptr) {
      return {};
    }

    auto &typed_container = component_container->get_typed_component_container();

    std::vector<T *> components;
    for (const auto &pair : typed_container) {
      T *component = dynamic_cast<T *>(pair.second.get());
      if (component != nullptr) {
        components.push_back(component);
      }
    }

    return components;
  }

  template <typename T>
  T *get_component(const EntityID entity_id) {
    const ComponentTypeID type_id = T::component_type_id();
    try {
      const ComponentID id = this->m_entity_component_table.at(entity_id).at(type_id);
      return dynamic_cast<T *>(m_component_table[id].get());
    } catch (std::out_of_range) {
      return nullptr;
    }
  }

  template <typename T>
  Either<BaseException, ComponentContainer<T> *> get_component_container() {
    ComponentTypeID type_id = T::component_type_id();

    auto type_container_exists = this->m_component_type_table.find(type_id);
    if (type_container_exists != m_component_type_table.end())
      return reinterpret_cast<ComponentContainer<T> *>(type_container_exists->second.get());

    auto type_container_ptr = this->m_component_type_table.try_emplace(type_id, std::make_unique<ComponentContainer<T>>());

    ASSERT(!type_container_ptr.second, "Failed to create EntityContainer<T>!");

    return reinterpret_cast<ComponentContainer<T> *>(type_container_ptr.first->second.get());
  }

  private:
  std::unordered_map<ComponentTypeID, IComponentContainer_ptr> m_component_type_table;
  std::unordered_map<ComponentID, IComponent_ptr> m_component_table;
  std::unordered_map<EntityID, std::unordered_map<ComponentTypeID, ComponentID>> m_entity_component_table;
};
