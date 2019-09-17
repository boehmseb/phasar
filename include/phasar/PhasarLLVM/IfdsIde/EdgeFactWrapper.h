/******************************************************************************
 * Copyright (c) 2017 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert and others
 *****************************************************************************/

#ifndef PHASAR_PHASARLLVM_IFDSIDE_EDGEFACTWRAPPER_H_
#define PHASAR_PHASARLLVM_IFDSIDE_EDGEFACTWRAPPER_H_

#include <ostream>

#include <phasar/PhasarLLVM/IfdsIde/EdgeFact.h>

namespace psr {

template <typename T> class EdgeFactWrapper : public EdgeFact {
private:
  const T fact;

public:
  EdgeFactWrapper(T f) : fact(f) {}

  ~EdgeFactWrapper() override = default;

  T get() const { return fact; }

  void print(std::ostream &OS) const override { OS << fact << '\n'; }

  bool equal_to(const EdgeFact &EF) const override {
    if (auto EFW = dynamic_cast<const EdgeFactWrapper<T> *>(&EF)) {
      return fact == EFW->get();
    } else {
      return this == EFW;
    }
  }

  bool less(const EdgeFact &EF) const override {
    if (auto EFW = dynamic_cast<const EdgeFactWrapper<T> *>(&EF)) {
      return fact < EFW->get();
    } else {
      return this < EFW;
    }
  }
};

} // namespace psr

#endif
