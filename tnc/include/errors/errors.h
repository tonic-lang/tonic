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
#include <utility>
#include <vector>
#include <string>

namespace tonic {

    class InternalError : public std::runtime_error {
    public:
        explicit InternalError(const std::string &message)
                : std::runtime_error("Internal error: " + message) {}
    };

    class CompilerError : public std::runtime_error {
    public:
        CompilerError(const std::string &prefix,
                      const std::string &message, size_t line,
                      const std::string &line_content,
                      const std::string &file_name)
                : std::runtime_error(MessageBuilder(prefix, message, line, line_content, file_name)) {}

    private:
        static std::string MessageBuilder(const std::string &prefix,
                                          const std::string &message, size_t line,
                                          const std::string &line_content,
                                          const std::string &file_name) {
            std::ostringstream ss;
            ss << "Error in file: " << file_name << "\n";
            ss << prefix << " error near line " << line << ": " << message << "\n";
            if (!line_content.empty())
                ss << "Found near: " << line_content << "\n\n";
            else
                ss << "\n";
            return ss.str();
        }
    };

    class SyntaxError : public CompilerError {
    public:
        SyntaxError(const std::string &message,
                    size_t line,
                    const std::string &line_content,
                    const std::string &file_name)
                : CompilerError("Syntax", message, line, line_content, file_name) {}
    };

    class TypeError : public CompilerError {
    public:
        TypeError(const std::string &message,
                  size_t line,
                  const std::string &line_content,
                  const std::string &file_name)
                : CompilerError("Type", message, line, line_content, file_name) {
        }
    };

    class NameError : public CompilerError {
    public:
        NameError(const std::string &message,
                  size_t line,
                  const std::string &line_content,
                  const std::string &file_name)
                : CompilerError("Name", message, line, line_content, file_name) {}
    };

    class InputOutputError : public CompilerError {
    public:
        InputOutputError(const std::string &message,
                         size_t line,
                         const std::string &line_content,
                         const std::string &file_name)
                : CompilerError("I/O", message, line, line_content, file_name) {}
    };

    class RangeError : public CompilerError {
    public:
        RangeError(const std::string &message,
                   size_t line,
                   const std::string &line_content,
                   const std::string &file_name)
                : CompilerError("Range", message, line, line_content, file_name) {}
    };

    class IndentationError : public CompilerError {
    public:
        IndentationError(const std::string &message,
                         size_t line,
                         const std::string &line_content,
                         const std::string &file_name)
                : CompilerError("Indentation", message, line, line_content, file_name) {}
    };

    class MetaError {
    public:
        MetaError(std::string meta_prefix,
                  std::string meta_error)
                : meta_prefix(std::move(meta_prefix)), meta_error(std::move(meta_error)) {}

        ~MetaError() = default;

        void AddError(const std::string &error) {
            errors.push_back(error);
        }

        void Throw() {
            if (errors.empty()) {
                throw InternalError("Error while throwing meta-error: No errors found");
            }

            std::ostringstream ss;
            for (const auto &error: errors) {
                ss << error << std::endl << std::endl;
            }
            ss << meta_prefix << " error: " << meta_error << std::endl;

            throw std::runtime_error(ss.str());
        }

        bool HasErrors() {
            return !errors.empty();
        }

    private:
        std::vector<std::string> errors;
        std::string meta_prefix;
        std::string meta_error;
    };

}

#endif //TONIC_ERRORS_H
