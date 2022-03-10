#include "SCC/main.h"

int main() {
  LOG(INFO, "Starting system...")
  configure();
  LOG(INFO, "System started")

  Tree::TestTree();

  return 0;
}
