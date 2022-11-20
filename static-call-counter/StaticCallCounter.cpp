#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

using namespace llvm;

namespace {

struct StaticCallCounter : public AnalysisInfoMixin<StaticCallCounter> {
	using Result = MapVector<const Function *, unsigned>;

	Result run (Module &M, ModuleAnalysisManager &) {
		Result result;
        for (auto &F : M) {
            for (auto &BB : F) {
                for (auto &I : BB) {
                    auto *CB = dyn_cast<CallBase>(&I);
                    if (CB == nullptr) {
                        continue;
                    }
                    auto DirectInvokedFunction = CB->getCalledFunction();
                    if (DirectInvokedFunction == nullptr) {
                        continue;
                    }
                    result[DirectInvokedFunction]++;
                }
            }
        }
        return result;
	}

	static bool isRequired() { return true; }

private:
	static AnalysisKey Key;
	friend struct AnalysisInfoMixin<StaticCallCounter>;
};

class StaticCallCounterPrinter : public PassInfoMixin<StaticCallCounterPrinter> {
public:
	explicit StaticCallCounterPrinter(raw_ostream &OS) : OS(OS) {}

	PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM) {
		auto DirectCalls = MAM.getResult<StaticCallCounter>(M);
        for (auto &CallCount : DirectCalls) {
            OS << format("%-20s %-10lu\n", CallCount.first->getName().str().c_str(), CallCount.second);
        }
		return PreservedAnalyses::all();
	}
    
	static bool isRequired() { return true; }

private:
	raw_ostream &OS;
};

AnalysisKey StaticCallCounter::Key;

PassPluginLibraryInfo getStaticCallCounterPluginInfo() {
	return {LLVM_PLUGIN_API_VERSION, "StaticCallCounter", LLVM_VERSION_STRING,
		[](PassBuilder &PB) {
			PB.registerPipelineParsingCallback(
				[](StringRef Name, ModulePassManager &MPM,
					ArrayRef<PassBuilder::PipelineElement>) {
					if (Name == "static-call-counter") {
						MPM.addPass(StaticCallCounterPrinter(errs()));
						return true;
					}
					return false;
				});
			
			PB.registerAnalysisRegistrationCallback(
				[](ModuleAnalysisManager &MAM) {
					MAM.registerPass([] {
						return StaticCallCounter();
					});
				});
		}
	};
}

}

extern "C" LLVM_ATTRIBUTE_WEAK::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
	return getStaticCallCounterPluginInfo();
}
