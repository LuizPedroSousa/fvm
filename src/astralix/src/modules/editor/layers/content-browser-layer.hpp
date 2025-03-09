#include "guid.hpp"
#include "entities/layer.hpp"
#include "filesystem"

namespace astralix {

  class ContentBrowserLayer : public Layer {
  public:
    ContentBrowserLayer();
    void start() override;
    void update() override;

  private:
    std::filesystem::path m_base_directory;
    std::filesystem::path m_current_directory;

    void draw_sub_directories(std::filesystem::path directory);
    void draw_aside_directory_view();
    void draw_directory_view();
    void draw_content_header();

    u_int32_t m_directory_icon_id;
    u_int32_t m_file_icon_id;
  };

} // namespace astralix
