#include "SCC/main.h"

int main(int argc, char* argv[]) {
  LOG(INFO, "Starting system...");
  Configure(argc, argv);
  LOG(INFO, "System started");

  Tree::TestTree();

  return 0;
}
