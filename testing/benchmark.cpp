#include <iostream>
#include <benchmark/benchmark.h>

#include "../core/latex_core.hpp"

static void BM_LexerTokenization(benchmark::State &state, std::string equation) {
	for (auto _ : state) {
		LatexCore core_impl(equation);
		benchmark::DoNotOptimize(core_impl);
	}
}

int main(int argc, char** argv) {
	benchmark::Initialize(&argc, argv);

	if (argc > 1) {
		std::string eq = argv[1];
		benchmark::RegisterBenchmark("BM_LexerTokenization", BM_LexerTokenization, eq);
	} else {
		std::cerr << "Usage: " << argv[0] << " <equation_string>\n";
		return 1;
	}

	benchmark::RunSpecifiedBenchmarks();
	return 0;
}
