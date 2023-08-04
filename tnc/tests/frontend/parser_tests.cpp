/**
 * Licensed under the Apache License, Version 2.0;
 * Please find the license in the repository .LICENSE file here:
 * https://github.com/tonic-lang/tonic/blob/main/LICENSE
 *
 * @brief Tests for parser
 */

#include "parser.h"
#include "gtest/gtest.h"

TEST(ParserTests, VariableDeclaration) {
    
}

TEST(ParserTests, ManualForStatement) {
    std::string statement = "for i in 0..20";
}

TEST(ParserTests, RangedForStatement) {
    std::string statement = "for o in vec";
}

// const, constexpr variables or const functions too
// destructor, constructor