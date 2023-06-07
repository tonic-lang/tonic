/**
 * Licensed under the Apache License, Version 2.0;
 * Please find the license in the repository .LICENSE file here:
 * https://github.com/tonic-lang/tonic/blob/main/LICENSE
 *
 * @brief Tests for the tokenizer functionality
 */

#include "lexer.h"
#include "gtest/gtest.h"

using tt = tonic::TokenType;

TEST(LexerTests, Function) {
    std::string code = "// comment here\n"
                       "int sum(a, b: int):\n"
                       "  return a + b\n";

    std::vector<tt> expected = {
            tt::COMMENT,
            tt::NEWLINE,
            tt::TYPE,
            tt::IDENTIFIER,
            tt::LPAREN,
            tt::IDENTIFIER,
            tt::COMMA,
            tt::IDENTIFIER,
            tt::COLON,
            tt::TYPE,
            tt::RPAREN,
            tt::COLON,
            tt::NEWLINE,
            tt::INDENT,
            tt::RETURN,
            tt::IDENTIFIER,
            tt::PLUS,
            tt::IDENTIFIER,
            tt::NEWLINE,
            tt::DEDENT,
            tt::EOF_TOKEN,
    };

    tonic::Lexer lexer(code, "test.tn");
    std::vector<tonic::Token> tokens = lexer.Tokenize();

    ASSERT_EQ(expected.size(), tokens.size()) << "Expected and result token sizes are different";

    for (int i = 0; i < tokens.size(); i++) {
        ASSERT_EQ(expected[i], tokens[i].type) << "Expected and result token is different at " << i << " and lexeme: "
                                               << tokens[i].lexeme;
    }
}

TEST(LexerTests, MemberFunctionImpl) {
    std::string code = "// switch\n"
                       "int Test::sum(a, b: int):\n"
                       "  return a + b\n";

    std::vector<tt> expected = {
            tt::COMMENT,
            tt::NEWLINE,
            tt::TYPE,
            tt::IDENTIFIER,
            tt::LPAREN,
            tt::IDENTIFIER,
            tt::COMMA,
            tt::IDENTIFIER,
            tt::COLON,
            tt::TYPE,
            tt::RPAREN,
            tt::COLON,
            tt::NEWLINE,
            tt::INDENT,
            tt::RETURN,
            tt::IDENTIFIER,
            tt::PLUS,
            tt::IDENTIFIER,
            tt::NEWLINE,
            tt::DEDENT,
            tt::EOF_TOKEN,
    };

    tonic::Lexer lexer(code, "test.tn");
    std::vector<tonic::Token> tokens = lexer.Tokenize();

    ASSERT_EQ(expected.size(), tokens.size()) << "Expected and result token sizes are different";

    for (int i = 0; i < tokens.size(); i++) {
        ASSERT_EQ(expected[i], tokens[i].type) << "Expected and result token is different at " << i << " and lexeme: "
                                               << tokens[i].lexeme;
    }
}

TEST(LexerTests, ClassDefinition) {
    std::string code = "class MyClass:\n"
                       "private:\n"
                       "  a: int\n"
                       "  b = 250\n"
                       "public:\n"
                       "  MyClass(a: int):\n"
                       "    this.a = a\n";

    std::vector<tt> expected = {
            tt::CLASS,
            tt::IDENTIFIER,
            tt::COLON,
            tt::NEWLINE,
            tt::PRIVATE,
            tt::COLON,
            tt::NEWLINE,
            tt::INDENT,
            tt::IDENTIFIER,
            tt::COLON,
            tt::TYPE,
            tt::NEWLINE,
            tt::IDENTIFIER,
            tt::EQ,
            tt::LITERAL,
            tt::NEWLINE,
            tt::DEDENT,
            tt::PUBLIC,
            tt::COLON,
            tt::NEWLINE,
            tt::INDENT,
            tt::IDENTIFIER,
            tt::LPAREN,
            tt::IDENTIFIER,
            tt::COLON,
            tt::TYPE,
            tt::RPAREN,
            tt::COLON,
            tt::NEWLINE,
            tt::INDENT,
            tt::IDENTIFIER,
            tt::DOT,
            tt::IDENTIFIER,
            tt::EQ,
            tt::IDENTIFIER,
            tt::NEWLINE,
            tt::DEDENT,
            tt::DEDENT,
            tt::EOF_TOKEN
    };

    tonic::Lexer lexer(code, "test.tn");
    std::vector<tonic::Token> tokens = lexer.Tokenize();

    ASSERT_EQ(expected.size(), tokens.size()) << "Expected and result token sizes are different";

    for (int i = 0; i < tokens.size(); i++) {
        ASSERT_EQ(expected[i], tokens[i].type) << "Expected and result token is different at " << i << " and lexeme: "
                                               << tokens[i].lexeme;
    }
}

