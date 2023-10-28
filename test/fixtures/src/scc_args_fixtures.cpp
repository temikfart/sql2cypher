#include "SCC/fixtures/scc_args_fixtures.h"

using namespace scc;

void SCCArgsBaseTests::ParseArgsWrapper() {
  parser.ParseArgs(args.argc, args.argv.data());
}
char* SCCArgsBaseTests::CreateArg(const std::string& option, const std::string& value) {
  char delimiter[] = "=";
  char* arg = new char[option.length() + value.length() + sizeof(delimiter) + 1];

  strcpy(arg, option.c_str());
  strcat(arg, delimiter);
  strcat(arg, value.c_str());

  return arg;
}
void SCCArgsBaseTests::AddArg(const std::string& option) {
  char* arg = new char[option.length() + 1];
  strcpy(arg, option.c_str());

  args.argv.push_back(arg);
  args.argc++;
}
void SCCArgsBaseTests::AddArg(const std::string& option, const std::string& value) {
  char* arg = CreateArg(option, value);

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
  AddArg("--sql", sql_path);
}

void SCCArgsTests::SetUp() {
  SCCArgsBaseTests::SetUp();
  AddSqlArgWithDefaultValue();
}
void SCCArgsTests::TearDown() {
  SCCArgsBaseTests::TearDown();
}
