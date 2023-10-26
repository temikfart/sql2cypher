#include "SCC/fixtures/scc_config_fixtures.h"

using namespace scc;

void SCCConfigBaseTests::SetUp() {
  SCCArgsTests::SetUp();
  ParseArgsWrapper();
}
void SCCConfigBaseTests::TearDown() {
  SCCArgsTests::TearDown();
}

