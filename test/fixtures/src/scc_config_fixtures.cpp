#include "SCC/fixtures/scc_config_fixtures.h"

using namespace scc;

void SCCConfigBaseTests::SetUp() {
  AddProgramName();
  AddSqlArgWithDefaultValue();
  ParseArgsWrapper();
}

