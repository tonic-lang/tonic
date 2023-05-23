/**
 * Licensed under the Apache License, Version 2.0;
 * Please find the license in the repository .LICENSE file here:
 * https://github.com/tonic-lang/tonic/blob/main/LICENSE
 *
 * @brief Testing error message
 */

#include "gtest/gtest.h"
#include "errors.h"

using namespace tonic;

TEST(ErrorsTests, CompilerErrorMessage) {
    try {
        throw CompilerError("test prefix", "test message", 10, "test content", "test_file.tn");
        FAIL() << "Expected CompilerError to be thrown";
    } catch (const CompilerError &e) {
        std::string expected = "Error in file: test_file.tn\n"
                               "test prefix error near line 10: test message\n"
                               "Found near: test content\n\n";
        EXPECT_STREQ(expected.c_str(), e.what());
    }
}

TEST(ErrorsTests, EmptyContent) {
    try {
        throw CompilerError("test prefix", "test message", 10, "", "test_file.tn");
        FAIL() << "Expected CompilerError to be thrown";
    } catch (const CompilerError &e) {
        std::string expected = "Error in file: test_file.tn\n"
                               "test prefix error near line 10: test message\n\n";
        EXPECT_STREQ(expected.c_str(), e.what());
    }
}