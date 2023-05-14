#include "lexer.h"

#include <utility>

namespace tonic {

    Lexer::Lexer(std::string source)
            : source(std::move(source)), first_pass_start(0), first_pass_current(0), first_pass_line(1),
              first_pass_indentation_level(0) {}

    std::vector<Token> Lexer::FirstPass() {
        while (first_pass_current < source.size()) {
            first_pass_start = first_pass_current;

            char c = source[first_pass_current];
            switch (c) {
                case ';':
                    AddToken(TokenType::SEMICOLON, ";");
                    ++first_pass_current;
                    break;
                case '\n':
                    AddToken(TokenType::NEWLINE, "\n");
                    ++first_pass_current;
                    HandleIndentation();
                    break;
                case ' ':
                case '\t':
                    SkipWhitespace();
                    break;
                case '/':
                    if (first_pass_current < source.size() - 1 &&
                        (source[first_pass_current + 1] == '/' || source[first_pass_current + 1] == '*')) {
                        HandleComment(source[first_pass_current + 1] == '*');
                    } else {
                        AddToken(TokenType::SLASH, "/");
                        ++first_pass_current;
                    }
                    break;
                case '#':
                    if (first_pass_current < source.size() - 1 && std::isalpha(source[first_pass_current + 1])) {
                        HandlePreprocessor();
                    } else {
                        AddToken(TokenType::HASHTAG, "#");
                        ++first_pass_current;
                    }
                    break;
                case ':':
                    AddToken(TokenType::COLON, ":");
                    ++first_pass_current;
                    break;
                case ',':
                    AddToken(TokenType::COMMA, ",");
                    ++first_pass_current;
                    break;
                case '.':
                    AddToken(TokenType::DOT, ".");
                    ++first_pass_current;
                    break;
                case '?':
                    AddToken(TokenType::QMARK, "?");
                    ++first_pass_current;
                    break;
                case '!':
                    AddToken(TokenType::EXCLAMATION, "!");
                    ++first_pass_current;
                    break;
                case '=':
                    AddToken(TokenType::EQ, "=");
                    ++first_pass_current;
                    break;
                case '+':
                    AddToken(TokenType::PLUS, "+");
                    ++first_pass_current;
                    break;
                case '-':
                    if (first_pass_current < source.size() - 1 && source[first_pass_current + 1] == '>') {
                        AddToken(TokenType::ARROW, "->");
                        first_pass_current += 2;
                    } else {
                        AddToken(TokenType::MINUS, "-");
                        ++first_pass_current;
                    }
                    break;
                case '*':
                    AddToken(TokenType::STAR, "*");
                    ++first_pass_current;
                    break;
                case '%':
                    AddToken(TokenType::PERCENT, "%");
                    ++first_pass_current;
                    break;
                case '&':
                    AddToken(TokenType::AMPERSAND, "&");
                    ++first_pass_current;
                    break;
                case '|':
                    AddToken(TokenType::BAR, "|");
                    ++first_pass_current;
                    break;
                case '^':
                    AddToken(TokenType::CARET, "^");
                    ++first_pass_current;
                    break;
                case '{':
                    AddToken(TokenType::LCURLY, "{");
                    ++first_pass_current;
                    break;
                case '}':
                    AddToken(TokenType::RCURLY, "}");
                    ++first_pass_current;
                    break;
                case '[':
                    AddToken(TokenType::LSQUARE, "[");
                    ++first_pass_current;
                    break;
                case ']':
                    AddToken(TokenType::RSQUARE, "]");
                    ++first_pass_current;
                    break;
                case '(':
                    AddToken(TokenType::LPAREN, "(");
                    ++first_pass_current;
                    break;
                case ')':
                    AddToken(TokenType::RPAREN, ")");
                    ++first_pass_current;
                    break;
                case '@':
                    AddToken(TokenType::AT, "@");
                    ++first_pass_current;
                    break;
                case '>':
                    AddToken(TokenType::GT, ">");
                    ++first_pass_current;
                    break;
                case '<':
                    AddToken(TokenType::LT, "<");
                    ++first_pass_current;
                    break;
                default:
                    if (std::isalpha(c) || c == '_') {  // first char is alpha or _
                        HandleIdentifiers();
                    } else if (std::isdigit(c)) {
                        HandleNumbers();
                    } else if (c == '"' || c == '\'') {
                        if (c == '"') {
                            HandleString();
                        } else {
                            HandleCharacter();
                        }
                    } else {
                        throw std::runtime_error(
                                "Syntax error: Unexpected character at line " + std::to_string(first_pass_line));
                    }
                    break;
            }
        }

        AddToken(TokenType::EOF_TOKEN, "");
        return tokens;
    }

