#include "serializer.hpp"
#include "assert.hpp"
#include "filesystem"
#include "fstream"
#include "json/json.h"

namespace astralix {
Json::Value Serializer::read(const std::filesystem::path &absolute_path) {
  bool file_exists = std::filesystem::exists(absolute_path);

  ASTRA_ASSERT_THROW(!file_exists,
                     "File" + std::string(absolute_path) + " doesn't exists");
  Json::Value root;

  auto file = std::ifstream(absolute_path);

  Json::CharReaderBuilder rbuilder;

  rbuilder["collectComments"] = false;
  std::string errs;
  bool ok = Json::parseFromStream(rbuilder, file, &root, &errs);

  ASTRA_ASSERT_THROW(!ok, "Canno parse" + std::string(absolute_path));

  return root;
}

void Serializer::write(const std::filesystem::path &absolute_path,
                       Json::Value &data) {

  auto directory = absolute_path.parent_path();

  bool directory_exists = std::filesystem::exists(directory) &&
                          std::filesystem::is_directory(directory);

  if (!directory_exists) {
    ASTRA_ASSERT_THROW(!std::filesystem::create_directory(directory),
                       "Cannot create directory");
  }

  Json::StreamWriterBuilder writer;
  std::string json_string = Json::writeString(writer, data);

  std::ofstream out_file(absolute_path);

  if (out_file.is_open()) {
    out_file << json_string;
    out_file.close();
  } else {
    ASTRA_ASSERT_THROW(true,
                       "Canno write serialized" + std::string(absolute_path))
  }
}

} // namespace astralix