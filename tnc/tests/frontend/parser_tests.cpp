/**
 * Licensed under the Apache License, Version 2.0;
 * Please find the license in the repository .LICENSE file here:
 * https://github.com/tonic-lang/tonic/blob/main/LICENSE
 *
 * @brief Tests for parser
 */

#include "parser.h"
#include "gtest/gtest.h"

using namespace tonic;

const std::string file = "test.tn";

TEST(ParserTests, ManualForStatementDecleration) {
    std::string statement = "for i in 0..20:";

    Lexer l(statement, file);
    std::vector<Token> tokens = l.Tokenize();

    Parser p(tokens, file);
    std::shared_ptr<Node> node = p.ParseForStatement();

    auto for_node = std::dynamic_pointer_cast<ForLoop>(node);
    ASSERT_NE(for_node, nullptr);

    ASSERT_EQ(for_node->identifier->statement, "i");
    ASSERT_EQ(for_node->operation, nullptr);
    ASSERT_EQ(for_node->start->statement, "0");
    ASSERT_EQ(for_node->end->statement, "20");
    ASSERT_EQ(for_node->id_type, "auto");
}

TEST(ParserTests, RangedForStatementDecleration) {
    std::string statement = "for o in vec";

    Lexer l(statement, file);
    std::vector<Token> tokens = l.Tokenize();

    Parser p(tokens, file);
    std::shared_ptr<Node> node = p.ParseForStatement();

    auto ranged_node = std::dynamic_pointer_cast<RangedLoop>(node);
    ASSERT_NE(ranged_node, nullptr);

    ASSERT_EQ(ranged_node->identifier->statement, "o");
    ASSERT_EQ(ranged_node->operation, nullptr);
    ASSERT_EQ(ranged_node->object->statement, "vec");
    ASSERT_EQ(ranged_node->id_type, "auto");
}

TEST(ParserTests, ManualForStatementWithType) {
    std::string statement = "for i: int in 0..20:";

    Lexer l(statement, file);
    std::vector<Token> tokens = l.Tokenize();

    Parser p(tokens, file);
    std::shared_ptr<Node> node = p.ParseForStatement();

    auto for_node = std::dynamic_pointer_cast<ForLoop>(node);
    ASSERT_NE(for_node, nullptr);

    ASSERT_EQ(for_node->identifier->statement, "i");
    ASSERT_EQ(for_node->operation, nullptr);
    ASSERT_EQ(for_node->start->statement, "0");
    ASSERT_EQ(for_node->end->statement, "20");
    ASSERT_EQ(for_node->id_type, "int");
}

TEST(ParserTests, ManualRangedStatementWithType) {
    std::string statement = "for o: int& in vec";

    Lexer l(statement, file);
    std::vector<Token> tokens = l.Tokenize();

    Parser p(tokens, file);
    std::shared_ptr<Node> node = p.ParseForStatement();

    auto ranged_node = std::dynamic_pointer_cast<RangedLoop>(node);
    ASSERT_NE(ranged_node, nullptr);

    ASSERT_EQ(ranged_node->identifier->statement, "o");
    ASSERT_EQ(ranged_node->operation, nullptr);
    ASSERT_EQ(ranged_node->object->statement, "vec");
    ASSERT_EQ(ranged_node->id_type, "int&");
}
