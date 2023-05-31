/**
 * Licensed under the Apache License, Version 2.0;
 * Please find the license in the repository .LICENSE file here:
 * https://github.com/tonic-lang/tonic/blob/main/LICENSE
 *
 * @brief Code generator to C++
 */

#ifndef TONIC_GENERATOR_H
#define TONIC_GENERATOR_H

#include <sstream>

#include "ast.h"

namespace tonic {
    std::string Generator(const VariableDeclaration &var);
}

#endif //TONIC_GENERATOR_H
