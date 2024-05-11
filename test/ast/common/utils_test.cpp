#include "gtest/gtest.h"

#include "SCC/ast/data_types.h"
#include "SCC/ast/nodes/char_node.h"
#include "SCC/ast/nodes/float_node.h"
#include "SCC/ast/nodes/inode.h"
#include "SCC/ast/nodes/int_node.h"
#include "SCC/ast/nodes/root_node.h"
#include "SCC/ast/nodes/service_node.h"
#include "SCC/ast/nodes/string_node.h"
#include "SCC/ast/stmt_types.h"
#include "SCC/ast/common/utils.h"

using namespace scc::ast;
using namespace scc::ast::common;
using namespace testing;

template<typename T>
using SharedPtr = std::shared_ptr<T>;

TEST(ASTUtilsCastToBaseTests, CastCharNodeToINodeTest) {
  SharedPtr<CharNode> char_node = std::make_shared<CharNode>(')', DataType::kBracket);
  EXPECT_NO_THROW(
      SharedPtr<INode> inode = ASTUtils::CastToINode(char_node);
      SharedPtr<CharNode> casted_char_node = std::static_pointer_cast<CharNode>(inode);
      EXPECT_EQ(casted_char_node->data, char_node->data);
      EXPECT_EQ(casted_char_node->data_type, char_node->data_type);
      EXPECT_EQ(casted_char_node->ChildrenCount(), char_node->ChildrenCount());
  );
}
TEST(ASTUtilsCastToBaseTests, CastFloatNumNodeToINodeTest) {
  SharedPtr<FloatNumNode> float_num_node = std::make_shared<FloatNumNode>(0.05f);
  EXPECT_NO_THROW(
      SharedPtr<INode> inode = ASTUtils::CastToINode(float_num_node);
      SharedPtr<FloatNumNode> casted_float_num_node = std::static_pointer_cast<FloatNumNode>(inode);
      EXPECT_EQ(casted_float_num_node->data, float_num_node->data);
      EXPECT_EQ(casted_float_num_node->data_type, float_num_node->data_type);
      EXPECT_EQ(casted_float_num_node->ChildrenCount(), float_num_node->ChildrenCount());
  );
}
TEST(ASTUtilsCastToBaseTests, CastINodeToINodeTest) {
  SharedPtr<CharNode> char_node = std::make_shared<CharNode>(',', DataType::kPunctuation);
  EXPECT_NO_THROW(
      SharedPtr<INode> casted_inode = std::static_pointer_cast<INode>(char_node);
      SharedPtr<INode> casted_casted_inode = ASTUtils::CastToINode(casted_inode);
      EXPECT_EQ(casted_casted_inode->line, casted_inode->line);
      EXPECT_EQ(casted_casted_inode->data_type, casted_inode->data_type);
      EXPECT_EQ(casted_casted_inode->stmt_type, casted_inode->stmt_type);
      EXPECT_EQ(casted_casted_inode->ChildrenCount(), casted_inode->ChildrenCount());
  );
}
TEST(ASTUtilsCastToBaseTests, CastIntNumNodeToINodeTest) {
  SharedPtr<IntNumNode> int_num_node = std::make_shared<IntNumNode>(100500);
  EXPECT_NO_THROW(
      SharedPtr<INode> inode = ASTUtils::CastToINode(int_num_node);
      SharedPtr<IntNumNode> casted_int_num_node = std::static_pointer_cast<IntNumNode>(inode);
      EXPECT_EQ(casted_int_num_node->data, int_num_node->data);
      EXPECT_EQ(casted_int_num_node->data_type, int_num_node->data_type);
      EXPECT_EQ(casted_int_num_node->ChildrenCount(), int_num_node->ChildrenCount());
  );
}
TEST(ASTUtilsCastToBaseTests, CastRootNodeToINodeTest) {
  SharedPtr<RootNode> root_node = std::make_shared<RootNode>(StmtType::kPrimaryKey);
  EXPECT_NO_THROW(
      SharedPtr<INode> inode = ASTUtils::CastToINode(root_node);
      SharedPtr<RootNode> casted_root_node = std::static_pointer_cast<RootNode>(inode);
      EXPECT_EQ(casted_root_node->data_type, root_node->data_type);
      EXPECT_EQ(casted_root_node->stmt_type, root_node->stmt_type);
      EXPECT_EQ(casted_root_node->ChildrenCount(), root_node->ChildrenCount());
  );
}
TEST(ASTUtilsCastToBaseTests, CastServiceNodeToINodeTest) {
  SharedPtr<ServiceNode> service_node = std::make_shared<ServiceNode>(StmtType::kDropDatabaseStmt);
  EXPECT_NO_THROW(
      SharedPtr<INode> inode = ASTUtils::CastToINode(service_node);
      SharedPtr<ServiceNode> casted_service_node = std::static_pointer_cast<ServiceNode>(inode);
      EXPECT_EQ(casted_service_node->data_type, service_node->data_type);
      EXPECT_EQ(casted_service_node->stmt_type, service_node->stmt_type);
      EXPECT_EQ(casted_service_node->ChildrenCount(), service_node->ChildrenCount());
  );
}
TEST(ASTUtilsCastToBaseTests, CastStringNodeToINodeTest) {
  SharedPtr<StringNode> string_node = std::make_shared<StringNode>("word", DataType::kWord);
  EXPECT_NO_THROW(
      SharedPtr<INode> inode = ASTUtils::CastToINode(string_node);
      SharedPtr<StringNode> casted_string_node = std::static_pointer_cast<StringNode>(inode);
      EXPECT_EQ(casted_string_node->data, string_node->data);
      EXPECT_EQ(casted_string_node->data_type, string_node->data_type);
      EXPECT_EQ(casted_string_node->ChildrenCount(), string_node->ChildrenCount());
  );
}

