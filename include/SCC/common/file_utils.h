#pragma once

#include <filesystem>
#include <string>

namespace scc::common {

std::string ResourcesPath();

void ValidateFileExists(const std::filesystem::path& path);

} // scc::common
