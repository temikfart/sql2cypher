#include <filesystem>
#include <stdexcept>

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

TEST(ValidateFileExistsTests, FileExistsTest) {
  fs::path valid_path = fs::current_path();
  EXPECT_NO_THROW(common::ValidateFileExists(valid_path));
}
TEST(ValidateFileExistsTests, FileDoesNotExistsTest) {
  fs::path invalid_path = fs::path("");
  EXPECT_THROW(common::ValidateFileExists(invalid_path), std::runtime_error);
}
