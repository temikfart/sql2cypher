#pragma once

#include "gtest/gtest.h"

#include "SCC/config/scc_config.h"
#include "SCC/fixtures/scc_args_fixtures.h"

class SCCConfigBaseTests : public SCCArgsTests {
protected:
  scc::config::SCCConfig* config = nullptr;

  void SetUp() override;
  void TearDown() override;
};

class SCCConfigTests : public SCCConfigBaseTests {
protected:
  void InitializeConfig();

  void SetUp() override;
  void TearDown() override;
};

class CustomSCCConfigTests : public SCCArgsBaseTests {
protected:
  scc::config::SCCConfig* config;

  void InitializeConfig();

  void SetUp() override;
  void TearDown() override;
};