    std::vector<Token> Lexer::SecondPass() {
        std::vector<Token> new_tokens;
        size_t i = 0;
        while (i < tokens.size()) {
            if (tokens[i] == TokenType::AT && i < tokens.size() - 1 &&
                tokens[i + 1] == TokenType::IDENTIFIER &&
                tokens[i + 1].lexeme == "memoize") {
                new_tokens.emplace_back(TokenType::MEMOIZE, "@memoize", tokens[i].line);
                ++i;
            } else if (tokens[i] == TokenType::EQ && i < tokens.size() - 1 &&
                       tokens[i + 1] == TokenType::GT) {
                new_tokens.emplace_back(TokenType::LAMBDA, "=>", tokens[i].line);
                ++i;
            } else if (tokens[i] == TokenType::CONST) {
                if ((i >= tokens.size() - 1 || tokens[i + 1] != TokenType::IDENTIFIER) ||
                    (i <= 0 || tokens[i - 1] != TokenType::COLON)) {
                    throw std::runtime_error("Syntax error: Please use const next to a type declaration");
                }

                new_tokens.emplace_back(TokenType::TYPE, tokens[i].lexeme + " " + tokens[i + 1].lexeme, tokens[i].line);
                ++i;
            } else if (tokens[i] == TokenType::IDENTIFIER) {
                if (i > 0 && tokens[i - 1] == TokenType::COLON) {
                    new_tokens.emplace_back(TokenType::TYPE, tokens[i].lexeme,
                                            tokens[i].line);
                } else if (i < tokens.size() - 2 &&
                           tokens[i + 1] == TokenType::IDENTIFIER &&
                           tokens[i + 2] == TokenType::LPAREN) {
                    new_tokens.emplace_back(TokenType::TYPE, tokens[i].lexeme,
                                            tokens[i].line);
                } else if (i < tokens.size() - 1 &&
                           tokens[i + 1] == TokenType::IDENTIFIER) {
                    new_tokens.emplace_back(TokenType::TYPE, tokens[i].lexeme,
                                            tokens[i].line);
                } else {
                    new_tokens.push_back(tokens[i]);
                }
            } else if (tokens[i] == TokenType::DOT && i < tokens.size() - 1 &&
                       tokens[i + 1] == TokenType::DOT) {
                new_tokens.emplace_back(TokenType::FOR_RANGE, FOR_DOTS, tokens[i].line);
                ++i;
            } else {
                if (tokens[i] == TokenType::SEMICOLON) {
                    throw std::runtime_error(
                            "Syntax error: Semi-colons are not supported in Tonic. Please use the #cpp directive to use C++ code");
                }

                new_tokens.push_back(tokens[i]);
            }
            ++i;
        }
        return new_tokens;
    }

    std::vector<Token> Lexer::Tokenize() {
        tokens = FirstPass();
        tokens = SecondPass();
        return tokens;
    }

    void Lexer::HandlePreprocessor() {
        first_pass_current++;

        std::string directive = "#";

        while (first_pass_current < source.size()) {
            if (std::isalpha(source[first_pass_current])) {
                directive.push_back(source[first_pass_current]);
                first_pass_current++;
            } else {
                first_pass_current++;
                break;
            }
        }
        if (directive == CPP_TAG) {
            size_t j = first_pass_current + 1;
            bool ended = false;
            std::string chunk;
            while (j < source.size()) {
                if (j < source.size() - 3 && source.substr(j, 4) == END_TAG) {
                    ended = true;
                    break;
                }
                chunk += source[j];
                j++;
            }
            if (!ended) {
                throw std::runtime_error("Syntax error: Need #end directive for #cpp chunk");
            }

            AddToken(TokenType::CPP_CHUNK, chunk);
            first_pass_current = j + 4;
        } else {
            AddToken(TokenType::CPP_DIRECTIVE, directive);
        }
    }

