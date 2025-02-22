/******************************************************************************
 * Copyright (c) 2020 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert and others
 *****************************************************************************/

#include <algorithm>
#include <ostream>
#include <utility>

#include "llvm/IR/Constants.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"

#include "phasar/DB/ProjectIRDB.h"
#include "phasar/PhasarLLVM/ControlFlow/LLVMBasedCFG.h"
#include "phasar/PhasarLLVM/DataFlowSolver/Mono/Problems/IntraMonoUninitVariables.h"
#include "phasar/PhasarLLVM/Pointer/LLVMPointsToInfo.h"
#include "phasar/PhasarLLVM/TypeHierarchy/LLVMTypeHierarchy.h"
#include "phasar/Utils/BitVectorSet.h"
#include "phasar/Utils/LLVMShorthands.h"

using namespace std;
using namespace psr;
namespace psr {

IntraMonoUninitVariables::IntraMonoUninitVariables(
    const ProjectIRDB *IRDB, const LLVMTypeHierarchy *TH,
    const LLVMBasedCFG *CF, const LLVMPointsToInfo *PT,
    std::set<std::string> EntryPoints)
    : IntraMonoProblem<IntraMonoUninitVariablesDomain>(IRDB, TH, CF, PT,
                                                       std::move(EntryPoints)) {
}

IntraMonoUninitVariables::mono_container_t IntraMonoUninitVariables::merge(
    const IntraMonoUninitVariables::mono_container_t &Lhs,
    const IntraMonoUninitVariables::mono_container_t &Rhs) {
  IntraMonoUninitVariables::mono_container_t Intersect;
  std::set_intersection(Lhs.begin(), Lhs.end(), Rhs.begin(), Rhs.end(),
                        std::inserter(Intersect, Intersect.begin()));
  return Intersect;
}

bool IntraMonoUninitVariables::equal_to(
    const IntraMonoUninitVariables::mono_container_t &Lhs,
    const IntraMonoUninitVariables::mono_container_t &Rhs) {
  return Lhs == Rhs;
}

IntraMonoUninitVariables::mono_container_t IntraMonoUninitVariables::allTop() {
  return {};
}

IntraMonoUninitVariables::mono_container_t IntraMonoUninitVariables::normalFlow(
    IntraMonoUninitVariables::n_t Inst,
    const IntraMonoUninitVariables::mono_container_t &In) {
  auto Out = In;
  if (const auto *Alloca = llvm::dyn_cast<llvm::AllocaInst>(Inst)) {
    Out.insert(Alloca);
  }
  if (const auto *Store = llvm::dyn_cast<llvm::StoreInst>(Inst)) {
    if (Store->getValueOperand()->getType()->isIntegerTy() &&
        llvm::isa<llvm::ConstantData>(Store->getValueOperand())) {
      llvm::outs() << "Found initialization at: ";
      Store->print(llvm::outs());
      llvm::outs() << '\n';
      Out.erase(Store->getPointerOperand());
    }
  }
  return Out;
}

unordered_map<IntraMonoUninitVariables::n_t,
              IntraMonoUninitVariables::mono_container_t>
IntraMonoUninitVariables::initialSeeds() {
  std::unordered_map<IntraMonoUninitVariables::n_t,
                     IntraMonoUninitVariables::mono_container_t>
      Seeds;
  for (const auto &EntryPoint : EntryPoints) {
    if (const auto *Fun = IRDB->getFunctionDefinition(EntryPoint)) {
      for (const auto *I : CF->getStartPointsOf(Fun)) {
        Seeds[I] = allTop();
      }
    }
  }
  return Seeds;
}

void IntraMonoUninitVariables::printNode(
    ostream &OS, IntraMonoUninitVariables::n_t Inst) const {
  OS << llvmIRToString(Inst);
}

void IntraMonoUninitVariables::printDataFlowFact(
    ostream &OS, IntraMonoUninitVariables::d_t Fact) const {
  OS << llvmIRToString(Fact);
}

void IntraMonoUninitVariables::printFunction(
    ostream &OS, IntraMonoUninitVariables::f_t Fun) const {
  OS << Fun->getName().str();
}

} // namespace psr
