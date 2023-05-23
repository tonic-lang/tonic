/**
 * Licensed under the Apache License, Version 2.0;
 * Please find the license in the repository .LICENSE file here:
 * https://github.com/tonic-lang/tonic/blob/main/LICENSE
 *
 * @brief Parses tokens into an AST for each file using a
 * recursive descent parsing algorithm
 */

#ifndef TONIC_PARSER_H
#define TONIC_PARSER_H

#include <memory>

#include "lexer.h"
#include "ast.h"

namespace tonic {

    class Parser {
    public:
        explicit Parser(const std::vector<Token> &tokens);

        std::unique_ptr<Program> Parse();

    private:
        // parsers
        // upper level
        std::unique_ptr<Node> ParseStatement();

        std::unique_ptr<GeneralStatement> ParseGeneralStatement();

        std::unique_ptr<VariableDeclaration> ParseVariableDeclaration();

        std::unique_ptr<FunctionDeclaration> ParseFunctionDeclaration();

        std::unique_ptr<Node> ParseForLoop();

        std::unique_ptr<WhileLoop> ParseWhileLoop();

        std::unique_ptr<InputOutput> ParseInputOutput();

        std::unique_ptr<ClassDeclaration> ParseClassDeclaration();

        std::unique_ptr<StructDeclaration> ParseStructDeclaration();

        std::unique_ptr<NamespaceDeclaration> ParseNamespaceDeclaration();

        std::unique_ptr<TemplateDeclaration> ParseTemplateDeclaration();

        std::unique_ptr<IfStatement> ParseIfStatement();

        std::unique_ptr<TryCatchStatement> ParseTryCatchStatement();

        std::unique_ptr<SwitchCaseStatement> ParseSwitchCaseStatement();

        std::unique_ptr<CppNode> ParseCppNode();

        // lower level
        std::unique_ptr<Block> ParseBlock();

        std::unique_ptr<LambdaExpression> ParseLambdaExpression();

        std::unique_ptr<PairDestructuring> ParsePairDestructuring();

        std::unique_ptr<Node> ParseListComprehension();

        // helpers
        bool Match(TokenType type);

        bool Match(const std::vector<TokenType> &types);

        bool MatchForward(TokenType type);

        Token Advance();

        bool CheckEnd();

        Token Peek();

        Token PeekForward();

        Token Previous();

        void SynchronizeError();

        std::string GenerateStringUntil(TokenType type);

        std::vector<std::string> errors;
        const std::vector<Token> &tokens;
        size_t current;
    };

}

#endif //TONIC_PARSER_H
