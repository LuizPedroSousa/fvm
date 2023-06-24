
namespace astralix {

struct MSAA {
  int samples;
  bool is_enabled;
};

class RenderConfiguration {
public:
  RenderConfiguration(MSAA msaa) : msaa(msaa){};

  bool has_msaa_enabled() { return msaa.is_enabled; }

  MSAA msaa;
};

} // namespace astralix