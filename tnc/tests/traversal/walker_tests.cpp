/**
 * Licensed under the Apache License, Version 2.0;
 * Please find the license in the repository .LICENSE file here:
 * https://github.com/tonic-lang/tonic/blob/main/LICENSE
 *
 * @brief Tests for the AST Walker
 */

#include "gtest/gtest.h"

#include "traversal/walker.h"

using namespace tonic;

TEST(WalkerTests, VariableDeclaration) {
    Walker walker;
    bool was_called = false;

    walker.Register<VariableDeclaration>([&](std::shared_ptr<VariableDeclaration> node) {
        was_called = true;
    });

    auto node = std::make_shared<VariableDeclaration>();
    walker.Walk(node);

    EXPECT_TRUE(was_called);
}

TEST(WalkerTests, FunctionDeclaration) {
    Walker walker;
    bool was_called = false;

    walker.Register<FunctionDeclaration>([&](std::shared_ptr<FunctionDeclaration> node) {
        was_called = true;
    });

    auto node = std::make_shared<FunctionDeclaration>();
    walker.Walk(node);

    EXPECT_TRUE(was_called);
}

TEST(WalkerTests, Program) {
    Walker walker;
    bool var_decl_called = false;
    bool func_decl_called = false;

    walker.Register<VariableDeclaration>([&](std::shared_ptr<VariableDeclaration> node) {
        var_decl_called = true;
    });

    walker.Register<FunctionDeclaration>([&](std::shared_ptr<FunctionDeclaration> node) {
        func_decl_called = true;
    });

    auto program = std::make_shared<Program>();
    program->body.push_back(std::make_shared<VariableDeclaration>());
    program->body.push_back(std::make_shared<FunctionDeclaration>());

    walker.Walk(program);

    EXPECT_TRUE(var_decl_called);
    EXPECT_TRUE(func_decl_called);
}

TEST(WalkerTests, NestedNodes) {
    Walker walker;
    bool was_called = false;

    walker.Register<VariableDeclaration>([&](std::shared_ptr<VariableDeclaration> node) {
        was_called = true;
    });

    auto outer_block = std::make_shared<Block>();
    auto inner_block = std::make_shared<Block>();
    inner_block->body.push_back(std::make_shared<VariableDeclaration>());
    outer_block->body.push_back(inner_block);

    walker.Walk(outer_block);

    EXPECT_TRUE(was_called);
}

TEST(WalkerTests, IgnoresUnregisteredNodeTypes) {
    Walker walker;
    bool was_called = false;

    walker.Register<VariableDeclaration>([&](std::shared_ptr<VariableDeclaration> node) {
        was_called = true;
    });

    auto node = std::make_shared<FunctionDeclaration>();
    walker.Walk(node);

    EXPECT_FALSE(was_called);
}