TEST(ASTUtilsCastToNodeTypeTests, CastToNodeTypeCharTest) {
  SharedPtr<CharNode> char_node = std::make_shared<CharNode>('[', DataType::kBracket);
  SharedPtr<INode> casted_to_inode = std::static_pointer_cast<INode>(char_node);
  EXPECT_NO_THROW(
      SharedPtr<CharNode> casted_to_char_node =
          ASTUtils::CastToNodeType<CharNode>(casted_to_inode);
      EXPECT_EQ(casted_to_char_node->data, char_node->data);
      EXPECT_EQ(casted_to_char_node->data_type, char_node->data_type);
      EXPECT_EQ(casted_to_char_node->ChildrenCount(), char_node->ChildrenCount());
  );
}
TEST(ASTUtilsCastToNodeTypeTests, CastToNodeTypeFloatNumTest) {
  SharedPtr<FloatNumNode> float_num_node = std::make_shared<FloatNumNode>(0.123456f);
  SharedPtr<INode> casted_to_inode = std::static_pointer_cast<INode>(float_num_node);
  EXPECT_NO_THROW(
      SharedPtr<FloatNumNode> casted_to_float_num_node =
          ASTUtils::CastToNodeType<FloatNumNode>(casted_to_inode);
      EXPECT_EQ(casted_to_float_num_node->data, float_num_node->data);
      EXPECT_EQ(casted_to_float_num_node->data_type, float_num_node->data_type);
      EXPECT_EQ(casted_to_float_num_node->ChildrenCount(), float_num_node->ChildrenCount());
  );
}
TEST(ASTUtilsCastToNodeTypeTests, CastToNodeTypeINodeTest) {
  SharedPtr<IntNumNode> int_num_node = std::make_shared<IntNumNode>(20819);
  SharedPtr<INode> casted_to_inode = std::static_pointer_cast<INode>(int_num_node);
  EXPECT_NO_THROW(
      SharedPtr<INode> casted_to_casted_inode = ASTUtils::CastToNodeType<INode>(casted_to_inode);
      EXPECT_EQ(casted_to_casted_inode->line, casted_to_inode->line);
      EXPECT_EQ(casted_to_casted_inode->data_type, casted_to_inode->data_type);
      EXPECT_EQ(casted_to_casted_inode->stmt_type, casted_to_inode->stmt_type);
      EXPECT_EQ(casted_to_casted_inode->ChildrenCount(), casted_to_inode->ChildrenCount());
  );
}
TEST(ASTUtilsCastToNodeTypeTests, CastToNodeTypeIntNumTest) {
  SharedPtr<IntNumNode> int_num_node = std::make_shared<IntNumNode>(123456);
  SharedPtr<INode> casted_to_inode = std::static_pointer_cast<INode>(int_num_node);
  EXPECT_NO_THROW(
      SharedPtr<IntNumNode> casted_to_int_num_node =
          ASTUtils::CastToNodeType<IntNumNode>(casted_to_inode);
      EXPECT_EQ(casted_to_int_num_node->data, int_num_node->data);
      EXPECT_EQ(casted_to_int_num_node->data_type, int_num_node->data_type);
      EXPECT_EQ(casted_to_int_num_node->ChildrenCount(), int_num_node->ChildrenCount());
  );
}
TEST(ASTUtilsCastToNodeTypeTests, CastToNodeTypeRootTest) {
  SharedPtr<RootNode> root_node = std::make_shared<RootNode>(StmtType::kCreateTableStmt);
  SharedPtr<INode> casted_to_inode = std::static_pointer_cast<INode>(root_node);
  EXPECT_NO_THROW(
      SharedPtr<RootNode> casted_to_root_node =
          ASTUtils::CastToNodeType<RootNode>(casted_to_inode);
      EXPECT_EQ(casted_to_root_node->data_type, root_node->data_type);
      EXPECT_EQ(casted_to_root_node->stmt_type, root_node->stmt_type);
      EXPECT_EQ(casted_to_root_node->ChildrenCount(), root_node->ChildrenCount());
  );
}
TEST(ASTUtilsCastToNodeTypeTests, CastToNodeTypeServiceTest) {
  SharedPtr<ServiceNode> service_node = std::make_shared<ServiceNode>(StmtType::kAlterTableStmt);
  SharedPtr<INode> casted_to_inode = std::static_pointer_cast<INode>(service_node);
  EXPECT_NO_THROW(
      SharedPtr<ServiceNode> casted_to_service_node =
          ASTUtils::CastToNodeType<ServiceNode>(casted_to_inode);
      EXPECT_EQ(casted_to_service_node->data_type, service_node->data_type);
      EXPECT_EQ(casted_to_service_node->stmt_type, service_node->stmt_type);
      EXPECT_EQ(casted_to_service_node->ChildrenCount(), service_node->ChildrenCount());
  );
}
TEST(ASTUtilsCastToNodeTypeTests, CastToNodeTypeStringTest) {
  SharedPtr<StringNode> string_node = std::make_shared<StringNode>("Ivan", DataType::kWord);
  SharedPtr<INode> casted_to_inode = std::static_pointer_cast<INode>(string_node);
  EXPECT_NO_THROW(
      SharedPtr<StringNode> casted_to_string_node =
          ASTUtils::CastToNodeType<StringNode>(casted_to_inode);
      EXPECT_EQ(casted_to_string_node->data, string_node->data);
      EXPECT_EQ(casted_to_string_node->data_type, string_node->data_type);
      EXPECT_EQ(casted_to_string_node->ChildrenCount(), string_node->ChildrenCount());
  );
}

