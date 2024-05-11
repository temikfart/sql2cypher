#include <stdexcept>

#include "gtest/gtest.h"

#include "SCC/ast/data_types.h"
#include "SCC/ast/nodes/inode.h"
#include "SCC/ast/nodes/int_node.h"
#include "SCC/ast/nodes/service_node.h"
#include "SCC/ast/stmt_types.h"

using namespace scc::ast;
using namespace testing;

template<typename T>
using SharedPtr = std::shared_ptr<T>;

TEST(ASTINodeTests, OneArgCtorTest) {
  SharedPtr<INode> node = std::make_shared<ServiceNode>();

  EXPECT_EQ(node->line, 0);
  EXPECT_EQ(node->data_type, DataType::kService);
  EXPECT_EQ(node->stmt_type, StmtType::kNone);
}
TEST(ASTINodeTests, TwoArgsCtorTest) {
  SharedPtr<INode> node = std::make_shared<ServiceNode>(StmtType::kQuery);

  EXPECT_EQ(node->line, 0);
  EXPECT_EQ(node->data_type, DataType::kService);
  EXPECT_EQ(node->stmt_type, StmtType::kQuery);
}
TEST(ASTINodeTests, ThreeArgsCtorTest) {
  SharedPtr<INode> node = std::make_shared<ServiceNode>(StmtType::kQuery, 100500);

  EXPECT_EQ(node->line, 100500);
  EXPECT_EQ(node->data_type, DataType::kService);
  EXPECT_EQ(node->stmt_type, StmtType::kQuery);
}

TEST(ASTINodeTests, AddChildTest) {
  SharedPtr<INode> parent = std::make_shared<ServiceNode>(StmtType::kProgram);
  SharedPtr<INode> service_node_child = std::make_shared<ServiceNode>(StmtType::kQuery);
  SharedPtr<INode> int_num_node_child = std::make_shared<IntNumNode>(56);

  EXPECT_NO_THROW(
      parent->AddChild(service_node_child);
      EXPECT_EQ(service_node_child->parent.get(), nullptr);

      parent->AddChild(int_num_node_child);
      EXPECT_EQ(int_num_node_child->parent.get(), nullptr);
  );
}
TEST(ASTINodeTests, ChildrenCountTest) {
  SharedPtr<INode> parent = std::make_shared<ServiceNode>(StmtType::kProgram);
  SharedPtr<INode> first_child = std::make_shared<ServiceNode>(StmtType::kQuery);
  SharedPtr<INode> second_child = std::make_shared<IntNumNode>(23465);

  EXPECT_NO_THROW(
      parent->AddChild(first_child);
      EXPECT_EQ(parent->ChildrenCount(), 1);

      parent->AddChild(second_child);
      EXPECT_EQ(parent->ChildrenCount(), 2);
  );
}
TEST(ASTINodeTests, ChildGetterTest) {
  SharedPtr<INode> parent = std::make_shared<ServiceNode>(StmtType::kProgram);
  SharedPtr<INode> first_child = std::make_shared<ServiceNode>(StmtType::kQuery);
  SharedPtr<INode> second_child = std::make_shared<IntNumNode>(257);

  EXPECT_NO_THROW(
      parent->AddChild(first_child);
      auto child = parent->Child(0);
      EXPECT_TRUE(child == first_child);
  );
  EXPECT_THROW(parent->Child(1), std::out_of_range);
  EXPECT_THROW(parent->Child(2), std::out_of_range);

  EXPECT_NO_THROW(
      parent->AddChild(second_child);
      auto child = parent->Child(1);
      EXPECT_TRUE(child == second_child);
  );
  EXPECT_THROW(parent->Child(2), std::out_of_range);
}
