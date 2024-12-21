#pragma once
#include "string"

namespace astralix
{

  struct Layout
  {
    Layout(const std::string& name, const std::string& ini_settings) : name(name), ini_settings(ini_settings) {}

    std::string name = "Untitled";
    std::string ini_settings;
  };

} // namespace astralix
