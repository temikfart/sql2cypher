#include "gtest/gtest.h"

#include "SCC/ast/data_types.h"
#include "SCC/ast/nodes/char_node.h"
#include "SCC/ast/stmt_types.h"

using namespace scc::ast;
using namespace testing;

template<typename T>
using SharedPtr = std::shared_ptr<T>;

TEST(ASTCharNodeTests, TwoArgsCtorTest) {
  SharedPtr<CharNode> node = std::make_shared<CharNode>(',', DataType::kPunctuation);

  EXPECT_EQ(node->line, 0);
  EXPECT_EQ(node->data_type, DataType::kPunctuation);
  EXPECT_EQ(node->stmt_type, StmtType::kNone);
  EXPECT_EQ(node->data, ',');
}
