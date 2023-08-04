/**
 * Licensed under the Apache License, Version 2.0;
 * Please find the license in the repository .LICENSE file here:
 * https://github.com/tonic-lang/tonic/blob/main/LICENSE
 *
 * @brief Symbol table for analyzers
 */

#ifndef TONIC_TABLE_H
#define TONIC_TABLE_H

#include <string>
#include <unordered_map>

namespace tonic {
    struct SymbolInfo {
        std::string name;
        std::string type;

        SymbolInfo(std::string name, std::string type) : name(name), type(type) {}
    };

    class SymbolTable {
    public:
        void Insert(const std::string &name, const SymbolInfo &info);

        SymbolInfo &LookupSymbol(const std::string &name);

        bool SymbolExists(const std::string &name) const;

        void RemoveSymbol(const std::string &name);

    private:
        std::unordered_map<std::string, SymbolInfo> table;
    };
}

#endif //TONIC_TABLE_H
