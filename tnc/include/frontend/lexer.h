/**
 * Licensed under the Apache License, Version 2.0;
 * Please find the license in the repository .LICENSE file here:
 * https://github.com/tonic-lang/tonic/blob/main/LICENSE
 *
 * @brief Tokenizes Tonic source code
 */

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

#include "tokens.h"

namespace tonic {

    class Lexer {
    public:
        explicit Lexer(std::string source, std::string file_name);

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

        bool CheckEnumClass(size_t i);

        std::string get_last_first_token();

        const std::string file_name;
        std::string source;
        std::vector<Token> first_pass_tokens;
        std::vector<Token> tokens;
        size_t first_pass_start;
        size_t first_pass_current;
        size_t first_pass_line;
        size_t first_pass_indentation_level;
        std::vector<size_t> indent_stack;
    };

}

#endif //TONIC_LEXER_H
