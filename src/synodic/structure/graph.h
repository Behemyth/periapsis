#pragma once

#include <concepts>
#include <span>

namespace synodic::graph
{
	/**
	 * \brief Vertex
	 * \tparam T Data to be associated with each vertex
	 */
	template<typename T>
	class VertexData
	{
		
	};

	/**
	 * \brief Edge
	 * \tparam T Data to be associated with each edge
	 */
	template<typename T>
	class EdgeData
	{
		
	};

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

	using Edge8 = Edge<uint8_t>;
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

		template<std::unsigned_integral VertexIndexType, std::unsigned_integral EdgeIndexType>
		constexpr Graph<VertexIndexType, EdgeIndexType>::Graph() :
			Base<VertexIndexType, EdgeIndexType>()
		{
		}

		template<std::unsigned_integral VertexIndexType, std::unsigned_integral EdgeIndexType, size_t N>
		class StaticGraph : public Base<VertexIndexType, EdgeIndexType>
		{
		public:
			constexpr StaticGraph(std::span<const Edge<VertexIndexType>, N> input);

		private:

			static constexpr size_t size = N; 

			std::array<EdgeIndexType, size> head_;
			std::array<EdgeIndexType, size> incidentEdge_;

		};

		template <std::unsigned_integral VertexIndexType, std::unsigned_integral EdgeIndexType, size_t N>
		constexpr StaticGraph<VertexIndexType, EdgeIndexType, N>::StaticGraph(
			std::span<const Edge<VertexIndexType>, N> input)
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
