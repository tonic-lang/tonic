/**
 * Licensed under the Apache License, Version 2.0;
 * Please find the license in the repository .LICENSE file here:
 * https://github.com/tonic-lang/tonic/blob/main/LICENSE
 *
 * @brief Token types and token class for lexer, analyzers and parser
 */

#ifndef TONIC_TOKENS_H
#define TONIC_TOKENS_H

#include <string>

namespace tonic {
    const std::string CPP_TAG = "#cpp";
    const std::string END_TAG = "#end";
    const std::string SHIFT_LEFT_STR = "<<";
    const std::string SHIFT_RIGHT_STR = ">>";
    const std::string FOR_DOTS = "..";
    const std::string MEMOIZE_TAG = "@memoize";
    const std::string ARROW_STR = "=>";
    const std::string ENUM_CLASS_STR = "enum class";

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
        DELETE,
        ENUM_CLASS,
        ENUM,

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
}

#endif //TONIC_TOKENS_H
