#include "gtest/gtest.h"

#include "SCC/ast/data_types.h"
#include "SCC/ast/nodes/root_node.h"
#include "SCC/ast/stmt_types.h"

using namespace scc::ast;
using namespace testing;

template<typename T>
using SharedPtr = std::shared_ptr<T>;

TEST(ASTRootNodeTests, ZeroArgsCtorTest) {
  SharedPtr<RootNode> node = std::make_shared<RootNode>();

  EXPECT_EQ(node->line, 0);
  EXPECT_EQ(node->data_type, DataType::kRoot);
  EXPECT_EQ(node->stmt_type, StmtType::kNone);
}
TEST(ASTRootNodeTests, OneArgCtorTest) {
  SharedPtr<RootNode> node = std::make_shared<RootNode>(StmtType::kProgram);

  EXPECT_EQ(node->line, 0);
  EXPECT_EQ(node->data_type, DataType::kRoot);
  EXPECT_EQ(node->stmt_type, StmtType::kProgram);
}
