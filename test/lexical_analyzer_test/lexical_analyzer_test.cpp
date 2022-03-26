#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "SCC/lexical_analyzer.h"

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
public:
  Tokenizer tTokenizer;
};

TEST_F(TokenizerTests, IntNumTest) {
  config.set_sql_path(config.GetConfigPath() +
                      "../resources/lexical_analyzer_test/int_num_test.sql");
  config.Start();

  tTokenizer.Tokenize();

  EXPECT_EQ(51241235, (std::static_pointer_cast<IntNumNode>
    (tTokenizer.get_first_token()))->get_data());
}

//TEST_F(TokenizerTests, FloatNumTest) { //TODO: fix cypher opening file
//  config.set_sql_path(config.GetConfigPath() +
//                      "../resources/lexical_analyzer_test/float_num_test.sql");
//  config.Start();
//
//  tTokenizer.Tokenize();
//
//  EXPECT_DOUBLE_EQ(34623458.13249874081, (std::static_pointer_cast<FloatNumNode>
//    (tTokenizer.get_first_token()))->get_data());
//}
