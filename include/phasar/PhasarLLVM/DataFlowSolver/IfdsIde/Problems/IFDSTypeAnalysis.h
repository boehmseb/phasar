/******************************************************************************
 * Copyright (c) 2017 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert and others
 *****************************************************************************/

#ifndef PHASAR_PHASARLLVM_DATAFLOWSOLVER_IFDSIDE_PROBLEMS_IFDSTYPEANALYSIS_H
#define PHASAR_PHASARLLVM_DATAFLOWSOLVER_IFDSIDE_PROBLEMS_IFDSTYPEANALYSIS_H

#include <map>
#include <memory>
#include <set>
#include <string>

#include "phasar/PhasarLLVM/DataFlowSolver/IfdsIde/IFDSTabulationProblem.h"
#include "phasar/PhasarLLVM/Domain/AnalysisDomain.h"

namespace llvm {
class Instruction;
class Function;
class StructType;
class Value;
} // namespace llvm

namespace psr {

class LLVMBasedICFG;
class LLVMTypeHierarchy;
class LLVMPointsToInfo;

class IFDSTypeAnalysis
    : public IFDSTabulationProblem<LLVMIFDSAnalysisDomainDefault> {
public:
  IFDSTypeAnalysis(const ProjectIRDB *IRDB, const LLVMTypeHierarchy *TH,
                   const LLVMBasedICFG *ICF, LLVMPointsToInfo *PT,
                   std::set<std::string> EntryPoints = {"main"});

  ~IFDSTypeAnalysis() override = default;

  FlowFunctionPtrType getNormalFlowFunction(n_t curr, n_t succ) override;

  FlowFunctionPtrType getCallFlowFunction(n_t callSite, f_t destFun) override;

  FlowFunctionPtrType getRetFlowFunction(n_t callSite, f_t calleeFun,
                                         n_t exitInst, n_t retSite) override;

  FlowFunctionPtrType getCallToRetFlowFunction(n_t callSite, n_t retSite,
                                               std::set<f_t> callees) override;

  FlowFunctionPtrType getSummaryFlowFunction(n_t curr, f_t destFun) override;

  InitialSeeds<n_t, d_t, l_t> initialSeeds() override;

  [[nodiscard]] d_t createZeroValue() const override;

  bool isZeroValue(d_t d) const override;

  void printNode(std::ostream &os, n_t n) const override;

  void printDataFlowFact(std::ostream &os, d_t d) const override;

  void printFunction(std::ostream &os, f_t m) const override;
};
} // namespace psr

#endif
