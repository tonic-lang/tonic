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

    tonic::Lexer lexer(code);
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

    tonic::Lexer lexer(code);
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

    tonic::Lexer lexer(code);
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

    tonic::Lexer lexer(code);
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

    tonic::Lexer lexer(code);
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

    tonic::Lexer lexer(code);
    std::vector<tonic::Token> tokens = lexer.Tokenize();

    ASSERT_EQ(expected.size(), tokens.size()) << "Expected and result token sizes are different";

    for (int i = 0; i < tokens.size(); i++) {
        ASSERT_EQ(expected[i], tokens[i].type) << "Expected and result token is different at " << i << " and lexeme: "
                                               << tokens[i].lexeme;
    }
}

TEST(LexerTests, Ranges) {
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

    tonic::Lexer lexer(code);
    std::vector<tonic::Token> tokens = lexer.Tokenize();

    ASSERT_EQ(expected.size(), tokens.size()) << "Expected and result token sizes are different";

    for (int i = 0; i < tokens.size(); i++) {
        ASSERT_EQ(expected[i], tokens[i].type) << "Expected and result token is different at " << i << " and lexeme: "
                                               << tokens[i].lexeme;
    }
}

TEST(LexerTests, FotRange) {
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

    tonic::Lexer lexer(code);
    std::vector<tonic::Token> tokens = lexer.Tokenize();

    ASSERT_EQ(expected.size(), tokens.size()) << "Expected and result token sizes are different";

    for (int i = 0; i < tokens.size(); i++) {
        ASSERT_EQ(expected[i], tokens[i].type) << "Expected and result token is different at " << i << " and lexeme: "
                                               << tokens[i].lexeme;
    }
}