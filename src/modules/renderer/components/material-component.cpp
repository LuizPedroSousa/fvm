#include "game.hpp"
#include "material-component.hpp"
#include "resource-component.hpp"

MaterialComponent::MaterialComponent(COMPONENT_INIT_PARAMS) : COMPONENT_INIT(MaterialComponent){};

Either<BaseException, Unit> MaterialComponent::load_maps(Either<BaseException, Texture> diffuse, Either<BaseException, Texture> specular, float shininess) {
  ASSERT_COMPARE(diffuse);
  ASSERT_COMPARE(specular);

  auto owner = Game::get()->get_entity_manager()->get_entity(get_owner_id());

  auto resource = owner->get_component<ResourceComponent>();

  auto diffuse_loaded = resource->load_texture(diffuse.right());
  ASSERT_COMPARE(specular);

  auto specular_loaded = resource->load_texture(specular.right());
  ASSERT_COMPARE(specular);

  m_material = {
      .diffuse = diffuse.right(),
      .specular = specular.right()};

  return Unit();
}