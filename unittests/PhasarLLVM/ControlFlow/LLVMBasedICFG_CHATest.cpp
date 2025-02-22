#include "gtest/gtest.h"

#include <string>

#include "phasar/Config/Configuration.h"
#include "phasar/DB/ProjectIRDB.h"
#include "phasar/PhasarLLVM/ControlFlow/LLVMBasedICFG.h"
#include "phasar/PhasarLLVM/Pointer/LLVMPointsToSet.h"
#include "phasar/PhasarLLVM/TypeHierarchy/LLVMTypeHierarchy.h"
#include "phasar/Utils/LLVMShorthands.h"

#include "TestConfig.h"

using namespace std;
using namespace psr;

TEST(LLVMBasedICFG_CHATest, StaticCallSite_1) {
  ProjectIRDB IRDB(
      {unittest::PathToLLTestFiles + "call_graphs/static_callsite_1_c.ll"},
      IRDBOptions::WPA);
  LLVMTypeHierarchy TH(IRDB);
  LLVMPointsToSet PT(IRDB);
  LLVMBasedICFG ICFG(IRDB, CallGraphAnalysisType::CHA, {"main"}, &TH, &PT);
  const llvm::Function *F = IRDB.getFunctionDefinition("main");
  const llvm::Function *Foo = IRDB.getFunctionDefinition("foo");
  // iterate all instructions
  for (const auto &BB : *F) {
    for (const auto &I : BB) {
      // inspect call-sites
      if (llvm::isa<llvm::CallInst>(&I) || llvm::isa<llvm::InvokeInst>(&I)) {
        auto Callees = ICFG.getCalleesOfCallAt(&I);
        ASSERT_EQ(Callees.size(), 1U);
        ASSERT_TRUE(Callees.count(Foo));
        ASSERT_TRUE(ICFG.getCallersOf(Foo).count(&I));
      }
    }
  }
}

TEST(LLVMBasedICFG_CHATest, VirtualCallSite_2) {
  ProjectIRDB IRDB(
      {unittest::PathToLLTestFiles + "call_graphs/virtual_call_2_cpp.ll"},
      IRDBOptions::WPA);
  LLVMTypeHierarchy TH(IRDB);
  LLVMPointsToSet PT(IRDB);
  LLVMBasedICFG ICFG(IRDB, CallGraphAnalysisType::CHA, {"main"}, &TH, &PT);
  const llvm::Function *F = IRDB.getFunctionDefinition("main");
  ASSERT_TRUE(F);

  const llvm::Instruction *I = getNthInstruction(F, 13);
  if (llvm::isa<llvm::CallInst>(I) || llvm::isa<llvm::InvokeInst>(I)) {
    set<const llvm::Function *> Callees = ICFG.getCalleesOfCallAt(I);
    ASSERT_EQ(Callees.size(), 2U);
    set<string> CalleeNames;
    for (const llvm::Function *F : Callees) {
      CalleeNames.insert(F->getName().str());
    }
    ASSERT_TRUE(CalleeNames.count("_ZN1B3fooEv"));
    ASSERT_TRUE(CalleeNames.count("_ZN1A3fooEv"));
  }
}

TEST(LLVMBasedICFG_CHATest, VirtualCallSite_9) {
  ProjectIRDB IRDB(
      {unittest::PathToLLTestFiles + "call_graphs/virtual_call_9_cpp.ll"},
      IRDBOptions::WPA);
  LLVMTypeHierarchy TH(IRDB);
  LLVMPointsToSet PT(IRDB);
  LLVMBasedICFG ICFG(IRDB, CallGraphAnalysisType::CHA, {"main"}, &TH, &PT);
  const llvm::Function *F = IRDB.getFunctionDefinition("main");
  const llvm::Function *Foo = IRDB.getFunctionDefinition("_ZN1D3fooEv");
  ASSERT_TRUE(Foo);
  ASSERT_TRUE(F);

  const llvm::Instruction *I = getNthInstruction(F, 11);
  if (llvm::isa<llvm::CallInst>(I) || llvm::isa<llvm::InvokeInst>(I)) {
    set<const llvm::Function *> Callees = ICFG.getCalleesOfCallAt(I);
    set<string> CalleeNames;
    for (const llvm::Function *F : Callees) {
      CalleeNames.insert(F->getName().str());
    }
    ASSERT_EQ(Callees.size(), 4U);
    ASSERT_TRUE(CalleeNames.count("_ZN1B3fooEv"));
    ASSERT_TRUE(CalleeNames.count("_ZN1A3fooEv"));
    ASSERT_TRUE(CalleeNames.count("_ZN1D3fooEv"));
    ASSERT_TRUE(CalleeNames.count("_ZN1C3fooEv"));
    ASSERT_TRUE(ICFG.getCallersOf(Foo).count(I));
  }
}

TEST(LLVMBasedICFG_CHATest, VirtualCallSite_7) {
  ProjectIRDB IRDB(
      {unittest::PathToLLTestFiles + "call_graphs/virtual_call_7_cpp.ll"},
      IRDBOptions::WPA);
  LLVMTypeHierarchy TH(IRDB);
  LLVMPointsToSet PT(IRDB);
  LLVMBasedICFG ICFG(IRDB, CallGraphAnalysisType::CHA, {"main"}, &TH, &PT);
  const llvm::Function *F = IRDB.getFunctionDefinition("main");
  const llvm::Function *VfuncB = IRDB.getFunctionDefinition("_ZN1B5VfuncEv");
  const llvm::Function *VfuncA = IRDB.getFunctionDefinition("_ZN1A5VfuncEv");
  ASSERT_TRUE(F);
  ASSERT_TRUE(VfuncB);
  ASSERT_TRUE(VfuncA);

  const llvm::Instruction *I = getNthInstruction(F, 19);
  if (llvm::isa<llvm::CallInst>(I) || llvm::isa<llvm::InvokeInst>(I)) {
    set<const llvm::Function *> Callees = ICFG.getCalleesOfCallAt(I);
    ASSERT_TRUE(ICFG.getCallersOf(VfuncB).count(I));
    ASSERT_EQ(Callees.size(), 2U);
    ASSERT_TRUE(Callees.count(VfuncB));
    ASSERT_TRUE(Callees.count(VfuncA));
  }
}

int main(int Argc, char **Argv) {
  ::testing::InitGoogleTest(&Argc, Argv);
  return RUN_ALL_TESTS();
}
