#include "gtest/gtest.h"

#include "SCC/ast/data_types.h"
#include "SCC/ast/nodes/service_node.h"
#include "SCC/ast/stmt_types.h"

using namespace scc::ast;
using namespace testing;

template<typename T>
using SharedPtr = std::shared_ptr<T>;

TEST(ASTServiceNodeTests, ZeroArgsCtorTest) {
  SharedPtr<ServiceNode> node = std::make_shared<ServiceNode>();

  EXPECT_EQ(node->line, 0);
  EXPECT_EQ(node->data_type, DataType::kService);
  EXPECT_EQ(node->stmt_type, StmtType::kNone);
}
TEST(ASTServiceNodeTests, OneArgCtorTest) {
  SharedPtr<ServiceNode> node = std::make_shared<ServiceNode>(StmtType::kQuery);

  EXPECT_EQ(node->line, 0);
  EXPECT_EQ(node->data_type, DataType::kService);
  EXPECT_EQ(node->stmt_type, StmtType::kQuery);
}
TEST(ASTServiceNodeTests, TwoArgsCtorTest) {
  SharedPtr<ServiceNode> node = std::make_shared<ServiceNode>(StmtType::kQuery, 57);

  EXPECT_EQ(node->line, 57);
  EXPECT_EQ(node->data_type, DataType::kService);
  EXPECT_EQ(node->stmt_type, StmtType::kQuery);
}
