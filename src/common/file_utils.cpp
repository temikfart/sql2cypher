#include "SCC/common/file_utils.h"

namespace scc::common {

std::string ResourcesPath() {
  std::string current_file = __FILE__;
  std::string filename = current_file.substr(current_file.find_last_of('/') + 1);
  std::string current_dir = current_file.substr(0, current_file.find(filename));

  return current_dir + "../../resources";
}

} // scc::common
