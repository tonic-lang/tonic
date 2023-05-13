#ifndef TONIC_LEXER_H
#define TONIC_LEXER_H

#include <cctype>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace tonic {

    const std::string CPP_TAG = "#cpp";
    const std::string END_TAG = "#end";

    enum class TokenType {
        // basic
        IDENTIFIER,
        LITERAL,
        COMMENT,

        // formatting
        NEWLINE,
        INDENT,
        DEDENT,

        // symbols
        COLON,
        SEMICOLON,
        COMMA,
        DOT,
        SLASH,
        QMARK,
        EXCLAMATION,
        EQ,
        PLUS,
        MINUS,
        STAR,
        PERCENT,
        AMPERSAND,
        BAR,
        CARET,
        ARROW,
        AT,
        GT,
        LT,

        // brackets
        LCURLY,
        RCURLY,
        LSQUARE,
        RSQUARE,
        LPAREN,
        RPAREN,

        // keywords
        IF,
        ELSE_IF,
        ELSE,
        FOR,
        IN,
        OUT,
        RETURN,
        SWITCH,
        CASE,
        CLASS,
        STRUCT,
        PUBLIC,
        PRIVATE,
        PROTECTED,
        TRY,
        CATCH,
        THROW,
        TRACE,
        TEMPLATE,
        TYPENAME,
        USING,
        NAMESPACE,
        OPERATOR,
        DEFAULT,
        WHILE,
        BREAK,
        CONST,

        // preprocessor directives
        HASHTAG,
        CPP,
        END,

        // cpp directive
        CPP_DIRECTIVE,

        // second pass
        TYPE,
        FOR_RANGE,
        SLICE_RANGE,
        MEMOIZE,
        LAMBDA,
        CPP_CHUNK,

        // end of file
        EOF_TOKEN,
    };

    class Token {
    public:
        TokenType type;
        std::string lexeme;
        int line;

        Token(TokenType type, std::string lexeme, int line)
                : type(type), lexeme(std::move(lexeme)), line(line) {}

        friend std::ostream &operator<<(std::ostream &os, const Token &token) {
            os << token.lexeme;
            return os;
        }

        bool operator==(const TokenType &otherType) const {
            return type == otherType;
        }
    };

    class Lexer {
    public:
        explicit Lexer(std::string source);

        std::vector<Token> FirstPass();

        std::vector<Token> SecondPass();

        std::vector<Token> Tokenize();

    private:
        void AddToken(TokenType type, const std::string &text);

        void SkipWhitespace();

        void HandleComment(bool multiline);

        void HandleIndentation();

        void HandleKeywords();

        void HandleIdentifiers();

        void HandleNumbers();

        void HandleString();

        void HandleCharacter();

        void HandlePreprocessor();

        bool HandleTemplateExpression();

        std::string source;
        std::vector<Token> tokens;
        size_t first_pass_start;
        size_t first_pass_current;
        size_t first_pass_line;
        size_t first_pass_indentation_level;
        std::vector<size_t> indent_stack;
    };

}

#endif //TONIC_LEXER_H