TEST(ASTUtilsCreationTests, CreateRootNodeTest) {
  EXPECT_NO_THROW(
      SharedPtr<INode> root_node = ASTUtils::CreateRootNode(StmtType::kCreateDatabaseStmt);
      SharedPtr<RootNode> casted_root_node = std::static_pointer_cast<RootNode>(root_node);
      EXPECT_EQ(root_node->data_type, DataType::kRoot);
      EXPECT_EQ(root_node->stmt_type, StmtType::kCreateDatabaseStmt);
      EXPECT_EQ(root_node->ChildrenCount(), 0);
  );
}
TEST(ASTUtilsCreationTests, CreateServiceNodeTest) {
  EXPECT_NO_THROW(
      SharedPtr<INode> service_node = ASTUtils::CreateServiceNode(StmtType::kTableConstraint);
      SharedPtr<ServiceNode> casted_service_node =
          std::static_pointer_cast<ServiceNode>(service_node);
      EXPECT_EQ(service_node->data_type, DataType::kService);
      EXPECT_EQ(service_node->stmt_type, StmtType::kTableConstraint);
      EXPECT_EQ(service_node->ChildrenCount(), 0);
  );
}
TEST(ASTUtilsCreationTests, CreateServiceNodeWithBaseNodeTest) {
  SharedPtr<StringNode> base_node = std::make_shared<StringNode>("Foreign Key", DataType::kString);
  base_node->stmt_type = StmtType::kForeignKey;
  base_node->line = 53;
  EXPECT_NO_THROW(
      SharedPtr<INode> service_node =
          ASTUtils::CreateServiceNode(StmtType::kTableConstraint, base_node);
      SharedPtr<ServiceNode> casted_service_node =
          std::static_pointer_cast<ServiceNode>(service_node);
      EXPECT_EQ(service_node->line, base_node->line);
      EXPECT_EQ(service_node->data_type, DataType::kService);
      EXPECT_EQ(service_node->stmt_type, StmtType::kTableConstraint);
      EXPECT_EQ(service_node->ChildrenCount(), 0);
  );
}
TEST(ASTUtilsCreationTests, CreateCharNodeTest) {
  EXPECT_NO_THROW(
      SharedPtr<INode> char_node = ASTUtils::CreateCharNode('.', DataType::kPunctuation);
      SharedPtr<CharNode> casted_char_node = std::static_pointer_cast<CharNode>(char_node);
      EXPECT_EQ(casted_char_node->data, '.');
      EXPECT_EQ(casted_char_node->data_type, DataType::kPunctuation);
      EXPECT_EQ(casted_char_node->ChildrenCount(), 0);
  );
}
TEST(ASTUtilsCreationTests, CreateStringNodeTest) {
  EXPECT_NO_THROW(
      SharedPtr<INode> string_node = ASTUtils::CreateStringNode("string", DataType::kWord);
      SharedPtr<StringNode> casted_string_node = std::static_pointer_cast<StringNode>(string_node);
      EXPECT_EQ(casted_string_node->data, "string");
      EXPECT_EQ(casted_string_node->data_type, DataType::kWord);
      EXPECT_EQ(casted_string_node->ChildrenCount(), 0);
  );
}

