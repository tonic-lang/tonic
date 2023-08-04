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
#include "core/ast.h"

namespace tonic {

    class Parser {
    public:
        explicit Parser(const std::vector<Token> &tokens, std::string file_name);

        std::shared_ptr<Program> Parse();

    private:
        // parsers
        // upper level
        std::shared_ptr<Node> ParseStatement();

        // general statements
        std::shared_ptr<GeneralStatement> ParseGeneralStatement();

        std::shared_ptr<GeneralStatement> ParseGeneralStatement(size_t length);

        std::shared_ptr<GeneralStatement> ParseGeneralStatement(TokenType type);

        std::shared_ptr<GeneralStatement> ParseGeneralStatement(const std::vector<TokenType> &types);

        std::shared_ptr<VariableDeclaration> ParseVariableDeclaration();

        std::shared_ptr<FunctionDeclaration> ParseFunctionDeclaration();

        std::shared_ptr<Node> ParseForLoop();

        std::shared_ptr<WhileLoop> ParseWhileLoop();

        std::shared_ptr<InputOutput> ParseInputOutput();

        std::shared_ptr<ClassDeclaration> ParseClassDeclaration();

        std::shared_ptr<StructDeclaration> ParseStructDeclaration();

        std::shared_ptr<NamespaceDeclaration> ParseNamespaceDeclaration();

        std::shared_ptr<TemplateDeclaration> ParseTemplateDeclaration();

        std::shared_ptr<IfStatement> ParseIfStatement();

        std::shared_ptr<TryCatchStatement> ParseTryCatchStatement();

        std::shared_ptr<SwitchCaseStatement> ParseSwitchCaseStatement();

        std::shared_ptr<CppNode> ParseCppNode();

        // lower level
        std::shared_ptr<Block> ParseBlock();

        std::shared_ptr<LambdaExpression> ParseLambdaExpression();

        std::shared_ptr<PairDestructuring> ParsePairDestructuring();

        std::shared_ptr<Node> ParseListComprehension();

        std::shared_ptr<Node> ParseForStatement();

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

        size_t CurrentLine();

        void Throw(const std::string& message);

        bool CheckTokenInLine(TokenType type);

        const std::vector<Token> &tokens;
        size_t current;
        std::string file_name;
    };

}

#endif //TONIC_PARSER_H
