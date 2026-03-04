/**
 * @brief Benchmarks for math vector types
 */

#include <benchmark/benchmark.h>

import std;
import synodic.periapsis;

using namespace peri::math;

namespace
{
	// ============================================================================
	// Vec3<float> Benchmarks
	// ============================================================================

	void BM_Vec3Addition(benchmark::State& state)
	{
		Vec3<float> a {1.0f, 2.0f, 3.0f};
		Vec3<float> b {4.0f, 5.0f, 6.0f};
		for (auto _: state)
		{
			auto result = a + b;
			benchmark::DoNotOptimize(result);
		}
	}

	BENCHMARK(BM_Vec3Addition);

	void BM_Vec3Dot(benchmark::State& state)
	{
		Vec3<float> a {1.0f, 2.0f, 3.0f};
		Vec3<float> b {4.0f, 5.0f, 6.0f};
		for (auto _: state)
		{
			auto result = Dot(a, b);
			benchmark::DoNotOptimize(result);
		}
	}

	BENCHMARK(BM_Vec3Dot);

	void BM_Vec3Cross(benchmark::State& state)
	{
		Vec3<float> a {1.0f, 2.0f, 3.0f};
		Vec3<float> b {4.0f, 5.0f, 6.0f};
		for (auto _: state)
		{
			auto result = Cross(a, b);
			benchmark::DoNotOptimize(result);
		}
	}

	BENCHMARK(BM_Vec3Cross);

	void BM_Vec3Normalize(benchmark::State& state)
	{
		Vec3<float> a {1.0f, 2.0f, 3.0f};
		for (auto _: state)
		{
			auto result = Normalize(a);
			benchmark::DoNotOptimize(result);
		}
	}

	BENCHMARK(BM_Vec3Normalize);

	void BM_Vec3Length(benchmark::State& state)
	{
		Vec3<float> a {1.0f, 2.0f, 3.0f};
		for (auto _: state)
		{
			auto result = Length(a);
			benchmark::DoNotOptimize(result);
		}
	}

	BENCHMARK(BM_Vec3Length);

	void BM_Vec3Distance(benchmark::State& state)
	{
		Vec3<float> a {1.0f, 2.0f, 3.0f};
		Vec3<float> b {4.0f, 5.0f, 6.0f};
		for (auto _: state)
		{
			auto result = Distance(a, b);
			benchmark::DoNotOptimize(result);
		}
	}

	BENCHMARK(BM_Vec3Distance);
}  // namespace
