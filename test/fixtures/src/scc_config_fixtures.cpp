#include "SCC/fixtures/scc_config_fixtures.h"

using namespace scc;

void SCCConfigBaseTests::SetUp() {
  SCCArgsTests::SetUp();
  ParseArgsWrapper();
}
void SCCConfigBaseTests::TearDown() {
  SCCArgsTests::TearDown();
}

void SCCConfigTests::InitializeConfig() {
  InitConfig(parser);
  config = SCCConfig::Get();
}

void SCCConfigTests::SetUp() {
  SCCConfigBaseTests::SetUp();
  InitializeConfig();
}
void SCCConfigTests::TearDown() {
  SCCConfigBaseTests::TearDown();
}

void CustomSCCConfigTests::InitializeConfig() {
  InitConfig(parser);
  config = SCCConfig::Get();
}

void CustomSCCConfigTests::SetUp() {
  SCCArgsBaseTests::SetUp();
}
void CustomSCCConfigTests::TearDown() {
  SCCArgsBaseTests::TearDown();
}
