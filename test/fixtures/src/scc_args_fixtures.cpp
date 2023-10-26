#include "SCC/fixtures/scc_args_fixtures.h"

using namespace scc;

void SCCArgsBaseTests::ParseArgsWrapper() {
  parser.ParseArgs(args.argc, args.argv.data());
}
void SCCArgsBaseTests::AddArg(const std::string& option) {
  char* arg = new char[option.length() + 1];
  strcpy(arg, option.c_str());

  args.argv.push_back(arg);
  args.argc++;
}

void SCCArgsBaseTests::AddProgramName() {
  AddArg("scc");
}
void SCCArgsBaseTests::DeleteArgs() {
  for (const char* arg : args.argv)
    delete[] arg;
}

void SCCArgsBaseTests::SetUp() {
  AddProgramName();
}
void SCCArgsBaseTests::TearDown() {
  DeleteArgs();
}

void SCCArgsTests::AddSqlArgWithDefaultValue() {
  AddArg("--sql={}", sql_path);
}

void SCCArgsTests::SetUp() {
  SCCArgsBaseTests::SetUp();
  AddSqlArgWithDefaultValue();
}
void SCCArgsTests::TearDown() {
  SCCArgsBaseTests::TearDown();
}
