#include <gtest/gtest.h>

import MaxFlow;
import Graph;

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

TEST(GraphBuilder, TODO)
{
	synodic::graph::directed::GraphBuilder<std::int32_t, std::int32_t> builder;

	// builder.PushNode();

	const auto graph = builder.Build();

	EXPECT_FALSE(graph.EdgeCount());
}

TEST(StaticGraph, edge_index_initialization)
{
	using namespace synodic::graph;

	// The test data representation for the vertex range
	constexpr std::array<VertexData, 3> vertexData;

	// The test data representation for the edge range
	std::array<EdgeData, 3> edgeDate;

	// TODO: Temporary. All edge-vertex connections are consumed by the graph structure.
	// TODO: Edges should be index or value based.

	//constexpr directed::StaticGraph graph(vertices, edges);

	//EXPECT_FALSE(graph.EdgeCount());
}

TEST(MaxFlow, TODO)
{
	using namespace synodic::graph;
	using namespace synodic::algorithm::graph;

	// constexpr std::array edges {Edge8(0, 1), Edge8(1, 2), Edge8(0, 2)};
	// constexpr directed::StaticGraph<std::int8_t, std::int8_t, 3, 3> graph(edges);

	// MaxFlow(graph);
}