TEST(ASTUtilsLinkTests, LinkTest) {
  SharedPtr<INode> parent_node = std::make_shared<RootNode>(StmtType::kProgram);
  SharedPtr<INode> first_child_node = std::make_shared<ServiceNode>(StmtType::kQuery);

  EXPECT_NO_THROW(
      ASTUtils::Link(parent_node, first_child_node);

      EXPECT_EQ(parent_node->ChildrenCount(), 1);
      EXPECT_EQ(first_child_node->ChildrenCount(), 0);

      EXPECT_EQ(parent_node->Child(0), first_child_node);
      EXPECT_EQ(first_child_node->parent, parent_node);
  );

  SharedPtr<INode> second_child_node = std::make_shared<ServiceNode>(StmtType::kQuery);
  EXPECT_NO_THROW(
      ASTUtils::Link(parent_node, second_child_node);

      EXPECT_EQ(parent_node->ChildrenCount(), 2);
      EXPECT_EQ(second_child_node->ChildrenCount(), 0);

      EXPECT_EQ(parent_node->Child(0), first_child_node);
      EXPECT_EQ(parent_node->Child(1), second_child_node);
      EXPECT_EQ(second_child_node->parent, parent_node);
  );

  SharedPtr<INode> child_for_child_node = std::make_shared<ServiceNode>(StmtType::kDdlStmt);
  EXPECT_NO_THROW(
      ASTUtils::Link(first_child_node, child_for_child_node);

      EXPECT_EQ(first_child_node->ChildrenCount(), 1);
      EXPECT_EQ(child_for_child_node->ChildrenCount(), 0);

      EXPECT_EQ(parent_node->Child(0), first_child_node);
      EXPECT_EQ(parent_node->Child(1), second_child_node);
      EXPECT_EQ(first_child_node->Child(0), child_for_child_node);
      EXPECT_EQ(child_for_child_node->parent, first_child_node);
  );
}