TEST(LexerTests, IfStatement) {
    std::string code = "if a > b:\n"
                       "  i = 50\n"
                       "    while c < d:\n"
                       "      c = c + 1\n"
                       "else:\n"
                       "  a = b\n";

    std::vector<tt> expected = {
            tt::IF,
            tt::IDENTIFIER,
            tt::GT,
            tt::IDENTIFIER,
            tt::COLON,
            tt::NEWLINE,
            tt::INDENT,
            tt::IDENTIFIER,
            tt::EQ,
            tt::LITERAL,
            tt::NEWLINE,
            tt::INDENT,
            tt::WHILE,
            tt::IDENTIFIER,
            tt::LT,
            tt::IDENTIFIER,
            tt::COLON,
            tt::NEWLINE,
            tt::INDENT,
            tt::IDENTIFIER,
            tt::EQ,
            tt::IDENTIFIER,
            tt::PLUS,
            tt::LITERAL,
            tt::NEWLINE,
            tt::DEDENT,
            tt::DEDENT,
            tt::DEDENT,
            tt::ELSE,
            tt::COLON,
            tt::NEWLINE,
            tt::INDENT,
            tt::IDENTIFIER,
            tt::EQ,
            tt::IDENTIFIER,
            tt::NEWLINE,
            tt::DEDENT,
            tt::EOF_TOKEN
    };

    tonic::Lexer lexer(code, "test.tn");
    std::vector<tonic::Token> tokens = lexer.Tokenize();

    ASSERT_EQ(expected.size(), tokens.size()) << "Expected and result token sizes are different";

    for (int i = 0; i < tokens.size(); i++) {
        ASSERT_EQ(expected[i], tokens[i].type) << "Expected and result token is different at " << i << " and lexeme: "
                                               << tokens[i].lexeme;
    }
}

TEST(LexerTests, CppChunk) {
    std::string code = "i = 20\n"
                       "#cpp\n"
                       "int j = 50;\n"
                       "int k = j + 10.20;\n"
                       "#end\n"
                       "vector<int> vi\n"
                       "vi.push_back(j)";

    std::vector<tt> expected = {
            tt::IDENTIFIER,
            tt::EQ,
            tt::LITERAL,
            tt::NEWLINE,
            tt::CPP_CHUNK,
            tt::NEWLINE,
            tt::TYPE,
            tt::IDENTIFIER,
            tt::NEWLINE,
            tt::IDENTIFIER,
            tt::DOT,
            tt::IDENTIFIER,
            tt::LPAREN,
            tt::IDENTIFIER,
            tt::RPAREN,
            tt::EOF_TOKEN
    };

    tonic::Lexer lexer(code, "test.tn");
    std::vector<tonic::Token> tokens = lexer.Tokenize();

    ASSERT_EQ(expected.size(), tokens.size()) << "Expected and result token sizes are different";

    for (int i = 0; i < tokens.size(); i++) {
        ASSERT_EQ(expected[i], tokens[i].type) << "Expected and result token is different at " << i << " and lexeme: "
                                               << tokens[i].lexeme;
    }
}

