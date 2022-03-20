#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "SCC/ast.h"

using namespace testing;

TEST(IntNumNodeTests, GetDataTest) {
  int value = 0;

  value = 0;
  std::shared_ptr<IntNumNode> node1 = std::make_shared<IntNumNode>(value);
  EXPECT_EQ(node1->get_data(), value);

  value = 5;
  std::shared_ptr<IntNumNode> node2 = std::make_shared<IntNumNode>(value);
  EXPECT_EQ(node2->get_data(), value);

  value = -5;
  std::shared_ptr<IntNumNode> node3 = std::make_shared<IntNumNode>(value);
  EXPECT_EQ(node3->get_data(), value);
}
TEST(IntNumNodeTests, SetDataTest) {
  int value = 0;
  std::shared_ptr<IntNumNode> node = std::make_shared<IntNumNode>(0);

  value = 0;
  node->set_data(value);
  EXPECT_EQ(node->get_data(), value);

  value = 9;
  node->set_data(value);
  EXPECT_EQ(node->get_data(), value);

  value = -123;
  node->set_data(value);
  EXPECT_EQ(node->get_data(), value);
}
TEST(IntNumNodeTests, InvalidConstructorTest) {
  EXPECT_EXIT(std::make_shared<IntNumNode>(5, DataType::ROOT),
              ExitedWithCode(EXIT_FAILURE),
              "")
    << "type 'ROOT' should not be accepted by type validator";

  EXPECT_EXIT(std::make_shared<IntNumNode>(5, DataType::FLOAT_NUMBER),
              ExitedWithCode(EXIT_FAILURE),
              "")
    << "type 'FLOAT_NUMBER' should not be accepted by type validator";

  EXPECT_EXIT(std::make_shared<IntNumNode>(5, DataType::BRACKET),
              ExitedWithCode(EXIT_FAILURE),
              "")
    << "type 'BRACKET' should not be accepted by type validator";

  EXPECT_EXIT(std::make_shared<IntNumNode>(5, DataType::PUNCTUATION),
              ExitedWithCode(EXIT_FAILURE),
              "")
    << "type 'PUNCTUATION' should not be accepted by type validator";

  EXPECT_EXIT(std::make_shared<IntNumNode>(5, DataType::OPERATOR),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'WORD' should not be accepted by type validator";

  EXPECT_EXIT(std::make_shared<IntNumNode>(5, DataType::WORD),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'WORD' should not be accepted by type validator";
}
TEST(IntNumNodeTests, InvalidSetTest) {
  std::shared_ptr<IntNumNode> node = std::make_shared<IntNumNode>(0);

  EXPECT_EXIT(node->set_data(5, DataType::ROOT),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'ROOT' should not be accepted by type validator";

  EXPECT_EXIT(node->set_data(5, DataType::FLOAT_NUMBER),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'FLOAT_NUMBER' should not be accepted by type validator";

  EXPECT_EXIT(node->set_data(5, DataType::BRACKET),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'FLOAT_NUMBER' should not be accepted by type validator";

  EXPECT_EXIT(node->set_data(5, DataType::PUNCTUATION),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'PUNCTUATION' should not be accepted by type validator";

  EXPECT_EXIT(node->set_data(5, DataType::OPERATOR),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'WORD' should not be accepted by type validator";

  EXPECT_EXIT(node->set_data(5, DataType::WORD),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'WORD' should not be accepted by type validator";
}

TEST(FloatNumNodeTests, GetDataTest) {
  double value = 0;

  value = 0;
  std::shared_ptr<FloatNumNode> node1 = std::make_shared<FloatNumNode>(value);
  EXPECT_EQ(node1->get_data(), value);

  value = 5;
  std::shared_ptr<FloatNumNode> node2 = std::make_shared<FloatNumNode>(value);
  EXPECT_EQ(node2->get_data(), value);

  value = 35.1234;
  std::shared_ptr<FloatNumNode> node3 = std::make_shared<FloatNumNode>(value);
  EXPECT_EQ(node3->get_data(), value);

  value = -5;
  std::shared_ptr<FloatNumNode> node4 = std::make_shared<FloatNumNode>(value);
  EXPECT_EQ(node4->get_data(), value);

  value = -34.21243;
  std::shared_ptr<FloatNumNode> node5 = std::make_shared<FloatNumNode>(value);
  EXPECT_EQ(node5->get_data(), value);
}
TEST(FloatNumNodeTests, SetDataTest) {
  double value = 0;
  std::shared_ptr<FloatNumNode> node = std::make_shared<FloatNumNode>(0);

  value = 0;
  node->set_data(value);
  EXPECT_EQ(node->get_data(), value);

  value = 9;
  node->set_data(value);
  EXPECT_EQ(node->get_data(), value);

  value = 45.1235;
  node->set_data(value);
  EXPECT_EQ(node->get_data(), value);

  value = -123;
  node->set_data(value);
  EXPECT_EQ(node->get_data(), value);

  value = -12345.12345;
  node->set_data(value);
  EXPECT_EQ(node->get_data(), value);
}
TEST(FloatNumNodeTests, InvalidConstructorTest) {
  EXPECT_EXIT(std::make_shared<FloatNumNode>(5, DataType::ROOT),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'ROOT' should not be accepted by type validator";

  EXPECT_EXIT(std::make_shared<FloatNumNode>(5, DataType::INT_NUMBER),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'FLOAT_NUMBER' should not be accepted by type validator";

  EXPECT_EXIT(std::make_shared<FloatNumNode>(5, DataType::BRACKET),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'BRACKET' should not be accepted by type validator";

  EXPECT_EXIT(std::make_shared<FloatNumNode>(5, DataType::PUNCTUATION),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'PUNCTUATION' should not be accepted by type validator";

  EXPECT_EXIT(std::make_shared<IntNumNode>(5, DataType::OPERATOR),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'WORD' should not be accepted by type validator";

  EXPECT_EXIT(std::make_shared<FloatNumNode>(5, DataType::WORD),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'WORD' should not be accepted by type validator";
}
TEST(FloatNumNodeTests, InvalidSetTest) {
  std::shared_ptr<FloatNumNode> node = std::make_shared<FloatNumNode>(0);

  EXPECT_EXIT(node->set_data(5, DataType::ROOT),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'ROOT' should not be accepted by type validator";

  EXPECT_EXIT(node->set_data(5, DataType::INT_NUMBER),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'FLOAT_NUMBER' should not be accepted by type validator";

  EXPECT_EXIT(node->set_data(5, DataType::BRACKET),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'FLOAT_NUMBER' should not be accepted by type validator";

  EXPECT_EXIT(node->set_data(5, DataType::PUNCTUATION),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'PUNCTUATION' should not be accepted by type validator";

  EXPECT_EXIT(node->set_data(5, DataType::OPERATOR),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'WORD' should not be accepted by type validator";

  EXPECT_EXIT(node->set_data(5, DataType::WORD),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'WORD' should not be accepted by type validator";
}

TEST(CharNodeTests, GetDataTest) {
  char value = 0;

  value = 0;
  std::shared_ptr<CharNode> node1 =
    std::make_shared<CharNode>(value, DataType::PUNCTUATION);
  EXPECT_EQ(node1->get_data(), value);

  value = 'a';
  std::shared_ptr<CharNode> node2 =
    std::make_shared<CharNode>(value, DataType::PUNCTUATION);
  EXPECT_EQ(node2->get_data(), value);

  value = '{';
  std::shared_ptr<CharNode> node3 =
    std::make_shared<CharNode>(value, DataType::BRACKET);
  EXPECT_EQ(node3->get_data(), value);
}
TEST(CharNodeTests, SetDataTest) {
  char value = 0;
  std::shared_ptr<CharNode> node =
    std::make_shared<CharNode>(value, DataType::PUNCTUATION);

  value = 0;
  node->set_data(value);
  EXPECT_EQ(node->get_data(), value);

  value = 'a';
  node->set_data(value);
  EXPECT_EQ(node->get_data(), value);

  value = '+';
  node->set_data(value);
  EXPECT_EQ(node->get_data(), value);
}
TEST(CharNodeTests, InvalidConstructorTest) {
  EXPECT_EXIT(std::make_shared<CharNode>(5, DataType::ROOT),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'ROOT' should not be accepted by type validator";

  EXPECT_EXIT(std::make_shared<CharNode>(5, DataType::INT_NUMBER),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'FLOAT_NUMBER' should not be accepted by type validator";

  EXPECT_EXIT(std::make_shared<CharNode>(5, DataType::FLOAT_NUMBER),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'BRACKET' should not be accepted by type validator";

  EXPECT_EXIT(std::make_shared<IntNumNode>(5, DataType::OPERATOR),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'WORD' should not be accepted by type validator";

  EXPECT_EXIT(std::make_shared<CharNode>(5, DataType::WORD),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'WORD' should not be accepted by type validator";
}
TEST(CharNodeTests, InvalidSetTest) {
  std::shared_ptr<CharNode> node =
    std::make_shared<CharNode>('a', DataType::BRACKET);

  EXPECT_EXIT(node->set_data(5, DataType::ROOT),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'ROOT' should not be accepted by type validator";

  EXPECT_EXIT(node->set_data(5, DataType::INT_NUMBER),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'FLOAT_NUMBER' should not be accepted by type validator";

  EXPECT_EXIT(node->set_data(5, DataType::FLOAT_NUMBER),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'FLOAT_NUMBER' should not be accepted by type validator";

  EXPECT_EXIT(node->set_data(5, DataType::OPERATOR),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'WORD' should not be accepted by type validator";

  EXPECT_EXIT(node->set_data(5, DataType::WORD),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'WORD' should not be accepted by type validator";
}

TEST(StringNodeTests, GetDataTest) {
  std::string value;

  value = "String\n";
  std::shared_ptr<StringNode> node1 =
    std::make_shared<StringNode>(value, DataType::WORD);
  EXPECT_EQ(node1->get_data(), value);

  value = ">=";
  std::shared_ptr<StringNode> node2 =
    std::make_shared<StringNode>(value, DataType::OPERATOR);
  EXPECT_EQ(node2->get_data(), value);

  value = 'a';
  std::shared_ptr<StringNode> node3 =
    std::make_shared<StringNode>(value, DataType::WORD);
  EXPECT_EQ(node3->get_data(), value);
}
TEST(StringNodeTests, SetDataTest) {
  std::string value;
  std::shared_ptr<StringNode> node =
    std::make_shared<StringNode>(value, DataType::WORD);

  value = "String\n";
  node->set_data(value);
  EXPECT_EQ(node->get_data(), value);

  value = "==";
  node->set_data(value, DataType::WORD);
  EXPECT_EQ(node->get_data(), value);

  value = 'a';
  node->set_data(value);
  EXPECT_EQ(node->get_data(), value);
}
TEST(StringNodeTests, InvalidConstructorTest) {
  EXPECT_EXIT(std::make_shared<CharNode>(5, DataType::ROOT),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'ROOT' should not be accepted by type validator";

  EXPECT_EXIT(std::make_shared<CharNode>(5, DataType::INT_NUMBER),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'FLOAT_NUMBER' should not be accepted by type validator";

  EXPECT_EXIT(std::make_shared<CharNode>(5, DataType::FLOAT_NUMBER),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'BRACKET' should not be accepted by type validator";

  EXPECT_EXIT(std::make_shared<FloatNumNode>(5, DataType::BRACKET),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'BRACKET' should not be accepted by type validator";

  EXPECT_EXIT(std::make_shared<FloatNumNode>(5, DataType::PUNCTUATION),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'PUNCTUATION' should not be accepted by type validator";
}
TEST(StringNodeTests, InvalidSetTest) {
  std::shared_ptr<StringNode> node =
    std::make_shared<StringNode>("string", DataType::WORD);

  EXPECT_EXIT(node->set_data("string", DataType::ROOT),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'ROOT' should not be accepted by type validator";

  EXPECT_EXIT(node->set_data("string", DataType::INT_NUMBER),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'FLOAT_NUMBER' should not be accepted by type validator";

  EXPECT_EXIT(node->set_data("string", DataType::FLOAT_NUMBER),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'FLOAT_NUMBER' should not be accepted by type validator";

  EXPECT_EXIT(node->set_data("string", DataType::BRACKET),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'FLOAT_NUMBER' should not be accepted by type validator";

  EXPECT_EXIT(node->set_data("string", DataType::PUNCTUATION),
              ExitedWithCode(EXIT_FAILURE),
              "")
          << "type 'PUNCTUATION' should not be accepted by type validator";
}

class GeneralNodeTests : public ::testing::Test {
protected:
  std::shared_ptr<RootNode> root = std::make_shared<RootNode>();
  size_t children_amount = 0;

  std::shared_ptr<IntNumNode> node0 = std::make_shared<IntNumNode>(5);
  std::shared_ptr<FloatNumNode> node1 = std::make_shared<FloatNumNode>(5);
  std::shared_ptr<CharNode> node2 =
    std::make_shared<CharNode>('{', DataType::BRACKET);
  std::shared_ptr<StringNode> node3 =
    std::make_shared<StringNode>("String", DataType::WORD);

  void SetUp() override {
    root->AddChild(node0);
    children_amount++;

    root->AddChild(node1);
    children_amount++;

    root->AddChild(node2);
    children_amount++;

    root->AddChild(node3);
    children_amount++;
  }
};
TEST_F(GeneralNodeTests, GetTypeTest) {
  std::shared_ptr<RootNode> node0 = std::make_shared<RootNode>();
  EXPECT_EQ(node0->get_type(), DataType::ROOT);

  std::shared_ptr<IntNumNode> node1 = std::make_shared<IntNumNode>(1);
  EXPECT_EQ(node1->get_type(), DataType::INT_NUMBER);

  std::shared_ptr<FloatNumNode> node2 = std::make_shared<FloatNumNode>(1.1);
  EXPECT_EQ(node2->get_type(), DataType::FLOAT_NUMBER);

  std::shared_ptr<CharNode> node3 =
    std::make_shared<CharNode>(';', DataType::PUNCTUATION);
  EXPECT_EQ(node3->get_type(), DataType::PUNCTUATION);

  std::shared_ptr<CharNode> node4 =
    std::make_shared<CharNode>(')', DataType::BRACKET);
  EXPECT_EQ(node4->get_type(), DataType::BRACKET);

  std::shared_ptr<StringNode> node5 =
    std::make_shared<StringNode>("String", DataType::WORD);
  EXPECT_EQ(node5->get_type(), DataType::WORD);

  std::shared_ptr<StringNode> node6 =
    std::make_shared<StringNode>("String", DataType::OPERATOR);
  EXPECT_EQ(node6->get_type(), DataType::OPERATOR);
}
TEST_F(GeneralNodeTests, GetChildrenAmountTest) {
  EXPECT_EQ(root->get_children_amount(), children_amount);
}
TEST_F(GeneralNodeTests, GetChildTest) {
  std::shared_ptr<Node> node;

  node = root->get_child(0);
  EXPECT_EQ(node->get_type(), node0->get_type());
  EXPECT_EQ(std::dynamic_pointer_cast<IntNumNode>(node)->get_data(),
            node0->get_data());

  node = root->get_child(1);
  EXPECT_EQ(node->get_type(), node1->get_type());
  EXPECT_EQ(std::dynamic_pointer_cast<FloatNumNode>(node)->get_data(),
            node1->get_data());

  node = root->get_child(2);
  EXPECT_EQ(node->get_type(), node2->get_type());
  EXPECT_EQ(std::dynamic_pointer_cast<CharNode>(node)->get_data(),
            node2->get_data());

  node = root->get_child(3);
  EXPECT_EQ(node->get_type(), node3->get_type());
  EXPECT_EQ(std::dynamic_pointer_cast<StringNode>(node)->get_data(),
            node3->get_data());
}
TEST_F(GeneralNodeTests, InvalidGetChildrenAmountTest) {
  EXPECT_EXIT(root->get_child(4), ExitedWithCode(EXIT_FAILURE), "")
    << "child number bigger than child amount";

  EXPECT_EXIT(root->get_child(-1), ExitedWithCode(EXIT_FAILURE), "")
    << "child number bigger than child amount";
}