TEST(WalkerTests, AllNodeTypes) {
    Walker walker;
    std::unordered_map<std::string, bool> node_types_visited;

    walker.Register<VariableDeclaration>([&](std::shared_ptr<VariableDeclaration> node) {
        node_types_visited["VariableDeclaration"] = true;
    });

    walker.Register<FunctionDeclaration>([&](std::shared_ptr<FunctionDeclaration> node) {
        node_types_visited["FunctionDeclaration"] = true;
    });

    walker.Register<Block>([&](std::shared_ptr<Block> node) {
        node_types_visited["Block"] = true;
    });

    walker.Register<GeneralStatement>([&](std::shared_ptr<GeneralStatement> node) {
        node_types_visited["GeneralStatement"] = true;
    });

    walker.Register<CppNode>([&](std::shared_ptr<CppNode> node) {
        node_types_visited["CppNode"] = true;
    });

    walker.Register<ForLoop>([&](std::shared_ptr<ForLoop> node) {
        node_types_visited["ForLoop"] = true;
    });

    walker.Register<RangedLoop>([&](std::shared_ptr<RangedLoop> node) {
        node_types_visited["RangedLoop"] = true;
    });

    walker.Register<WhileLoop>([&](std::shared_ptr<WhileLoop> node) {
        node_types_visited["WhileLoop"] = true;
    });

    walker.Register<InputOutput>([&](std::shared_ptr<InputOutput> node) {
        node_types_visited["InputOutput"] = true;
    });

    walker.Register<ClassDeclaration>([&](std::shared_ptr<ClassDeclaration> node) {
        node_types_visited["ClassDeclaration"] = true;
    });

    walker.Register<StructDeclaration>([&](std::shared_ptr<StructDeclaration> node) {
        node_types_visited["StructDeclaration"] = true;
    });

    walker.Register<NamespaceDeclaration>([&](std::shared_ptr<NamespaceDeclaration> node) {
        node_types_visited["NamespaceDeclaration"] = true;
    });

    walker.Register<TemplateDeclaration>([&](std::shared_ptr<TemplateDeclaration> node) {
        node_types_visited["TemplateDeclaration"] = true;
    });

    walker.Register<LambdaExpression>([&](std::shared_ptr<LambdaExpression> node) {
        node_types_visited["LambdaExpression"] = true;
    });

    walker.Register<ElseIfStatement>([&](std::shared_ptr<ElseIfStatement> node) {
        node_types_visited["ElseIfStatement"] = true;
    });

    walker.Register<IfStatement>([&](std::shared_ptr<IfStatement> node) {
        node_types_visited["IfStatement"] = true;
    });

    walker.Register<TryCatchStatement>([&](std::shared_ptr<TryCatchStatement> node) {
        node_types_visited["TryCatchStatement"] = true;
    });

    walker.Register<SwitchCaseStatement>([&](std::shared_ptr<SwitchCaseStatement> node) {
        node_types_visited["SwitchCaseStatement"] = true;
    });

    walker.Register<PairDestructuring>([&](std::shared_ptr<PairDestructuring> node) {
        node_types_visited["PairDestructuring"] = true;
    });

    auto program = std::make_shared<Program>();
    program->body.push_back(std::make_shared<VariableDeclaration>());
    program->body.push_back(std::make_shared<FunctionDeclaration>());
    program->body.push_back(std::make_shared<Block>());
    program->body.push_back(std::make_shared<GeneralStatement>());
    program->body.push_back(std::make_shared<CppNode>());
    program->body.push_back(std::make_shared<ForLoop>());
    program->body.push_back(std::make_shared<RangedLoop>());
    program->body.push_back(std::make_shared<WhileLoop>());
    program->body.push_back(std::make_shared<InputOutput>());
    program->body.push_back(std::make_shared<ClassDeclaration>());
    program->body.push_back(std::make_shared<StructDeclaration>());
    program->body.push_back(std::make_shared<NamespaceDeclaration>());
    program->body.push_back(std::make_shared<TemplateDeclaration>());
    program->body.push_back(std::make_shared<LambdaExpression>());
    program->body.push_back(std::make_shared<ElseIfStatement>());
    program->body.push_back(std::make_shared<IfStatement>());
    program->body.push_back(std::make_shared<TryCatchStatement>());
    program->body.push_back(std::make_shared<SwitchCaseStatement>());
    program->body.push_back(std::make_shared<PairDestructuring>());

    walker.Walk(program);

    EXPECT_TRUE(node_types_visited["VariableDeclaration"]);
    EXPECT_TRUE(node_types_visited["FunctionDeclaration"]);
    EXPECT_TRUE(node_types_visited["Block"]);
    EXPECT_TRUE(node_types_visited["GeneralStatement"]);
    EXPECT_TRUE(node_types_visited["CppNode"]);
    EXPECT_TRUE(node_types_visited["ForLoop"]);
    EXPECT_TRUE(node_types_visited["RangedLoop"]);
    EXPECT_TRUE(node_types_visited["WhileLoop"]);
    EXPECT_TRUE(node_types_visited["InputOutput"]);
    EXPECT_TRUE(node_types_visited["ClassDeclaration"]);
    EXPECT_TRUE(node_types_visited["StructDeclaration"]);
    EXPECT_TRUE(node_types_visited["NamespaceDeclaration"]);
    EXPECT_TRUE(node_types_visited["TemplateDeclaration"]);
    EXPECT_TRUE(node_types_visited["LambdaExpression"]);
    EXPECT_TRUE(node_types_visited["ElseIfStatement"]);
    EXPECT_TRUE(node_types_visited["IfStatement"]);
    EXPECT_TRUE(node_types_visited["TryCatchStatement"]);
    EXPECT_TRUE(node_types_visited["SwitchCaseStatement"]);
    EXPECT_TRUE(node_types_visited["PairDestructuring"]);
}

TEST(WalkerTests, WithoutRegisteredHandlerDoesNotThrow) {
    Walker walker;
    auto node = std::make_shared<VariableDeclaration>();
    EXPECT_NO_THROW(walker.Walk(node));
}

TEST(WalkerTests, ParentNodeIsVisitedBeforeChildNodes) {
    Walker walker;
    std::vector<std::string> visit_order;

    walker.Register<FunctionDeclaration>([&](std::shared_ptr<FunctionDeclaration> node) {
        visit_order.emplace_back("FunctionDeclaration");
    });

    walker.Register<Block>([&](std::shared_ptr<Block> node) {
        visit_order.emplace_back("Block");
    });

    auto block = std::make_shared<Block>();
    block->body.push_back(std::make_shared<FunctionDeclaration>());
    walker.Walk(block);

    ASSERT_EQ(2, visit_order.size());
    EXPECT_EQ("Block", visit_order[0]);
    EXPECT_EQ("FunctionDeclaration", visit_order[1]);
}

TEST(WalkerTests, WalkRecursivelyVisitsNodes) {
    Walker walker;
    bool visited_inner_node = false;

    walker.Register<FunctionDeclaration>([&](std::shared_ptr<FunctionDeclaration> node) {
        visited_inner_node = true;
    });

    auto outer_block = std::make_shared<Block>();
    auto inner_block = std::make_shared<Block>();
    inner_block->body.push_back(std::make_shared<FunctionDeclaration>());
    outer_block->body.push_back(inner_block);
    walker.Walk(outer_block);

    EXPECT_TRUE(visited_inner_node);
}

TEST(WalkerTests, CorrectOrderOfExecution) {
    Walker walker;
    std::vector<std::string> visit_order;

    walker.Register<VariableDeclaration>([&](std::shared_ptr<VariableDeclaration> node) {
        visit_order.emplace_back("VariableDeclaration");
    });

    walker.Register<FunctionDeclaration>([&](std::shared_ptr<FunctionDeclaration> node) {
        visit_order.emplace_back("FunctionDeclaration");
    });

    walker.Register<ForLoop>([&](std::shared_ptr<ForLoop> node) {
        visit_order.emplace_back("ForLoop");
    });

    auto program_node = std::make_shared<Program>();
    program_node->body.push_back(std::make_shared<VariableDeclaration>());
    program_node->body.push_back(std::make_shared<FunctionDeclaration>());
    program_node->body.push_back(std::make_shared<ForLoop>());

    walker.Walk(program_node);

    ASSERT_EQ(3, visit_order.size());
    EXPECT_EQ("VariableDeclaration", visit_order[0]);
    EXPECT_EQ("FunctionDeclaration", visit_order[1]);
    EXPECT_EQ("ForLoop", visit_order[2]);
}