TEST(LexerTests, SwitchCase) {
    std::string code = "try:\n"
                       "  obj.member = something\n"
                       "catch e: const exception&:\n"
                       "  out e.what()";

    std::vector<tt> expected = {
            tt::TRY,
            tt::COLON,
            tt::NEWLINE,
            tt::INDENT,
            tt::IDENTIFIER,
            tt::DOT,
            tt::IDENTIFIER,
            tt::EQ,
            tt::IDENTIFIER,
            tt::NEWLINE,
            tt::DEDENT,
            tt::CATCH,
            tt::IDENTIFIER,
            tt::COLON,
            tt::TYPE,
            tt::COLON,
            tt::NEWLINE,
            tt::INDENT,
            tt::OUT,
            tt::IDENTIFIER,
            tt::DOT,
            tt::IDENTIFIER,
            tt::LPAREN,
            tt::RPAREN,
            tt::EOF_TOKEN
    };

    tonic::Lexer lexer(code, "test.tn");
    std::vector<tonic::Token> tokens = lexer.Tokenize();

    ASSERT_EQ(expected.size(), tokens.size()) << "Expected and result token sizes are different";

    for (int i = 0; i < tokens.size(); i++) {
        ASSERT_EQ(expected[i], tokens[i].type) << "Expected and result token is different at " << i << " and lexeme: "
                                               << tokens[i].lexeme;
    }
}

TEST(LexerTests, SliceRange) {
    std::string code = "arr[2:5]";

    std::vector<tt> expected = {
            tt::IDENTIFIER,
            tt::LSQUARE,
            tt::LITERAL,
            tt::COLON,
            tt::LITERAL,
            tt::RSQUARE,
            tt::EOF_TOKEN
    };

    tonic::Lexer lexer(code, "test.tn");
    std::vector<tonic::Token> tokens = lexer.Tokenize();

    ASSERT_EQ(expected.size(), tokens.size()) << "Expected and result token sizes are different";

    for (int i = 0; i < tokens.size(); i++) {
        ASSERT_EQ(expected[i], tokens[i].type) << "Expected and result token is different at " << i << " and lexeme: "
                                               << tokens[i].lexeme;
    }
}

TEST(LexerTests, ForRange) {
    std::string code = "for i in 0..20:\n"
                       "  out i\n"
                       "for i in start..end:\n"
                       "  out i";

    std::vector<tt> expected = {
            tt::FOR,
            tt::IDENTIFIER,
            tt::IN,
            tt::LITERAL,
            tt::FOR_RANGE,
            tt::LITERAL,
            tt::COLON,
            tt::NEWLINE,
            tt::INDENT,
            tt::OUT,
            tt::IDENTIFIER,
            tt::NEWLINE,
            tt::DEDENT,
            tt::FOR,
            tt::IDENTIFIER,
            tt::IN,
            tt::IDENTIFIER,
            tt::FOR_RANGE,
            tt::IDENTIFIER,
            tt::COLON,
            tt::NEWLINE,
            tt::INDENT,
            tt::OUT,
            tt::IDENTIFIER,
            tt::EOF_TOKEN
    };

    tonic::Lexer lexer(code, "test.tn");
    std::vector<tonic::Token> tokens = lexer.Tokenize();

    ASSERT_EQ(expected.size(), tokens.size()) << "Expected and result token sizes are different";

    for (int i = 0; i < tokens.size(); i++) {
        ASSERT_EQ(expected[i], tokens[i].type) << "Expected and result token is different at " << i << " and lexeme: "
                                               << tokens[i].lexeme;
    }
}

