#include <gtest/gtest.h>
#include "synodic/structure/graph.h"
#include <array>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tests
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(iface, TODO)
{
	constexpr synodic::graph::directed::GraphBuilder<uint32_t, uint32_t> builder;

	const auto graph = builder.Build();

	EXPECT_FALSE(graph.EdgeCount());
}

TEST(edge, TODO)
{
	constexpr synodic::graph::directed::GraphBuilder<uint32_t, uint32_t> builder;

	//auto node = builder.PushNode();

}

TEST(graph, TODO)
{
	constexpr synodic::graph::directed::Graph<uint32_t, uint32_t> empty;

	EXPECT_FALSE(empty.EdgeCount());

	constexpr std::array<const synodic::graph::Edge<std::uint8_t>, 1> edges{{{0,1}}};

	constexpr std::span span = edges;
	constexpr synodic::graph::directed::StaticGraph<uint8_t, uint8_t,2,1> graph(span);

	EXPECT_FALSE(graph.EdgeCount());
}