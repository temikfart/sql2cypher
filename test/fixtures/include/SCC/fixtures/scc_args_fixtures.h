#pragma once

#include <cstring>

#include <vector>
#include <string>

#include "gtest/gtest.h"

#include "SCC/common/file_utils.h"
#include "SCC/config/scc_args.h"

class SCCArgsBaseTests : public testing::Test {
private:
  char* CreateArg(const std::string& option, const std::string& value);

protected:
  struct Args {
    int argc = 0;
    std::vector<const char*> argv;
  };

  Args args;
  scc::SCCArgs parser;
  const std::string sql_path = scc::common::ResourcesPath() + "/sql_queries.sql";

  void ParseArgsWrapper();
  void AddArg(const std::string& option);
  void AddArg(const std::string& option, const std::string& value);

  void AddProgramName();
  void DeleteArgs();

  void SetUp() override;
  void TearDown() override;
};

class SCCArgsTests : public SCCArgsBaseTests {
protected:
  void AddSqlArgWithDefaultValue();

  void SetUp() override;
  void TearDown() override;
};
