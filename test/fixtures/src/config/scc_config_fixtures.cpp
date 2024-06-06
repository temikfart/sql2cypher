#include "SCC/fixtures/config/scc_config_fixtures.h"

using namespace scc::config;

void SCCConfigBaseTests::SetUp() {
  SCCArgsTests::SetUp();
  SCCArgsTests::AddTranslateSchemaOption();
  SCCArgsTests::AddTranslateDataOption();
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
