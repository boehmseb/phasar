/******************************************************************************
 * Copyright (c) 2017 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert and others
 *****************************************************************************/

#ifndef PHASAR_PHASARLLVM_UTILS_SCOPES_H_
#define PHASAR_PHASARLLVM_UTILS_SCOPES_H_

#include <iosfwd>
#include <map>
#include <string>

namespace psr {

enum class Scope { function, module, project };

std::ostream &operator<<(std::ostream &OS, const Scope &S);

extern const std::map<std::string, Scope> StringToScope;

extern const std::map<Scope, std::string> ScopeToString;

} // namespace psr

#endif
