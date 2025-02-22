/******************************************************************************
 * Copyright (c) 2017 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert and others
 *****************************************************************************/

#ifndef PHASAR_PHASARLLVM_DATAFLOWSOLVER_WPDS_WPDSOPTIONS_H
#define PHASAR_PHASARLLVM_DATAFLOWSOLVER_WPDS_WPDSOPTIONS_H

#include <iosfwd>
#include <string>

namespace psr {

enum class WPDSType {
#define WPDS_TYPES(NAME, TYPE) TYPE,
#include "phasar/PhasarLLVM/DataFlowSolver/WPDS/WPDSType.def"
};

WPDSType toWPDSType(const std::string &S);

std::string toString(const WPDSType &T);

std::ostream &operator<<(std::ostream &OS, const WPDSType &T);

enum class WPDSSearchDirection { FORWARD, BACKWARD };

WPDSSearchDirection toWPDSSearchDirection(const std::string &S);

std::string toString(const WPDSSearchDirection &S);

std::ostream &operator<<(std::ostream &OS, const WPDSSearchDirection &S);

} // namespace psr

#endif
