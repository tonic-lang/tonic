/**
 * Licensed under the Apache License, Version 2.0;
 * Please find the license in the repository .LICENSE file here:
 * https://github.com/tonic-lang/tonic/blob/main/LICENSE
 *
 * @brief Implementation for the Walker class to be used
 * heavily in semantic analysis
 */

#include "traversal/walker.h"

namespace tonic {

    Walker::Walker() = default;

    void Walker::Walk(const NodePtr &node) {
        if (!node)
            return;

        // executing lambda if registered
        auto handler = handlers.find(typeid(*node).name());
        if (handler != handlers.end()) {
            handler->second(node);
        }

        visited_nodes.push(node);

        // walking through sub-nodes
        auto program_node = std::dynamic_pointer_cast<Program>(node);
        if (program_node) {
            WalkVector(program_node->body);
        }

        auto block_node = std::dynamic_pointer_cast<Block>(node);
        if (block_node) {
            WalkVector(block_node->body);
        }

        auto cpp_node = std::dynamic_pointer_cast<CppNode>(node);
        if (cpp_node) {
            Walk(cpp_node->cpp_code);
        }

        auto variable_declaration_node = std::dynamic_pointer_cast<VariableDeclaration>(node);
        if (variable_declaration_node) {
            Walk(variable_declaration_node->identifier);
            Walk(variable_declaration_node->initializer);
        }

        auto function_declaration_node = std::dynamic_pointer_cast<FunctionDeclaration>(node);
        if (function_declaration_node) {
            Walk(function_declaration_node->type);
            Walk(function_declaration_node->name);
            Walk(function_declaration_node->block);
        }

        auto for_loop_node = std::dynamic_pointer_cast<ForLoop>(node);
        if (for_loop_node) {
            Walk(for_loop_node->identifier);
            Walk(for_loop_node->start);
            Walk(for_loop_node->end);
            Walk(for_loop_node->step);
            Walk(for_loop_node->operation);
            Walk(for_loop_node->block);
        }

        auto ranged_loop_node = std::dynamic_pointer_cast<RangedLoop>(node);
        if (ranged_loop_node) {
            Walk(ranged_loop_node->identifier);
            Walk(ranged_loop_node->object);
            Walk(ranged_loop_node->operation);
            Walk(ranged_loop_node->block);
        }

        auto while_loop_node = std::dynamic_pointer_cast<WhileLoop>(node);
        if (while_loop_node) {
            Walk(while_loop_node->condition);
            Walk(while_loop_node->block);
        }

        auto in_out_node = std::dynamic_pointer_cast<InputOutput>(node);
        if (in_out_node) {
            WalkVector(in_out_node->operands);
        }

        auto class_declaration_node = std::dynamic_pointer_cast<ClassDeclaration>(node);
        if (class_declaration_node) {
            Walk(class_declaration_node->declaration);
            Walk(class_declaration_node->block);
        }

        auto struct_declaration_node = std::dynamic_pointer_cast<StructDeclaration>(node);
        if (struct_declaration_node) {
            Walk(struct_declaration_node->declaration);
            Walk(struct_declaration_node->block);
        }

        auto namespace_declaration_node = std::dynamic_pointer_cast<NamespaceDeclaration>(node);
        if (namespace_declaration_node) {
            Walk(namespace_declaration_node->namespace_name);
            Walk(namespace_declaration_node->block);
        }

        auto template_node = std::dynamic_pointer_cast<TemplateDeclaration>(node);
        if (template_node) {
            Walk(template_node->template_statement);
            Walk(template_node->content);
        }

        auto lambda_expression_node = std::dynamic_pointer_cast<LambdaExpression>(node);
        if (lambda_expression_node) {
            Walk(lambda_expression_node->capture_clause);
            Walk(lambda_expression_node->body);
        }

        auto if_statement_node = std::dynamic_pointer_cast<IfStatement>(node);
        if (if_statement_node) {
            Walk(if_statement_node->condition);
            Walk(if_statement_node->true_block);
            WalkVector(if_statement_node->else_if_statements);
            Walk(if_statement_node->else_block);
        }

        auto try_catch_statement_node = std::dynamic_pointer_cast<TryCatchStatement>(node);
        if (try_catch_statement_node) {
            Walk(try_catch_statement_node->try_block);
            Walk(try_catch_statement_node->catch_block);
        }

        auto switch_case_statement_node = std::dynamic_pointer_cast<SwitchCaseStatement>(node);
        if (switch_case_statement_node) {
            Walk(switch_case_statement_node->condition);
            for (const auto& case_pair : switch_case_statement_node->cases) {
                Walk(case_pair.first);
                Walk(case_pair.second);
            }
            Walk(switch_case_statement_node->default_case);
        }

        auto pair_destructuring_node = std::dynamic_pointer_cast<PairDestructuring>(node);
        if (pair_destructuring_node) {
            Walk(pair_destructuring_node->initializer);
        }

        visited_nodes.pop();
    }

}