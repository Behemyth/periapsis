#pragma once

#include <cstdint>

namespace synodic
{
	// TODO: Make the ID type configurable
	template<uint16_t T>
	struct Vertex
	{
		static constexpr uint16_t ID = T;
	};

	template<typename U, typename V>
	struct Edge
	{
		using Tail = U;
		using Head = V;
	};

	template<typename V, typename E>
	struct Graph
	{
		using Vertices = V;
		using Edges	   = E;
	};

	// Exports

}
