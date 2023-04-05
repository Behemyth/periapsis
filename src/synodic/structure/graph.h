#pragma once

#include <concepts>

namespace synodic::graph
{

	/**
	 * \brief The base implementation details for all graphs
	 * \tparam VertexIndexType The vertex index type
	 * \tparam EdgeIndexType The edge index type
	 */
	template<std::unsigned_integral VertexIndexType, std::unsigned_integral EdgeIndexType>
	class Base
	{
	};

	namespace directed
	{
		/**
		 * \brief A directed graph
		 * \tparam VertexIndexType The vertex index type
		 * \tparam EdgeIndexType The edge index type
		 */
		template<std::unsigned_integral VertexIndexType, std::unsigned_integral EdgeIndexType>
		class Graph : public Base<VertexIndexType, EdgeIndexType>
		{
		public:
			constexpr Graph();
		};

		template <std::unsigned_integral VertexIndexType, std::unsigned_integral EdgeIndexType>
		constexpr Graph<VertexIndexType, EdgeIndexType>::Graph()
		{
		}
	}

	namespace undirected
	{
		/**
		 * \brief A undirected graph
		 * \tparam VertexIndexType The vertex index type
		 * \tparam EdgeIndexType The edge index type
		 */
		template<std::unsigned_integral VertexIndexType, std::unsigned_integral EdgeIndexType>
		class Graph : public Base<VertexIndexType, EdgeIndexType>
		{
			constexpr Graph();

		};

		template <std::unsigned_integral VertexIndexType, std::unsigned_integral EdgeIndexType>
		constexpr Graph<VertexIndexType, EdgeIndexType>::Graph()
		{
		}
	}
}
