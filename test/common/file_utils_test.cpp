#include <filesystem>

#include "gtest/gtest.h"

#include "SCC/common/file_utils.h"

using namespace scc;
using namespace testing;

namespace fs = std::filesystem;

TEST(ResourcesPathTests, PathExistsTest) {
  std::string resources_path = common::ResourcesPath();
  ASSERT_TRUE(fs::exists(resources_path))
                << "Path to the resources folder does not exist";
}
