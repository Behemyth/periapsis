#include <gtest/gtest.h>

#include <array>

#include "synodic/algorithm/max_flow.h"
#include "synodic/structure/graph.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct VertexData
{
	bool data = true;
};

struct EdgeData
{
	float capacity = 3;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tests
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(builder, TODO)
{
	synodic::graph::directed::GraphBuilder<std::int32_t, std::int32_t> builder;

	// builder.PushNode();

	const auto graph = builder.Build();

	EXPECT_FALSE(graph.EdgeCount());
}

TEST(graph, direct_initialization)
{
	using namespace synodic::graph;

	std::array<VertexData, 3> vertices;
	std::array<EdgeData, 3> edges;

	// All edge-vertex connections are consumed by the graph structure. Temporary

	constexpr directed::StaticGraph<std::int8_t, std::int8_t, 3, 3> graph(edges);

	EXPECT_FALSE(graph.EdgeCount());
}

TEST(graph, indirect_initialization)
{
	using namespace synodic::graph;

	std::array<VertexData, 5> vertices; // Skip index 1 and 4
	std::array<EdgeData, 3> edges;

	// All edge-vertex connections are consumed by the graph structure. Temporary

	constexpr directed::StaticGraph<std::int8_t, std::int8_t, 3, 3> graph(edges);

	EXPECT_FALSE(graph.EdgeCount());
}

TEST(max_flow, TODO)
{
	using namespace synodic::graph;
	using namespace synodic::algorithm::graph;

	// constexpr std::array edges {Edge8(0, 1), Edge8(1, 2), Edge8(0, 2)};
	// constexpr directed::StaticGraph<std::int8_t, std::int8_t, 3, 3> graph(edges);

	// MaxFlow(graph);
}