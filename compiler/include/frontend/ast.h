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
    const std::string BLANK = "";

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
        std::vector<std::unique_ptr<Node>> body;
    };

    struct GeneralStatement : Node { // direct C++ statement: function calls, operators, ..
        std::string statement;
    };

    struct CppNode : Node {
        std::unique_ptr<GeneralStatement> cpp_code;
    };

    struct Block : Node {
        std::vector<std::unique_ptr<Node>> body;
    };

    struct VariableDeclaration : Node {
        DeclarationType declaration_type;
        std::string data_type = AUTO;
        std::string identifier;
        std::unique_ptr<Node> initializer;
    };

    struct FunctionDeclaration : Node {
        bool is_memoize = false;
        bool is_template = false;
        std::string type;
        std::string name;
        std::unique_ptr<GeneralStatement> template_statement;
        std::vector<std::pair<std::string, std::string>> arguments; // identifier, type
        std::unique_ptr<Block> block;
    };

    struct ForLoop : Node {
        std::string identifier;
        std::unique_ptr<GeneralStatement> start;
        std::unique_ptr<GeneralStatement> end;
        std::unique_ptr<GeneralStatement> step;
        std::unique_ptr<GeneralStatement> operation = nullptr; // for list comprehension
        std::unique_ptr<Block> block;
    };

    struct RangedLoop : Node {
        std::string type = AUTO; // optional type
        std::string identifier;
        std::unique_ptr<GeneralStatement> object;
        std::unique_ptr<GeneralStatement> operation = nullptr; // for list comprehension
        std::unique_ptr<Block> block;
    };

    struct WhileLoop : Node {
        std::unique_ptr<GeneralStatement> condition;
        std::unique_ptr<Block> block;
    };

    struct InputOutput : Node { // array-like input is in the form of a statement
        InOut type;
        std::vector<std::unique_ptr<GeneralStatement>> operands;
    };

    struct ClassDeclaration : Node {
        bool is_template = false;
        std::unique_ptr<GeneralStatement> template_statement;
        std::unique_ptr<Block> block;
    };

    struct StructDeclaration : Node {
        bool is_template = false;
        std::unique_ptr<GeneralStatement> template_statement;
        std::unique_ptr<Block> block;
    };

}

#endif //TONIC_AST_H
