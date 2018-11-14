#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/ADT/SCCIterator.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Analysis/LoopInfo.h"
using namespace llvm;
using namespace std;
namespace {
  struct SkeletonPass : public FunctionPass {
    static char ID;
    SkeletonPass() : FunctionPass(ID) {}

    void getAnalysisUsage(AnalysisUsage &AU) const override 
    {
      AU.setPreservesCFG();
    }

    virtual bool runOnFunction(Function &F) override {
     errs() << "I saw a function called " << F.getName() << "!\n";
      
     LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
     int LoopID = 0;

      for (scc_iterator<Function *> I = scc_begin(&F), IE = scc_end(&F); I != IE; ++I) {
         // iterating over SSC
        const std::vector<BasicBlock *> &SCCBBs = *I;

        if(SCCBBs.size()>1){
          int prev_depth = 0; // First Outer Loop
          vector<int> BBCount;
          vector<int> InstCount;
          errs()<<"Outermost loop is Loop ID"<<LoopID<<" \n";
          int Outermost = LoopID;
          //LoopSequence[prev_depth] = LoopID;
          LoopID++;
          for (std::vector<BasicBlock *>::const_iterator BBI = SCCBBs.begin(), BBIE = SCCBBs.end(); BBI != BBIE; ++BBI) {
              BBCount.push_back(0);
              InstCount.push_back(0);
              errs() << (*BBI)->getName() << "  ";
              int LDepth = LI.getLoopDepth(*BBI);
              if(LDepth >= prev_depth){
                // BBs are at same or greater level as previous BB i.e nested loops
                BBCount[LDepth]++;
                InstCount[LDepth] += ( **BBI).size();
              }
              else{
                errs()<<"Loop ID "<<LoopID<<" with InstCount "<<InstCount[LDepth]<<" and BasicBlockCount "<<BBCount[LDepth]<<"\n";
                InstCount[LDepth] = InstCount[prev_depth] + (**BBI).size();
                BBCount[LDepth] = BBCount[prev_depth] + 1;
                BBCount[prev_depth] = 0;
                InstCount[prev_depth] = 0;
              }
              prev_depth = LDepth;
          }

          errs()<<"OuterMost Loop which nests all the loops is Loop ID "<< Outermost<< " with InstCount "<<InstCount[1]<<" and BasicBlockCount "<<BBCount[1]<<"\n";

         }
       }
        return false;
    }
 
 
  };
}

char SkeletonPass::ID = 0;

// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerSkeletonPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
  PM.add(new SkeletonPass());
}
static RegisterStandardPasses
  RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                 registerSkeletonPass);

