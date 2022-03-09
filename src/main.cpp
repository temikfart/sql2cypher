#include "SCC/main.h"

int main() {
  LOG(INFO, "Starting system...")
  configure();
  LOG(INFO, "System started")

  test_tree_func();
  
  return 0;
}
