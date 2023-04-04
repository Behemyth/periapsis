#pragma once

#include <cstdint>

#include "synodic/functional/executor.h"

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

		constexpr Graph();

		void Execute(executor::scheduler auto scheduler) const;
	};

	template <typename V, typename E>
	constexpr Graph<V, E>::Graph()
	{
		
	}

	template <typename V, typename E>
	void Graph<V, E>::Execute(executor::scheduler auto scheduler) const
	{
	}

	// Exports

}
