#pragma once
#include "base.hpp"
#include "filesystem"
#include "json/json.h"

namespace astralix {

class Serializer {
public:
  Json::Value virtual parse() = 0;
  void virtual serialize() = 0;
  void virtual deserialize() = 0;

protected:
  Json::Value read(const std::filesystem::path &absolute_path);
  void write(const std::filesystem::path &absolute_path, Json::Value &data);
};

} // namespace astralix
