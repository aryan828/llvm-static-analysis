#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

using namespace llvm;

namespace {

struct ListFunctionResult {
	const char *name;
	size_t args;
	size_t bb;
	size_t inst;
};

struct ListFunction : public AnalysisInfoMixin<ListFunction> {
	using Result = ListFunctionResult;
	ListFunctionResult run (Function &F, FunctionAnalysisManager &) {
		return {F.getName().data(), F.arg_size(), F.size(), F.getInstructionCount()};
	}
	static bool isRequired() { return true; }

private:
	static AnalysisKey Key;
	friend struct AnalysisInfoMixin<ListFunction>;
};

class ListFunctionPrinter : public PassInfoMixin<ListFunctionPrinter> {
public:
	explicit ListFunctionPrinter(raw_ostream &OS) : OS(OS) {}
	PreservedAnalyses run(Function &Func, FunctionAnalysisManager &FAM) {
		auto &LFR = FAM.getResult<ListFunction>(Func);
		const char* name = "NAME";
		const char* arguments = "ARGUMENTS";
		const char* bb = "BASIC BLOCKS";
		const char* instructions = "INSTRUCTIONS";
		OS << format("%-20s %-20s %-20s %-20s", name, arguments, bb, instructions) << "\n";
		OS << "===========================================================================\n";
		OS << format("%-20s %-20u %-20u %-20u", LFR.name, LFR.args, LFR.bb, LFR.inst) << "\n";
		OS << "---------------------------------------------------------------------------\n";
		return PreservedAnalyses::all();
	}
	static bool isRequired() { return true; }

private:
	raw_ostream &OS;
};

AnalysisKey ListFunction::Key;

PassPluginLibraryInfo getListFunctionPluginInfo() {
	return {LLVM_PLUGIN_API_VERSION, "ListFunction", LLVM_VERSION_STRING,
		[](PassBuilder &PB) {
			PB.registerPipelineParsingCallback(
				[](StringRef Name, FunctionPassManager &FPM,
					ArrayRef<PassBuilder::PipelineElement>) {
					if (Name == "list-function") {
						FPM.addPass(ListFunctionPrinter(errs()));
						return true;
					}
					return false;
				});
			
			PB.registerAnalysisRegistrationCallback(
				[](FunctionAnalysisManager &FAM) {
					FAM.registerPass([] {
						return ListFunction();
					});
				});
		}
	};
}

}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
	return getListFunctionPluginInfo();
}