TEST(LexerTests, ShiftsAndI0) {
    std::string code = "ofstream ostrm(filename, ios::binary)\n"
                       "ostrm.write(reinterpret_cast<char*>(&d), sizeof d)\n"
                       "ostrm << 123 << \"abc\" << '\\n'\n"
                       "a: int\n"
                       "in >> a\n"
                       "out << \"output: \" << a << '\\n'";

    std::vector<tt> expected = {
            tt::TYPE,
            tt::IDENTIFIER,
            tt::LPAREN,
            tt::IDENTIFIER,
            tt::COMMA,
            tt::IDENTIFIER,
            tt::RPAREN,
            tt::NEWLINE,
            tt::IDENTIFIER,
            tt::DOT,
            tt::IDENTIFIER,
            tt::LPAREN,
            tt::IDENTIFIER,
            tt::LPAREN,
            tt::AMPERSAND,
            tt::IDENTIFIER,
            tt::RPAREN,
            tt::COMMA,
            tt::SIZEOF,
            tt::IDENTIFIER,
            tt::RPAREN,
            tt::NEWLINE,
            tt::IDENTIFIER,
            tt::SHIFT_LEFT,
            tt::LITERAL,
            tt::SHIFT_LEFT,
            tt::LITERAL,
            tt::SHIFT_LEFT,
            tt::LITERAL,
            tt::NEWLINE,
            tt::IDENTIFIER,
            tt::COLON,
            tt::TYPE,
            tt::NEWLINE,
            tt::IN,
            tt::SHIFT_RIGHT,
            tt::IDENTIFIER,
            tt::NEWLINE,
            tt::OUT,
            tt::SHIFT_LEFT,
            tt::LITERAL,
            tt::SHIFT_LEFT,
            tt::IDENTIFIER,
            tt::SHIFT_LEFT,
            tt::LITERAL,
            tt::EOF_TOKEN
    };

    tonic::Lexer lexer(code, "test.tn");
    std::vector<tonic::Token> tokens = lexer.Tokenize();

    ASSERT_EQ(expected.size(), tokens.size()) << "Expected and result token sizes are different";

    for (int i = 0; i < tokens.size(); i++) {
        ASSERT_EQ(expected[i], tokens[i].type) << "Expected and result token is different at " << i << " and lexeme: "
                                               << tokens[i].lexeme;
    }
}

TEST(LexerTests, HexLiteral) {
    std::string code = "a = 0x1A";

    std::vector<tt> expected = {
            tt::IDENTIFIER,
            tt::EQ,
            tt::LITERAL,
            tt::EOF_TOKEN
    };

    tonic::Lexer lexer(code, "test.tn");
    std::vector<tonic::Token> tokens = lexer.Tokenize();

    ASSERT_EQ(expected.size(), tokens.size()) << "Expected and result token sizes are different";

    for (int i = 0; i < tokens.size(); i++) {
        ASSERT_EQ(expected[i], tokens[i].type) << "Expected and result token is different at " << i << " and lexeme: "
                                               << tokens[i].lexeme;
    }
}

TEST(LexerTests, ConstReturn) {
    std::string code = "const int foo(a, b: string):\n"
                       "  return a + b";

    std::vector<tt> expected = {
            tt::TYPE,
            tt::IDENTIFIER,
            tt::LPAREN,
            tt::IDENTIFIER,
            tt::COMMA,
            tt::IDENTIFIER,
            tt::COLON,
            tt::TYPE,
            tt::RPAREN,
            tt::COLON,
            tt::NEWLINE,
            tt::INDENT,
            tt::RETURN,
            tt::IDENTIFIER,
            tt::PLUS,
            tt::IDENTIFIER,
            tt::EOF_TOKEN
    };

    tonic::Lexer lexer(code, "test.tn");
    std::vector<tonic::Token> tokens = lexer.Tokenize();

    ASSERT_EQ(expected.size(), tokens.size()) << "Expected and result token sizes are different";

    for (int i = 0; i < tokens.size(); i++) {
        ASSERT_EQ(expected[i], tokens[i].type) << "Expected and result token is different at " << i << " and lexeme: "
                                               << tokens[i].lexeme;
    }
}