    void Lexer::HandleComment(bool multiline) {
        size_t start_comment = first_pass_current;

        if (multiline) {
            while (first_pass_current < source.size() - 1 &&
                   (source[first_pass_current] != '*' || source[first_pass_current + 1] != '/')) {
                if (source[first_pass_current] == '\n') {
                    ++first_pass_line;
                }
                ++first_pass_current;
            }

            if (first_pass_current < source.size() - 1) {
                first_pass_current += 2;  // To account for '*/'
            } else {
                throw std::runtime_error(
                        "Syntax error: Unterminated multiline comment at line " + std::to_string(first_pass_line));
            }
        } else {
            while (first_pass_current < source.size() && source[first_pass_current] != '\n') {
                ++first_pass_current;
            }
            if (first_pass_current < source.size() && source[first_pass_current] == '\n') {
                ++first_pass_line;
            }
        }

        AddToken(TokenType::COMMENT,
                 source.substr(start_comment, first_pass_current - start_comment));
    }

    void Lexer::AddToken(TokenType type, const std::string &text) {
        tokens.emplace_back(type, text, first_pass_line);
    }

    void Lexer::SkipWhitespace() {
        while (first_pass_current < source.size() &&
               (source[first_pass_current] == ' ' || source[first_pass_current] == '\t')) {
            ++first_pass_current;
        }
    }

    void Lexer::HandleIndentation() {
        int indent_count = 0;
        while (first_pass_current < source.size() &&
               (source[first_pass_current] == ' ' || source[first_pass_current] == '\t')) {
            indent_count += (source[first_pass_current] == ' ')
                            ? 1
                            : 4;  // assuming 1 space or 1 tab (as 4 spaces)
            ++first_pass_current;
        }

        if (source[first_pass_current] == '\n') {
            return;
        }

        if (indent_stack.empty()) {
            indent_stack.push_back(0);
        }

        if (indent_count > indent_stack.back()) {
            indent_stack.push_back(indent_count);
            ++first_pass_indentation_level;
            AddToken(TokenType::INDENT, "");
        } else {
            while (indent_count < indent_stack.back()) {
                indent_stack.pop_back();
                --first_pass_indentation_level;
                AddToken(TokenType::DEDENT, "");
            }
        }
    }

    void Lexer::HandleKeywords() {
        std::string identifier = source.substr(first_pass_start, first_pass_current - first_pass_start);
        static const std::unordered_map<std::string, TokenType> keywords = {
                {"if",        TokenType::IF},
                {"else if",   TokenType::ELSE_IF},
                {"else",      TokenType::ELSE},
                {"for",       TokenType::FOR},
                {"while",     TokenType::WHILE},
                {"in",        TokenType::IN},
                {"out",       TokenType::OUT},
                {"return",    TokenType::RETURN},
                {"switch",    TokenType::SWITCH},
                {"case",      TokenType::CASE},
                {"class",     TokenType::CLASS},
                {"struct",    TokenType::STRUCT},
                {"public",    TokenType::PUBLIC},
                {"private",   TokenType::PRIVATE},
                {"protected", TokenType::PROTECTED},
                {"type",      TokenType::TYPE},
                {"try",       TokenType::TRY},
                {"catch",     TokenType::CATCH},
                {"throw",     TokenType::THROW},
                {"trace",     TokenType::TRACE},
                {"template",  TokenType::TEMPLATE},
                {"typename",  TokenType::TYPENAME},
                {"using",     TokenType::USING},
                {"namespace", TokenType::NAMESPACE},
                {"operator",  TokenType::OPERATOR},
                {"default",   TokenType::DEFAULT},
                {"break",     TokenType::BREAK},
                {"const",     TokenType::CONST},
        };

        auto keyword_it = keywords.find(identifier);
        if (keyword_it != keywords.end()) {
            AddToken(keyword_it->second, identifier);
        } else {
            AddToken(TokenType::IDENTIFIER, identifier);
        }
    }

    void Lexer::HandleIdentifiers() {
        bool first = true;
        while (first_pass_current < source.size()) {
            if (std::isalnum(source[first_pass_current]) || source[first_pass_current] == '_') {
                ++first_pass_current;
                first = false;
            } else if (!first && source[first_pass_current] == ':' &&
                       first_pass_current < source.size() - 1 && source[first_pass_current + 1] == ':') {
                first_pass_current += 2;
            } else if (!first && (source[first_pass_current] == '&' || source[first_pass_current] == '*')) {
                ++first_pass_current;
            } else if (source[first_pass_current] == '<') {
                if (!HandleTemplateExpression()) {
                    throw std::runtime_error("Syntax error: Invalid template expression");
                }
            } else {
                break;
            }
        }

        HandleKeywords();
    }

