/**
 * Licensed under the Apache License, Version 2.0;
 * Please find the license in the repository .LICENSE file here:
 * https://github.com/tonic-lang/tonic/blob/main/LICENSE
 *
 * @brief Implementation of the parser using a
 * recursive descent parsing approach
 */

// TODO check memory safety for pointers
// TODO errors:
// - invalid arguments in function declaration (too many identifiers etc, or not starting with id)
// - invalid argument pass (in analyzers maybe)
// - curly bracket without constructor (type before) or quick representation of vectors, etc

// TODO make a listener linked with advance for errors like #3

#include "parser.h"
#include "errors.h"

namespace tonic {

    Parser::Parser(const std::vector<Token> &tokens) : tokens(tokens), current(0) {}

    //////////////////////
    // Parser functions //
    //////////////////////

    // Upper level

    std::unique_ptr<Program> Parser::Parse() {
        auto program = std::make_unique<Program>();

        while (!CheckEnd()) {
            try {
                program->body.push_back(ParseStatement());
            } catch (const CompilerError &e) {
                errors.emplace_back(e.what());
                SynchronizeError();
            }
        }

        return program;
    }

    std::unique_ptr<Node> Parser::ParseStatement() {
        if (Match(TokenType::IF)) {
            return ParseIfStatement();
        } else if (Match(TokenType::IDENTIFIER)) {
            return ParseVariableDeclaration(); // calls list comprehension, lambda expression, pair destructuring parse functions, and handles constructors
        } else if (Match(TokenType::TYPE)) {
            return ParseFunctionDeclaration();
        } else if (Match(TokenType::FOR)) {
            return ParseForLoop(); // for both normal for loop and ranged loop
        } else if (Match(TokenType::WHILE)) {
            return ParseWhileLoop();
        } else if (Match({TokenType::IN, TokenType::OUT})) {
            return ParseInputOutput();
        } else if (Match(TokenType::CLASS)) {
            return ParseClassDeclaration();
        } else if (Match(TokenType::STRUCT)) {
            return ParseStructDeclaration();
        } else if (Match(TokenType::NAMESPACE)) {
            return ParseNamespaceDeclaration();
        } else if (Match(TokenType::TEMPLATE)) {
            return ParseTemplateDeclaration();
        } else if (Match(TokenType::TRY)) {
            return ParseTryCatchStatement();
        } else if (Match(TokenType::SWITCH)) {
            return ParseSwitchCaseStatement();
        } else if (Match(TokenType::CPP_CHUNK)) {
            return ParseCppNode();
        } else {
            return ParseGeneralStatement(); // should not allow some restricted tokens at the start that are dependent on other terms (e.g. catch, else, else if, case, default)
        }
    }

    // Lower level

    //////////////////////
    // Helper functions //
    //////////////////////

    bool Parser::Match(TokenType type) {
        return !CheckEnd() && Peek() == type;
    }

    bool Parser::Match(const std::vector<TokenType> &types) {
        bool result = false;
        for (const TokenType &type: types) {
            result = result || Peek() == type;
        }
        return result;
    }

    bool Parser::MatchForward(TokenType type) {
        if (!CheckEnd())
            return Peek() == type;
        return false;
    }

    Token Parser::Advance() {
        if (!CheckEnd())
            ++current;
        return Previous();
    }

    bool Parser::CheckEnd() {
        return current >= tokens.size() || tokens[current] == TokenType::EOF_TOKEN;
    }

    Token Parser::Peek() {
        return tokens[current];
    }

    Token Parser::PeekForward() {
        if (current >= tokens.size())
            throw InternalError("Cannot go beyond the end of the token vector");
        return tokens[current + 1];
    }

    Token Parser::Previous() {
        if (current <= 0)
            throw InternalError("Cannot go previous when current index is already 0 in parser");
        return tokens[current - 1];
    }

    void Parser::SynchronizeError() {
        while (!CheckEnd() && !Match({TokenType::NEWLINE, TokenType::EOF_TOKEN})) {
            Advance();
        }
        Advance();
    }

    std::string Parser::GenerateStringUntil(TokenType type) {
        std::string result;
        while (!CheckEnd() && !Match(type)) {
            result += Advance().lexeme;
            result += ' ';
        }
        return result;
    }

}