TEST(ASTUtilsEqualOperatorTests, EqualNodesTest) {
  SharedPtr<INode> first_node = std::make_shared<RootNode>(StmtType::kProgram);
  EXPECT_TRUE(first_node == first_node);

  SharedPtr<INode> second_node = std::make_shared<RootNode>(first_node->stmt_type);
  EXPECT_TRUE(first_node == second_node);

  SharedPtr<INode> first_child_node = std::make_shared<ServiceNode>(StmtType::kQuery);
  first_node->AddChild(first_child_node);
  EXPECT_TRUE(first_node == second_node);
}
TEST(ASTUtilsEqualOperatorTests, EqualNodesWithLineTest) {
  auto first_node = std::make_shared<RootNode>();
  first_node->line = 103;

  auto second_node = std::make_shared<RootNode>();
  second_node->line = first_node->line;

  EXPECT_TRUE(first_node == second_node);
}
TEST(ASTUtilsEqualOperatorTests, EqualNodeWithStmtTypeTest) {
  auto first_node = std::make_shared<ServiceNode>(StmtType::kQuery);
  auto second_node = std::make_shared<ServiceNode>(first_node->stmt_type);

  EXPECT_TRUE(first_node == second_node);
}
TEST(ASTUtilsEqualOperatorTests, EqualNodeWithDataTypeTest) {
  auto first_node = std::make_shared<CharNode>(';', DataType::kPunctuation);
  auto second_node = std::make_shared<CharNode>(first_node->data, first_node->data_type);

  EXPECT_TRUE(first_node == second_node);
}
TEST(ASTUtilsEqualOperatorTests, EqualNodesWithDataTest) {
  auto first_float_num_node = std::make_shared<FloatNumNode>(1.0);
  auto second_float_num_node = std::make_shared<FloatNumNode>(first_float_num_node->data);
  EXPECT_TRUE(first_float_num_node == second_float_num_node);

  auto first_int_num_node = std::make_shared<IntNumNode>(1);
  auto second_int_num_node = std::make_shared<IntNumNode>(first_int_num_node->data);
  EXPECT_TRUE(first_int_num_node == second_int_num_node);

  auto first_char_node = std::make_shared<CharNode>(';', DataType::kPunctuation);
  auto second_char_node = std::make_shared<CharNode>(first_char_node->data,
                                                     first_char_node->data_type);
  EXPECT_TRUE(first_char_node == second_char_node);

  auto first_string_node = std::make_shared<StringNode>("string", DataType::kWord);
  auto second_string_node = std::make_shared<StringNode>(first_string_node->data,
                                                         first_string_node->data_type);
  EXPECT_TRUE(first_string_node == second_string_node);
}

TEST(ASTUtilsNotEqualOperatorTests, DifferentLineTest) {
  auto first_node = std::make_shared<RootNode>();
  first_node->line = 103;

  auto second_node = std::make_shared<RootNode>();
  second_node->line = first_node->line * 2;

  EXPECT_TRUE(first_node != second_node);
}
TEST(ASTUtilsNotEqualOperatorTests, DifferentStmtTypeTest) {
  auto first_node = std::make_shared<ServiceNode>(StmtType::kQuery);
  auto second_node = std::make_shared<ServiceNode>(StmtType::kCreateTableStmt);

  EXPECT_TRUE(first_node != second_node);
}
TEST(ASTUtilsNotEqualOperatorTests, DifferentDataTypeTest) {
  auto first_node = std::make_shared<CharNode>(';', DataType::kPunctuation);
  auto second_node = std::make_shared<CharNode>('(', DataType::kBracket);

  EXPECT_TRUE(first_node != second_node);
}
TEST(ASTUtilsNotEqualOperatorTests, DifferentDataTest) {
  auto first_float_num_node = std::make_shared<FloatNumNode>(1.0);
  auto second_float_num_node = std::make_shared<FloatNumNode>(3.14);
  EXPECT_TRUE(first_float_num_node != second_float_num_node);

  auto first_int_num_node = std::make_shared<IntNumNode>(1);
  auto second_int_num_node = std::make_shared<IntNumNode>(5);
  EXPECT_TRUE(first_int_num_node != second_int_num_node);

  auto first_char_node = std::make_shared<CharNode>(';', DataType::kPunctuation);
  auto second_char_node = std::make_shared<CharNode>(',', first_char_node->data_type);
  EXPECT_TRUE(first_char_node != second_char_node);

  auto first_string_node = std::make_shared<StringNode>("string", DataType::kWord);
  auto second_string_node = std::make_shared<StringNode>(first_string_node->data + "_suffix",
                                                         first_string_node->data_type);
  EXPECT_TRUE(first_string_node != second_string_node);
}