    bool Lexer::HandleTemplateExpression() {
        int angle_brackets_count = 0;
        while (first_pass_current < source.size()) {
            if (source[first_pass_current] == '<') {
                angle_brackets_count++;
                ++first_pass_current;
            } else if (source[first_pass_current] == '>') {
                angle_brackets_count--;
                ++first_pass_current;
                if (angle_brackets_count < 0) {
                    throw std::runtime_error("Syntax error: Unmatched '>' in template expression");
                } else if (angle_brackets_count == 0) {
                    return true;
                }
            } else if (source[first_pass_current] == ',' && angle_brackets_count == 1) {
                ++first_pass_current;
            } else if (std::isalnum(source[first_pass_current]) || source[first_pass_current] == '_') {
                ++first_pass_current;
            } else if (source[first_pass_current] == ':' && first_pass_current < source.size() - 1 &&
                       source[first_pass_current + 1] == ':') {
                first_pass_current += 2;
            } else if (std::isspace(source[first_pass_current])) {
                ++first_pass_current;
            } else {
                throw std::runtime_error(
                        "Syntax error: Unexpected character '" + std::string(1, source[first_pass_current]) +
                        "' in template expression");
            }
        }
        if (angle_brackets_count > 0) {
            throw std::runtime_error("Syntax error: Unmatched '<' in template expression");
        }
        return false;
    }

    void Lexer::HandleNumbers() {
        while (first_pass_current < source.size() && std::isdigit(source[first_pass_current])) {
            ++first_pass_current;
        }

        if (first_pass_current < source.size() && source[first_pass_current] == '.' &&
            std::isdigit(source[first_pass_current + 1])) {
            ++first_pass_current;
            while (first_pass_current < source.size() && std::isdigit(source[first_pass_current])) {
                ++first_pass_current;
            }
        }

        AddToken(TokenType::LITERAL, source.substr(first_pass_start, first_pass_current - first_pass_start));
    }

    void Lexer::HandleString() {
        char quote = source[first_pass_current];
        ++first_pass_current;

        while (first_pass_current < source.size() && source[first_pass_current] != quote) {
            if (source[first_pass_current] == '\\' && first_pass_current + 1 < source.size() &&
                (source[first_pass_current + 1] == quote || source[first_pass_current + 1] == '\\')) {
                ++first_pass_current;
            }
            ++first_pass_current;
        }

        if (first_pass_current >= source.size() || source[first_pass_current] != quote) {
            throw std::runtime_error("Syntax error: Unterminated string at line " +
                                     std::to_string(first_pass_line));
        }

        ++first_pass_current;
        AddToken(TokenType::LITERAL, source.substr(first_pass_start, first_pass_current - first_pass_start));
    }

    void Lexer::HandleCharacter() {
        ++first_pass_current;

        if (first_pass_current >= source.size()) {
            throw std::runtime_error(
                    "Syntax error: Unterminated character literal at line " +
                    std::to_string(first_pass_line));
        }

        if (source[first_pass_current] == '\\') {
            ++first_pass_current;

            if (first_pass_current >= source.size()) {
                throw std::runtime_error(
                        "Syntax error: Unterminated character literal at line " +
                        std::to_string(first_pass_line));
            }

            switch (source[first_pass_current]) {
                case 'n':
                case 't':
                case 'r':
                case 'b':
                case 'f':
                case 'v':
                case 'a':
                case '\\':
                case '?':
                case '\'':
                case '\"':
                    ++first_pass_current;
                    break;
                default:
                    throw std::runtime_error(
                            "Syntax error: Invalid escape sequence at line " +
                            std::to_string(first_pass_line));
            }
        } else {
            ++first_pass_current;
        }

        if (first_pass_current >= source.size() || source[first_pass_current] != '\'') {
            throw std::runtime_error(
                    "Syntax error: Unterminated character literal at line " +
                    std::to_string(first_pass_line));
        }

        ++first_pass_current;
        AddToken(TokenType::LITERAL, source.substr(first_pass_start, first_pass_current - first_pass_start));
    }

}