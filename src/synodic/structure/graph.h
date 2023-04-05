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
	public:
		uint32_t VertexCount() const;
		uint32_t EdgeCount() const;
	private:

	};

	template <std::unsigned_integral VertexIndexType, std::unsigned_integral EdgeIndexType>
	uint32_t Base<VertexIndexType, EdgeIndexType>::VertexCount() const
	{
		return 0;
	}

	template <std::unsigned_integral VertexIndexType, std::unsigned_integral EdgeIndexType>
	uint32_t Base<VertexIndexType, EdgeIndexType>::EdgeCount() const
	{
		return 0;
	}

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
