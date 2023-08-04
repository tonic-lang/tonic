/**
 * Licensed under the Apache License, Version 2.0;
 * Please find the license in the repository .LICENSE file here:
 * https://github.com/tonic-lang/tonic/blob/main/LICENSE
 *
 * @brief Implementation of the parser using a
 * recursive descent parsing approach
 */

#include "parser.h"
#include "errors/errors.h"

namespace tonic {

    Parser::Parser(const std::vector<Token> &tokens, std::string file_name)
            : tokens(tokens), current(0), file_name(std::move(file_name)) {}

    //////////////////////
    // Parser functions //
    //////////////////////

    // Upper level

    std::shared_ptr<Program> Parser::Parse() {
        auto program = std::make_shared<Program>();
        MetaError error("Parser", "Parsing failed");

        while (!CheckEnd()) {
            try {
                program->body.push_back(ParseStatement());
            } catch (const SyntaxError &e) {
                error.AddError(e.what());
                SynchronizeError();
            }
        }

        if (error.HasErrors())
            error.Throw();

        return program;
    }

    std::shared_ptr<Node> Parser::ParseStatement() {
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

    std::shared_ptr<VariableDeclaration> Parser::ParseVariableDeclaration() {
        auto variable_declaration = std::make_shared<VariableDeclaration>();

        variable_declaration->identifier = ParseGeneralStatement(1);

        if (Match(TokenType::COLON)) {
            Advance();
            variable_declaration->data_type = Advance().lexeme;
        }

        if (Match(TokenType::EQ)) {
            Advance();
            variable_declaration->declaration_type = DeclarationType::ASSIGNMENT; // TODO need to change assignment/decl in sem analysis

            if (Match(TokenType::LSQUARE)) {
                variable_declaration->initializer = ParseListComprehension();
            } else {
                variable_declaration->initializer = ParseGeneralStatement();
            }
        }

        Advance();

        return variable_declaration;
    }

    // Lower level

    std::shared_ptr<Node> Parser::ParseListComprehension() {
        auto for_loop = std::make_shared<ForLoop>(); // might need ranged loop depending on statement

        Advance();

        // use special parser

        // change this to general statement first (until for), then rbracket
        // allow multiline
        while (!Match(TokenType::RSQUARE)) {
            if (CheckEnd())
                Throw("Closing square bracket missing");

            if (Match(TokenType::FOR))


                Advance();
        }

        if (!Match(TokenType::RSQUARE))
            Throw("Closing square bracket missing");

        for_loop->operation = ParseGeneralStatement(TokenType::FOR);


        return for_loop;
    }

    std::shared_ptr<Node> Parser::ParseForStatement() {
        if (!Match(TokenType::FOR))
            Throw("For loop must begin with a \"for\" token");

        Advance();

        if (!Match(TokenType::IDENTIFIER))
            Throw("Missing variable identifier in for statement");

        std::shared_ptr<GeneralStatement> identifier = ParseGeneralStatement(1);

        std::string id_type = AUTO;
        if (Match(TokenType::COLON)) {
            Advance();

            if (!Match(TokenType::TYPE))
                Throw("Invalid type of for loop identifier");

            id_type = Advance().lexeme;
        }

        if (!Match(TokenType::IN))
            Throw("Missing \"in\" token in for statement");

        Advance();

        bool is_manual = CheckTokenInLine(TokenType::FOR_RANGE);

        if (is_manual) {
            auto for_loop = std::make_shared<ForLoop>();
            for_loop->identifier = identifier;
            for_loop->id_type = id_type;

            for_loop->start = ParseGeneralStatement(TokenType::FOR_RANGE);
            Advance();

            bool is_stepped = CheckTokenInLine(TokenType::STEP);

            if (is_stepped) {
                for_loop->end = ParseGeneralStatement(TokenType::STEP);
            } else {
                for_loop->end = ParseGeneralStatement({TokenType::COLON, TokenType::RSQUARE});
            }

            return for_loop;

        } else {
            auto ranged_loop = std::make_shared<RangedLoop>();
            ranged_loop->identifier = identifier;
            ranged_loop->id_type = id_type;

            ranged_loop->object = ParseGeneralStatement({TokenType::COLON, TokenType::RSQUARE});

            return ranged_loop;
        }
    }

    // Parsing general statements

    std::shared_ptr<GeneralStatement> Parser::ParseGeneralStatement() {
        auto general_statement = std::make_shared<GeneralStatement>();

        while (!Match(TokenType::NEWLINE) && !CheckEnd()) {
            general_statement->statement += Advance().lexeme;
            general_statement->statement += ' ';
        }

        Advance();
        return general_statement;
    }

    std::shared_ptr<GeneralStatement> Parser::ParseGeneralStatement(size_t length) {
        auto general_statement = std::make_shared<GeneralStatement>();

        for (size_t i = 0; !Match(TokenType::NEWLINE) && !CheckEnd() && i < length; i++) {
            general_statement->statement += Advance().lexeme;
            general_statement->statement += ' ';
        }

        if (Match(TokenType::NEWLINE))
            Advance();

        return general_statement;
    }

    std::shared_ptr<GeneralStatement> Parser::ParseGeneralStatement(TokenType type) {
        auto general_statement = std::make_shared<GeneralStatement>();

        while (!Match({TokenType::NEWLINE, type}) && !CheckEnd()) {
            general_statement->statement += Advance().lexeme;
            general_statement->statement += ' ';
        }

        if (Match(TokenType::NEWLINE))
            Advance();

        return general_statement;
    }

    std::shared_ptr<GeneralStatement> Parser::ParseGeneralStatement(const std::vector<TokenType> &types) {
        auto general_statement = std::make_shared<GeneralStatement>();

        while (!Match(types) && !CheckEnd()) {
            general_statement->statement += Advance().lexeme;
            general_statement->statement += ' ';
        }

        if (Match(TokenType::NEWLINE))
            Advance();

        return general_statement;
    }

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
        return PeekForward() == type;
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
        while (!CheckEnd() && !Match(TokenType::NEWLINE) && !CheckEnd()) {
            Advance();
        }
    }

    size_t Parser::CurrentLine() {
        return tokens[current].line;
    }

    void Parser::Throw(const std::string &message) {
        throw SyntaxError(message,
                          CurrentLine(),
                          Peek().lexeme,
                          file_name);
    }

    bool Parser::CheckTokenInLine(TokenType type) {
        size_t i = current;

        while (i >= tokens.size() && tokens[i] != TokenType::NEWLINE && tokens[i] != TokenType::EOF_TOKEN) {
            if (tokens[i] == type)
                return true;

            ++i;
        }

        return false;
    }

}