#include <cstdio>

#include "gtest/gtest.h"

#include "SCC/lexer/symbol_classifier.h"

using namespace scc::lexer;
using namespace testing;

TEST(SymbolClassifierTest, IsSpaceTest) {
  EXPECT_TRUE(SymbolClassifier::IsSpace(' '));
  EXPECT_TRUE(SymbolClassifier::IsSpace('\n'));
  EXPECT_TRUE(SymbolClassifier::IsSpace('\t'));
  EXPECT_TRUE(SymbolClassifier::IsSpace('\v'));

  EXPECT_FALSE(SymbolClassifier::IsSpace('h'));
  EXPECT_FALSE(SymbolClassifier::IsSpace(','));
}

TEST(SymbolClassifierTest, IsDigitTest) {
  for (char c = '0'; c <= '9'; c++) {
    EXPECT_TRUE(SymbolClassifier::IsDigit(c));
  }

  EXPECT_FALSE(SymbolClassifier::IsDigit('a'));
  EXPECT_FALSE(SymbolClassifier::IsDigit('O'));
  EXPECT_FALSE(SymbolClassifier::IsDigit('l'));
}

TEST(SymbolClassifierTest, IsAlphaTest) {
  for (char c = 'a'; c <= 'z'; ++c) {
    EXPECT_TRUE(SymbolClassifier::IsAlpha(c));
  }
  for (char c = 'A'; c <= 'Z'; ++c) {
    EXPECT_TRUE(SymbolClassifier::IsAlpha(c));
  }

  EXPECT_FALSE(SymbolClassifier::IsAlpha('3'));
  EXPECT_FALSE(SymbolClassifier::IsAlpha('@'));
  EXPECT_FALSE(SymbolClassifier::IsAlpha('0'));
}

TEST(SymbolClassifierTest, IsOperatorTest) {
  EXPECT_TRUE(SymbolClassifier::IsOperator('+'));
  EXPECT_TRUE(SymbolClassifier::IsOperator('-'));
  EXPECT_TRUE(SymbolClassifier::IsOperator('*'));
  EXPECT_TRUE(SymbolClassifier::IsOperator('/'));
  EXPECT_TRUE(SymbolClassifier::IsOperator('^'));
  EXPECT_TRUE(SymbolClassifier::IsOperator('!'));
  EXPECT_TRUE(SymbolClassifier::IsOperator('='));
  EXPECT_TRUE(SymbolClassifier::IsOperator('<'));
  EXPECT_TRUE(SymbolClassifier::IsOperator('>'));

  EXPECT_FALSE(SymbolClassifier::IsOperator('#'));
  EXPECT_FALSE(SymbolClassifier::IsOperator('5'));
  EXPECT_FALSE(SymbolClassifier::IsOperator('m'));
}

TEST(SymbolClassifierTest, IsBracketTest) {
  EXPECT_TRUE(SymbolClassifier::IsBracket('('));
  EXPECT_TRUE(SymbolClassifier::IsBracket(')'));
  EXPECT_TRUE(SymbolClassifier::IsBracket('{'));
  EXPECT_TRUE(SymbolClassifier::IsBracket('}'));
  EXPECT_TRUE(SymbolClassifier::IsBracket('['));
  EXPECT_TRUE(SymbolClassifier::IsBracket(']'));

  EXPECT_FALSE(SymbolClassifier::IsBracket('a'));
  EXPECT_FALSE(SymbolClassifier::IsBracket('6'));
  EXPECT_FALSE(SymbolClassifier::IsBracket('/'));
}

TEST(SymbolClassifierTest, IsPunctuationTest) {
  EXPECT_TRUE(SymbolClassifier::IsPunctuation(':'));
  EXPECT_TRUE(SymbolClassifier::IsPunctuation(';'));
  EXPECT_TRUE(SymbolClassifier::IsPunctuation('.'));
  EXPECT_TRUE(SymbolClassifier::IsPunctuation(','));
  EXPECT_TRUE(SymbolClassifier::IsPunctuation('\''));
  EXPECT_TRUE(SymbolClassifier::IsPunctuation('\"'));

  EXPECT_FALSE(SymbolClassifier::IsPunctuation('e'));
  EXPECT_FALSE(SymbolClassifier::IsPunctuation('0'));
  EXPECT_FALSE(SymbolClassifier::IsPunctuation('$'));
}

TEST(SymbolClassifierTest, IsNullTerminatorTest) {
  EXPECT_TRUE(SymbolClassifier::IsNullTerminator('\0'));

  EXPECT_FALSE(SymbolClassifier::IsNullTerminator('4'));
  EXPECT_FALSE(SymbolClassifier::IsNullTerminator('a'));
  EXPECT_FALSE(SymbolClassifier::IsNullTerminator('?'));
}

TEST(SymbolClassifierTest, IsEOFTest) {
  EXPECT_TRUE(SymbolClassifier::IsEOF(EOF));

  EXPECT_FALSE(SymbolClassifier::IsEOF('\0'));
  EXPECT_FALSE(SymbolClassifier::IsEOF('!'));
  EXPECT_FALSE(SymbolClassifier::IsEOF('0'));
}

TEST(SymbolClassifierTest, DefineSymbolTypeTest) {
  EXPECT_EQ(SymbolClassifier::DefineSymbolType(' '), SymbolType::kSpace);
  EXPECT_EQ(SymbolClassifier::DefineSymbolType('5'), SymbolType::kDigit);
  EXPECT_EQ(SymbolClassifier::DefineSymbolType('a'), SymbolType::kAlpha);
  EXPECT_EQ(SymbolClassifier::DefineSymbolType('+'), SymbolType::kOperator);
  EXPECT_EQ(SymbolClassifier::DefineSymbolType('('), SymbolType::kBracket);
  EXPECT_EQ(SymbolClassifier::DefineSymbolType(';'), SymbolType::kPunctuation);
  EXPECT_EQ(SymbolClassifier::DefineSymbolType('\0'), SymbolType::kNullTerminator);
  EXPECT_EQ(SymbolClassifier::DefineSymbolType(EOF), SymbolType::kEOF);
  EXPECT_EQ(SymbolClassifier::DefineSymbolType('$'), SymbolType::kUnknown);
}