TEST(LexerTests, ConstFunction) {
    std::string code = "int foo(a, b: string) const:\n"
                       "  return a + b";

    std::vector<tt> expected = {
            tt::TYPE,
            tt::IDENTIFIER,
            tt::LPAREN,
            tt::IDENTIFIER,
            tt::COMMA,
            tt::IDENTIFIER,
            tt::COLON,
            tt::TYPE,
            tt::RPAREN,
            tt::CONST,
            tt::COLON,
            tt::NEWLINE,
            tt::INDENT,
            tt::RETURN,
            tt::IDENTIFIER,
            tt::PLUS,
            tt::IDENTIFIER,
            tt::EOF_TOKEN
    };

    tonic::Lexer lexer(code, "test.tn");
    std::vector<tonic::Token> tokens = lexer.Tokenize();

    ASSERT_EQ(expected.size(), tokens.size()) << "Expected and result token sizes are different";

    for (int i = 0; i < tokens.size(); i++) {
        ASSERT_EQ(expected[i], tokens[i].type) << "Expected and result token is different at " << i << " and lexeme: "
                                               << tokens[i].lexeme;
    }
}

TEST(LexerTests, MultilineParenthesis) {
    std::string code = "int foo(a,\n"
                       "        b: const int&\n"
                       "):\n"
                       "  return a + b";

    std::vector<tt> expected = {
            tt::TYPE,
            tt::IDENTIFIER,
            tt::LPAREN,
            tt::IDENTIFIER,
            tt::COMMA,
            tt::NEWLINE,
            tt::INDENT,
            tt::IDENTIFIER,
            tt::COLON,
            tt::TYPE,
            tt::NEWLINE,
            tt::DEDENT,
            tt::RPAREN,
            tt::COLON,
            tt::NEWLINE,
            tt::INDENT,
            tt::RETURN,
            tt::IDENTIFIER,
            tt::PLUS,
            tt::IDENTIFIER,
            tt::EOF_TOKEN
    };

    tonic::Lexer lexer(code, "test.tn");
    std::vector<tonic::Token> tokens = lexer.Tokenize();

    ASSERT_EQ(expected.size(), tokens.size()) << "Expected and result token sizes are different";

    for (int i = 0; i < tokens.size(); i++) {
        ASSERT_EQ(expected[i], tokens[i].type) << "Expected and result token is different at " << i << " and lexeme: "
                                               << tokens[i].lexeme;
    }
}

TEST(LexerTests, MultilineSquare) {
    std::string code = "v = [2, 5, 7,\n"
                       "  40]";

    std::vector<tt> expected = {
            tt::IDENTIFIER,
            tt::EQ,
            tt::LSQUARE,
            tt::LITERAL,
            tt::COMMA,
            tt::LITERAL,
            tt::COMMA,
            tt::LITERAL,
            tt::COMMA,
            tt::NEWLINE,
            tt::INDENT,
            tt::LITERAL,
            tt::RSQUARE,
            tt::EOF_TOKEN
    };

    tonic::Lexer lexer(code, "test.tn");
    std::vector<tonic::Token> tokens = lexer.Tokenize();

    ASSERT_EQ(expected.size(), tokens.size()) << "Expected and result token sizes are different";

    for (int i = 0; i < tokens.size(); i++) {
        ASSERT_EQ(expected[i], tokens[i].type) << "Expected and result token is different at " << i << " and lexeme: "
                                               << tokens[i].lexeme;
    }
}

