#pragma once

#include <vector>
#include <string>

#include "gtest/gtest.h"

#include "SCC/common/file_utils.h"
#include "SCC/config/scc_args.h"

class SCCArgsBaseTests : public testing::Test {
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
  template<typename... Values> // TODO: refactor to one value.
  void AddArg(std::format_string<Values...> format_option, Values&&... values) {
    AddArg(std::format(format_option, values...));
  }

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
