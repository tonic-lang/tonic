/**
 * Licensed under the Apache License, Version 2.0;
 * Please find the license in the repository .LICENSE file here:
 * https://github.com/tonic-lang/tonic/blob/main/LICENSE
 *
 * @brief Walker class for the AST node for semantic analysis
 */

#ifndef TONIC_WALKER_H
#define TONIC_WALKER_H

#include <stack>
#include <memory>
#include <functional>
#include <unordered_map>

#include "ast.h"

namespace tonic {

    class Walker {
    public:
        using NodePtr = std::shared_ptr<Node>;
        using Action = std::function<void(NodePtr)>;

        Walker();

        template<typename T>
        void Register(std::function<void(std::shared_ptr<T>)> action) {
            std::string type_name = typeid(T).name();
            handlers[type_name] = [action](NodePtr node) {
                action(std::dynamic_pointer_cast<T>(node));
            };
        }

        void Walk(const NodePtr& node);

    private:
        template<typename T>
        void WalkVector(const std::vector<T>& list) {
            for (const auto &sub_node: list) {
                Walk(sub_node);
            }
        }

        std::unordered_map<std::string, std::function<void(NodePtr)>> handlers;
        std::stack<NodePtr> visited_nodes;
    };

}

#endif //TONIC_WALKER_H
