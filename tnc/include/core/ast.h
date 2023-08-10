/**
 * Licensed under the Apache License, Version 2.0;
 * Please find the license in the repository .LICENSE file here:
 * https://github.com/tonic-lang/tonic/blob/main/LICENSE
 *
 * @brief Abstract Syntax Tree nodes for the Tonic language
 */

#ifndef TONIC_AST_H
#define TONIC_AST_H

#include <vector>
#include <string>
#include <memory>

namespace tonic {

    const std::string AUTO = "auto";

    enum class DeclarationType {
        DECLARATION,
        ASSIGNMENT,
    };

    enum class InOut {
        IN,
        OUT,
    };

    struct Node {
        virtual ~Node() = default;
    };

    struct Program : Node {
        std::vector<std::shared_ptr<Node>> body;
    };

    struct GeneralStatement : Node {
        std::string statement;

        void RemoveLast() {
            if (!statement.empty()) {
                statement.pop_back();
            }
        }
    };

    struct CppNode : Node {
        std::shared_ptr<GeneralStatement> cpp_code;

        CppNode() : cpp_code(nullptr) {}
    };

    struct Block : Node {
        std::vector<std::shared_ptr<Node>> body;
    };

    struct VariableDeclaration : Node {
        std::string data_type;
        DeclarationType declaration_type;
        std::shared_ptr<GeneralStatement> identifier;
        std::shared_ptr<Node> initializer;

        VariableDeclaration() : data_type(AUTO), declaration_type(DeclarationType::DECLARATION), identifier(nullptr),
                                initializer(nullptr) {}
    };

    struct FunctionDeclaration : Node {
        bool is_memoize;
        std::shared_ptr<GeneralStatement> type;
        std::shared_ptr<GeneralStatement> name;
        std::vector<std::pair<std::string, std::string>> arguments; // identifier, type
        std::shared_ptr<Block> block;

        FunctionDeclaration() : is_memoize(false), type(nullptr), name(nullptr), block(nullptr) {}
    };

    struct ForLoop : Node {
        std::string id_type;
        std::shared_ptr<GeneralStatement> identifier;
        std::shared_ptr<GeneralStatement> start;
        std::shared_ptr<GeneralStatement> end;
        std::shared_ptr<GeneralStatement> step;
        std::shared_ptr<GeneralStatement> operation; // for list comprehension
        std::shared_ptr<Block> block;

        ForLoop() : id_type(AUTO), identifier(nullptr), start(nullptr), end(nullptr), step(nullptr), operation(nullptr) {}
    };

    struct RangedLoop : Node {
        std::string id_type;
        std::shared_ptr<GeneralStatement> identifier;
        std::shared_ptr<GeneralStatement> object;
        std::shared_ptr<GeneralStatement> operation; // for list comprehension
        std::shared_ptr<Block> block;

        RangedLoop() : id_type(AUTO), identifier(nullptr), object(nullptr), operation(nullptr), block(nullptr) {}
    };

    struct WhileLoop : Node {
        std::shared_ptr<GeneralStatement> condition;
        std::shared_ptr<Block> block;

        WhileLoop() : condition(nullptr), block(nullptr) {}
    };

    struct InputOutput : Node { // array-like input is in the form of a statement
        InOut type;
        std::vector<std::shared_ptr<GeneralStatement>> operands;

        InputOutput() : type(InOut::IN) {}
    };

    struct ClassDeclaration : Node {
        std::shared_ptr<GeneralStatement> declaration;
        std::shared_ptr<Block> block;

        ClassDeclaration() : declaration(nullptr), block(nullptr) {}
    };

    struct StructDeclaration : Node {
        std::shared_ptr<GeneralStatement> declaration;
        std::shared_ptr<Block> block;

        StructDeclaration() : declaration(nullptr), block(nullptr) {}
    };

    struct NamespaceDeclaration : Node {
        std::shared_ptr<GeneralStatement> namespace_name;
        std::shared_ptr<Block> block;

        NamespaceDeclaration() : namespace_name(nullptr), block(nullptr) {}
    };

    struct TemplateDeclaration : Node {
        std::shared_ptr<GeneralStatement> template_statement;
        std::vector<std::pair<std::string, std::string>> arguments;
        std::shared_ptr<Node> content; // could be a function, class, or struct

        TemplateDeclaration() : template_statement(nullptr), content(nullptr) {}
    };

    struct LambdaExpression : Node {
        std::vector<std::pair<std::string, std::string>> arguments;
        std::shared_ptr<GeneralStatement> capture_clause;
        std::shared_ptr<Node> body; // could be an expression or a block

        LambdaExpression() : capture_clause(nullptr), body(nullptr) {}
    };

    struct ElseIfStatement : Node {
        std::shared_ptr<GeneralStatement> condition;
        std::shared_ptr<Block> block;

        ElseIfStatement() : condition(nullptr), block(nullptr) {}
    };

    struct IfStatement : Node {
        std::shared_ptr<GeneralStatement> condition;
        std::shared_ptr<Block> true_block;
        std::vector<std::shared_ptr<ElseIfStatement>> else_if_statements; // nullptr if there is no else if
        std::shared_ptr<Block> else_block; // nullptr if there is no else

        IfStatement() : condition(nullptr), true_block(nullptr), else_block(nullptr) {}
    };

    struct TryCatchStatement : Node {
        std::shared_ptr<Block> try_block;
        std::vector<std::pair<std::string, std::string>> catch_arguments;
        std::shared_ptr<Block> catch_block;

        TryCatchStatement() : try_block(nullptr), catch_block(nullptr) {}
    };

    struct SwitchCaseStatement : Node {
        std::shared_ptr<GeneralStatement> condition;
        std::vector<std::pair<std::shared_ptr<GeneralStatement>, std::shared_ptr<Block>>> cases;
        std::shared_ptr<Block> default_case; // nullptr if there is no default case

        SwitchCaseStatement() : condition(nullptr), default_case(nullptr) {}
    };

    struct PairDestructuring : Node {
        std::string first_var;
        std::string second_var;
        std::shared_ptr<GeneralStatement> initializer;

        PairDestructuring() : initializer(nullptr) {}
    };

}

#endif //TONIC_AST_H
