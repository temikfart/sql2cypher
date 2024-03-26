#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "SCC/lexer/lexer.h"

using namespace testing;

TEST(CheckFunctionsTests, IsCharacterFromArrayTest) {
  std::string array = "{}()[]";

  EXPECT_TRUE(Tokenizer::IsCharacterFromArray('(', array));
  EXPECT_FALSE(Tokenizer::IsCharacterFromArray('a', array));

  array = "abcde";
  EXPECT_TRUE(Tokenizer::IsCharacterFromArray('c', array));
  EXPECT_FALSE(Tokenizer::IsCharacterFromArray('t', array));
}
TEST(CheckFunctionsTests, IsOperatorTest) {
  EXPECT_TRUE(Tokenizer::IsOperator('+'));
  EXPECT_FALSE(Tokenizer::IsOperator('a'));
}
TEST(CheckFunctionsTests, IsBracketTest) {
  EXPECT_TRUE(Tokenizer::IsBracket('('));
  EXPECT_FALSE(Tokenizer::IsBracket('g'));
}
TEST(CheckFunctionsTests, IsPunctuationTest) {
  EXPECT_TRUE(Tokenizer::IsPunctuation(','));
  EXPECT_FALSE(Tokenizer::IsPunctuation('a'));
}

class TokenizerTests : public ::testing::Test {
protected:
  Tokenizer tTokenizer;

  void TearDown() override {
    config.CloseInputFile();
    config.CloseOutputFile();
    config.CloseTreeDumpFile();
  }
};
TEST_F(TokenizerTests, IntNumTest) {
  config.set_sql_file(config.GetConfigPath() +
                      "../../resources/lexical_analyzer_test/int_num_test.sql");
  config.Start(1, nullptr);

  tTokenizer.Tokenize();

  std::shared_ptr<IntNumNode> node = std::make_shared<IntNumNode>(51241235);

  EXPECT_TRUE(Node::IsNodesEqual(node, tTokenizer.get_first_token()));
}
TEST_F(TokenizerTests, FloatNumTest) {
  config.set_sql_file(config.GetConfigPath() +
                      "../../resources/lexical_analyzer_test/float_num_test.sql");
  config.Start(1, nullptr);

  tTokenizer.Tokenize();

  std::shared_ptr<FloatNumNode> node =
      std::make_shared<FloatNumNode>(34623458.13249874081);

  EXPECT_TRUE(Node::IsNodesEqual(node, tTokenizer.get_first_token()));
}
TEST_F(TokenizerTests, CharNodeTest) {
  config.set_sql_file(config.GetConfigPath() +
                      "../../resources/lexical_analyzer_test/char_test.sql");
  config.Start(1, nullptr);

  tTokenizer.Tokenize();

  std::shared_ptr<CharNode> node0 =
      std::make_shared<CharNode>('{', DataType::BRACKET);
  EXPECT_TRUE(Node::IsNodesEqual(node0, tTokenizer.get_first_token()));

  std::shared_ptr<CharNode> node1 =
      std::make_shared<CharNode>('}', DataType::BRACKET);
  EXPECT_TRUE(Node::IsNodesEqual(node1, tTokenizer.get_first_token()));

  std::shared_ptr<CharNode> node2 =
      std::make_shared<CharNode>(',', DataType::PUNCTUATION);
  EXPECT_TRUE(Node::IsNodesEqual(node2, tTokenizer.get_first_token()));

  std::shared_ptr<CharNode> node3 =
      std::make_shared<CharNode>(';', DataType::PUNCTUATION);
  EXPECT_TRUE(Node::IsNodesEqual(node3, tTokenizer.get_first_token()));

  std::shared_ptr<CharNode> node4 =
      std::make_shared<CharNode>('.', DataType::PUNCTUATION);
  EXPECT_TRUE(Node::IsNodesEqual(node4, tTokenizer.get_first_token()));
}
TEST_F(TokenizerTests, StringNodeTest) {
  config.set_sql_file(config.GetConfigPath() +
                      "../../resources/lexical_analyzer_test/string_test.sql");
  config.Start(1, nullptr);

  tTokenizer.Tokenize();

  std::shared_ptr<StringNode> node0 =
      std::make_shared<StringNode>("TheString_12", DataType::WORD);
  EXPECT_TRUE(Node::IsNodesEqual(node0, tTokenizer.get_first_token()));

  std::shared_ptr<StringNode> node1 =
      std::make_shared<StringNode>("=", DataType::OPERATOR);
  EXPECT_TRUE(Node::IsNodesEqual(node1, tTokenizer.get_first_token()));

  std::shared_ptr<StringNode> node2 =
      std::make_shared<StringNode>(">=", DataType::OPERATOR);
  EXPECT_TRUE(Node::IsNodesEqual(node2, tTokenizer.get_first_token()));

  std::shared_ptr<StringNode> node3 =
      std::make_shared<StringNode>("^", DataType::OPERATOR);
  EXPECT_TRUE(Node::IsNodesEqual(node3, tTokenizer.get_first_token()));
}
TEST_F(TokenizerTests, InvalidTest) {
  config.set_sql_file(config.GetConfigPath() +
                      "../../resources/lexical_analyzer_test/invalid_test.sql");
  config.Start(1, nullptr);

  EXPECT_EXIT(tTokenizer.Tokenize(), ExitedWithCode(EXIT_FAILURE) , "")
    << "№ is an invalid character";
}
TEST_F(TokenizerTests, LineTest) {
  config.set_sql_file(config.GetConfigPath() +
                      "../../resources/lexical_analyzer_test/line_test.sql");
  config.Start(1, nullptr);

  tTokenizer.Tokenize();

  EXPECT_EQ(1, tTokenizer.get_first_token()->get_line());
  EXPECT_EQ(2, tTokenizer.get_first_token()->get_line());
  EXPECT_EQ(4, tTokenizer.get_first_token()->get_line());
}

