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
    const std::string SHIFT_LEFT_STR = "<<";
    const std::string SHIFT_RIGHT_STR = ">>";
    const std::string FOR_DOTS = "..";
    const std::string MEMOIZE_TAG = "@memoize";
    const std::string ARROW_STR = "=>";

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

        // other operators
        SHIFT_RIGHT,
        SHIFT_LEFT,

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
        CONSTEXPR,
        SIZEOF,

        // preprocessor directives
        HASHTAG,

        // cpp directive
        CPP_DIRECTIVE,

        // second pass
        TYPE,
        FOR_RANGE,
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

        void HandleHex();

        void HandleString();

        void HandleCharacter();

        void HandlePreprocessor();

        bool HandleTemplateExpression();

        bool CheckMemoize(size_t i);

        bool CheckLambda(size_t i);

        bool CheckConst(size_t i);

        bool CheckConstexpr(size_t i);

        bool CheckIdentifier(size_t i);

        bool CheckForRange(size_t i);

        bool CheckShiftLeft(size_t i);

        bool CheckShiftRight(size_t i);

        bool CheckType(size_t i);

        bool CheckSemicolon(size_t i);

        std::string source;
        std::vector<Token> first_pass_tokens;
        size_t first_pass_start;
        size_t first_pass_current;
        size_t first_pass_line;
        size_t first_pass_indentation_level;
        std::vector<size_t> indent_stack;
    };

}

#endif //TONIC_LEXER_H
