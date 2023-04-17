#pragma once

#include <concepts>
#include <span>

namespace synodic::graph
{
	/**
	 * \brief Edge
	 * \tparam VertexIndexType The index type to refer to vertices
	 */
	template<std::unsigned_integral VertexIndexType>
	class Edge
	{
	public:
		using VertexType = VertexIndexType;

		VertexIndexType head;
		VertexIndexType tail;
	};

	using Edge8	 = Edge<uint8_t>;
	using Edge16 = Edge<uint16_t>;
	using Edge32 = Edge<uint32_t>;
	using Edge64 = Edge<uint64_t>;

	/**
	 * \brief The base implementation details for all graphs
	 * \tparam VertexIndexType The vertex index type
	 * \tparam EdgeIndexType The edge index type
	 */
	template<std::unsigned_integral VertexIndexType, std::unsigned_integral EdgeIndexType>
	class Base
	{
	public:
		constexpr Base();

		[[nodiscard]] uint32_t VertexCount() const;
		[[nodiscard]] uint32_t EdgeCount() const;

	protected:
	};

	template<std::unsigned_integral VertexIndexType, std::unsigned_integral EdgeIndexType>
	constexpr Base<VertexIndexType, EdgeIndexType>::Base()
	{
	}

	template<std::unsigned_integral VertexIndexType, std::unsigned_integral EdgeIndexType>
	uint32_t Base<VertexIndexType, EdgeIndexType>::VertexCount() const
	{
		return 0;
	}

	template<std::unsigned_integral VertexIndexType, std::unsigned_integral EdgeIndexType>
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

		/**
		 * \brief A directed graph
		 * \tparam VertexIndexType The vertex index type
		 * \tparam EdgeIndexType The edge index type
		 */
		template<std::unsigned_integral VertexIndexType, std::unsigned_integral EdgeIndexType>
		class GraphBuilder : public Base<VertexIndexType, EdgeIndexType>
		{
		public:
			constexpr GraphBuilder();

			Graph<VertexIndexType, EdgeIndexType> Build() const;
		};

		template<std::unsigned_integral VertexIndexType, std::unsigned_integral EdgeIndexType>
		Graph<VertexIndexType, EdgeIndexType> GraphBuilder<VertexIndexType, EdgeIndexType>::Build() const
		{
			return Graph<VertexIndexType, EdgeIndexType>();
		}



		template<std::unsigned_integral VertexIndexType, std::unsigned_integral EdgeIndexType>
		constexpr GraphBuilder<VertexIndexType, EdgeIndexType>::GraphBuilder() :
			Base<VertexIndexType, EdgeIndexType>()
		{
		}

		template<std::unsigned_integral VertexIndexType, std::unsigned_integral EdgeIndexType, size_t V, size_t E>
		class StaticGraph : public Base<VertexIndexType, EdgeIndexType>
		{

		public:
			explicit constexpr StaticGraph(std::span<const Edge<VertexIndexType>, E> edges);

		private:
			std::array<VertexIndexType, E> head_;
			std::array<EdgeIndexType, V> incidentEdge_;
		};

		template <std::unsigned_integral VertexIndexType, std::unsigned_integral EdgeIndexType, size_t V, size_t E>
		constexpr StaticGraph<VertexIndexType, EdgeIndexType, V, E>::StaticGraph(
			std::span<const Edge<VertexIndexType>, E> edges) :
			head_(),
			incidentEdge_()
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

		template<std::unsigned_integral VertexIndexType, std::unsigned_integral EdgeIndexType>
		constexpr Graph<VertexIndexType, EdgeIndexType>::Graph()
		{
		}
	}
}
