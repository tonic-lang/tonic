/**
 * Licensed under the Apache License, Version 2.0;
 * Please find the license in the repository .LICENSE file here:
 * https://github.com/tonic-lang/tonic/blob/main/LICENSE
 *
 * @brief All relevant errors for the Tonic compiler
 */

#ifndef TONIC_ERRORS_H
#define TONIC_ERRORS_H

#include <stdexcept>
#include <sstream>

namespace tonic {

    class CompilerError : public std::runtime_error {
    public:
        CompilerError(const std::string &prefix, const std::string &message, int line, int column,
                      const std::string &line_content)
                : std::runtime_error(MessageBuilder(prefix, message, line, column, line_content)) {}

    private:
        static std::string MessageBuilder(const std::string &prefix, const std::string &message, int line, int column,
                                          const std::string &line_content) {
            std::ostringstream ss;
            ss << prefix << " error at line " << line << ", column " << column << ": " << message << "\n";
            ss << line_content << "\n";
            ss << std::string(column - 1, ' ') << "^";  // to the column with error
            return ss.str();
        }
    };

    class SyntaxError : public CompilerError {
    public:
        SyntaxError(const std::string &message, int line, int column, const std::string &lineContent)
                : CompilerError("Syntax", message, line, column, lineContent) {}
    };

    class TypeError : public CompilerError {
    public:
        TypeError(const std::string &message, int line, int column, const std::string &lineContent)
                : CompilerError("Type", message, line, column, lineContent) {
        }
    };

    class NameError : public CompilerError {
    public:
        NameError(const std::string &message, int line, int column, const std::string &lineContent)
                : CompilerError("Name", message, line, column, lineContent) {}
    };

    class InputOutputError : public CompilerError {
    public:
        InputOutputError(const std::string &message, int line, int column, const std::string &lineContent)
                : CompilerError("I/O", message, line, column, lineContent) {}
    };

    class RangeError : public CompilerError {
    public:
        RangeError(const std::string &message, int line, int column, const std::string &lineContent)
                : CompilerError("Range", message, line, column, lineContent) {}
    };

    class IndentationError : public CompilerError {
    public:
        IndentationError(const std::string &message, int line, int column, const std::string &lineContent)
                : CompilerError("Indentation", message, line, column, lineContent) {}
    };

}

#endif //TONIC_ERRORS_H