TEST(LexerTests, MultilineCurly) {
    std::string code = "d = {'a': \"\",\n"
                       "  'b': \"something\"}\n"
                       "v = vector<int>{2,\n"
                       "       8}";

    std::vector<tt> expected = {
            tt::IDENTIFIER,
            tt::EQ,
            tt::LCURLY,
            tt::LITERAL,
            tt::COLON,
            tt::LITERAL,
            tt::COMMA,
            tt::NEWLINE,
            tt::INDENT,
            tt::LITERAL,
            tt::COLON,
            tt::LITERAL,
            tt::RCURLY,
            tt::NEWLINE,
            tt::DEDENT,
            tt::IDENTIFIER,
            tt::EQ,
            tt::IDENTIFIER,
            tt::LCURLY,
            tt::LITERAL,
            tt::COMMA,
            tt::NEWLINE,
            tt::INDENT,
            tt::LITERAL,
            tt::RCURLY,
            tt::EOF_TOKEN
    };

    tonic::Lexer lexer(code, "test.tn");
    std::vector<tonic::Token> tokens = lexer.Tokenize();

    ASSERT_EQ(expected.size(), tokens.size()) << "Expected and result token sizes are different";

    for (int i = 0; i < tokens.size(); i++) {
        ASSERT_EQ(expected[i], tokens[i].type) << "Expected and result token is different at " << i << " and lexeme: "
                                               << tokens[i].lexeme;
    }
}

TEST(LexerTests, Enums) {
    std::string code = "enum class Test:\n"
                       "  TEST1,\n"
                       "  TEST2\n"
                       "\n"
                       "enum Color:\n"
                       "  RED = 0,\n"
                       "  BLUE = 1,";

    std::vector<tt> expected = {
            tt::ENUM_CLASS,
            tt::IDENTIFIER,
            tt::COLON,
            tt::NEWLINE,
            tt::INDENT,
            tt::IDENTIFIER,
            tt::COMMA,
            tt::NEWLINE,
            tt::IDENTIFIER,
            tt::NEWLINE,
            tt::NEWLINE,
            tt::DEDENT,
            tt::ENUM,
            tt::IDENTIFIER,
            tt::COLON,
            tt::NEWLINE,
            tt::INDENT,
            tt::IDENTIFIER,
            tt::EQ,
            tt::LITERAL,
            tt::COMMA,
            tt::NEWLINE,
            tt::IDENTIFIER,
            tt::EQ,
            tt::LITERAL,
            tt::COMMA,
            tt::EOF_TOKEN
    };

    tonic::Lexer lexer(code, "test.tn");
    std::vector<tonic::Token> tokens = lexer.Tokenize();

    ASSERT_EQ(expected.size(), tokens.size()) << "Expected and result token sizes are different";

    for (int i = 0; i < tokens.size(); i++) {
        ASSERT_EQ(expected[i], tokens[i].type) << "Expected and result token is different at " << i << " and lexeme: "
                                               << tokens[i].lexeme;
    }
}

TEST(LexerTests, NestedLoops) {
    std::string code = "for a, b, c: int in arr, 0..10, 0..obj.foo():\n"
                       "  out a << b << c";

    std::vector<tt> expected = {
            tt::FOR,
            tt::IDENTIFIER,
            tt::COMMA,
            tt::IDENTIFIER,
            tt::COMMA,
            tt::IDENTIFIER,
            tt::COLON,
            tt::TYPE,
            tt::IN,
            tt::IDENTIFIER,
            tt::COMMA,
            tt::LITERAL,
            tt::FOR_RANGE,
            tt::LITERAL,
            tt::COMMA,
            tt::LITERAL,
            tt::FOR_RANGE,
            tt::IDENTIFIER,
            tt::DOT,
            tt::IDENTIFIER,
            tt::LPAREN,
            tt::RPAREN,
            tt::COLON,
            tt::NEWLINE,
            tt::INDENT,
            tt::OUT,
            tt::IDENTIFIER,
            tt::SHIFT_LEFT,
            tt::IDENTIFIER,
            tt::SHIFT_LEFT,
            tt::IDENTIFIER,
            tt::EOF_TOKEN,
    };

    tonic::Lexer lexer(code, "test.tn");
    std::vector<tonic::Token> tokens = lexer.Tokenize();

    ASSERT_EQ(expected.size(), tokens.size()) << "Expected and result token sizes are different";

    for (int i = 0; i < tokens.size(); i++) {
        ASSERT_EQ(expected[i], tokens[i].type) << "Expected and result token is different at " << i << " and lexeme: "
                                               << tokens[i].lexeme;
    }
}