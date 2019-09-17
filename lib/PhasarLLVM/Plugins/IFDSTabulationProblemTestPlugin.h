/******************************************************************************
 * Copyright (c) 2017 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert and others
 *****************************************************************************/

/*
 * PluginTest.h
 *
 *  Created on: 14.06.2017
 *      Author: philipp
 */

#ifndef SRC_ANALYSIS_PLUGINS_IFDSTABULATIONPROBLEMTESTPLUGIN_H_
#define SRC_ANALYSIS_PLUGINS_IFDSTABULATIONPROBLEMTESTPLUGIN_H_

#include <phasar/PhasarLLVM/IfdsIde/EdgeFunctionComposer.h>
#include <phasar/PhasarLLVM/IfdsIde/FlowFactWrapper.h>
#include <phasar/PhasarLLVM/IfdsIde/Problems/TypeStateDescriptions/TypeStateDescription.h>
#include <phasar/PhasarLLVM/Plugins/Interfaces/IfdsIde/IFDSTabulationProblemPlugin.h>

namespace psr {

class IFDSTabulationProblemTestPlugin : public IFDSTabulationProblemPlugin {
public:
  IFDSTabulationProblemTestPlugin(LLVMBasedICFG &I,
                                  std::vector<std::string> EntryPoints);
  ~IFDSTabulationProblemTestPlugin() = default;

  const FlowFact *createZeroValue() override;

  bool isZeroValue(const FlowFact *d) const override;

  void printDataFlowFact(std::ostream &os, const FlowFact *d) const override;

  std::shared_ptr<FlowFunction<const FlowFact *>>
  getNormalFlowFunction(const llvm::Instruction *curr,
                        const llvm::Instruction *succ) override;

  std::shared_ptr<FlowFunction<const FlowFact *>>
  getCallFlowFunction(const llvm::Instruction *callStmt,
                      const llvm::Function *destMthd) override;

  std::shared_ptr<FlowFunction<const FlowFact *>>
  getRetFlowFunction(const llvm::Instruction *callSite,
                     const llvm::Function *calleeMthd,
                     const llvm::Instruction *exitStmt,
                     const llvm::Instruction *retSite) override;

  std::shared_ptr<FlowFunction<const FlowFact *>>
  getCallToRetFlowFunction(const llvm::Instruction *callSite,
                           const llvm::Instruction *retSite,
                           std::set<const llvm::Function *> callees) override;

  std::shared_ptr<FlowFunction<const FlowFact *>>
  getSummaryFlowFunction(const llvm::Instruction *callStmt,
                         const llvm::Function *destMthd) override;

  std::map<const llvm::Instruction *, std::set<const FlowFact *>>
  initialSeeds() override;
};

extern "C" std::unique_ptr<IFDSTabulationProblemPlugin>
makeIFDSTabulationProblemTestPlugin(LLVMBasedICFG &I,
                                    std::vector<std::string> EntryPoints);

} // namespace psr

#endif /* SRC_ANALYSIS_PLUGINS_IFDSTABULATIONPROBLEMTESTPLUGIN_HH_ */
