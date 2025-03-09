#include "guid.hpp"
#include "systems/system.hpp"
#include "resources/mesh.hpp"
#include "storage-buffer.hpp"
#include <unordered_map>
#include <vector>

namespace astralix {

  struct Batch {
    std::vector<Mesh> meshes;
    std::unordered_map<EntityID, glm::mat4> transforms;
    bool is_dirty = true;
  };

  class MeshSystem : public System<MeshSystem> {
  public:
    MeshSystem();
    ~MeshSystem();

    void start() override;
    void fixed_update(double fixed_dt) override;
    void pre_update(double dt) override;
    void update(double dt) override;

  private:
    MeshBatchID compute_batch_id(std::vector<Mesh>& mesh);

    std::unordered_map<MeshBatchID, Batch> m_batches;

    Ref<StorageBuffer> m_storage_buffer;
  };

} // namespace astralix
