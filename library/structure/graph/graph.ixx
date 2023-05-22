/**
 * \brief Graph structures are independent of any data and therefor rely on indices for independent computation.
 */

export module synodic.library.structure.graph;

import std;

export namespace synodic::graph
{
	/**
	 * \brief Users that want to accept multiple kinds of graphs can expect that the Graph concept is the shared scheme
	 * for all
	 */
	template<typename T>
	concept Graph = true;

	/**
	 * \brief Allows building of the graph via a relationship with a node object
	 */
	template<std::signed_integral NodeIndexType>
	class Node
	{
	public:
		explicit Node();

	private:
		NodeIndexType index_;
	};

	template<std::signed_integral NodeIndexType>
	Node<NodeIndexType>::Node()
	{
	}

	/**
	 * \brief Temporary edge data to facilitate graph description
	 * \tparam NodeIndexType The index type to refer to vertices. Unsigned externally for indices, signed internally for
	 *		representation
	 */
	template<std::unsigned_integral NodeIndexType>
	class Edge
	{
	public:
		using VertexType = NodeIndexType;

		Edge(Node<NodeIndexType> head, Node<NodeIndexType> tail);

	private:
		NodeIndexType head;
		NodeIndexType tail;
	};

	template<std::unsigned_integral NodeIndexType>
	Edge<NodeIndexType>::Edge(Node<NodeIndexType> head, Node<NodeIndexType> tail)
	{
	}

	using Node8	 = Node<std::int8_t>;
	using Node16 = Node<std::int16_t>;
	using Node32 = Node<std::int32_t>;
	using Node64 = Node<std::int64_t>;

	using Edge8	 = Edge<std::uint8_t>;
	using Edge16 = Edge<std::uint16_t>;
	using Edge32 = Edge<std::uint32_t>;
	using Edge64 = Edge<std::uint64_t>;

	/**
	 * \brief The base implementation details for all graphs
	 * \tparam NodeIndexType The vertex index type
	 * \tparam EdgeIndexType The edge index type
	 */
	template<std::signed_integral NodeIndexType, std::signed_integral EdgeIndexType>
	class Base
	{
	public:
		constexpr Base();

		[[nodiscard]] uint32_t VertexCount() const;
		[[nodiscard]] uint32_t EdgeCount() const;

	protected:
	};

	template<std::signed_integral NodeIndexType, std::signed_integral EdgeIndexType>
	constexpr Base<NodeIndexType, EdgeIndexType>::Base()
	{
	}

	template<std::signed_integral NodeIndexType, std::signed_integral EdgeIndexType>
	uint32_t Base<NodeIndexType, EdgeIndexType>::VertexCount() const
	{
		return 0;
	}

	template<std::signed_integral NodeIndexType, std::signed_integral EdgeIndexType>
	uint32_t Base<NodeIndexType, EdgeIndexType>::EdgeCount() const
	{
		return 0;
	}

	/**
	 * \brief The base implementation details for dynamic graphs
	 * \tparam NodeIndexType The vertex index type
	 * \tparam EdgeIndexType The edge index type
	 */
	template<std::signed_integral NodeIndexType, std::signed_integral EdgeIndexType>
	class BaseBuilder
	{
	public:
		constexpr BaseBuilder();

		void PushNode(Node<NodeIndexType> node);

	protected:
	};

	template<std::signed_integral NodeIndexType, std::signed_integral EdgeIndexType>
	void BaseBuilder<NodeIndexType, EdgeIndexType>::PushNode(Node<NodeIndexType> node)
	{
	}

	template<std::signed_integral NodeIndexType, std::signed_integral EdgeIndexType>
	constexpr BaseBuilder<NodeIndexType, EdgeIndexType>::BaseBuilder()
	{
	}

	namespace directed
	{
		/**
		 * \brief A directed graph
		 * \tparam NodeIndexType The vertex index type
		 * \tparam EdgeIndexType The edge index type
		 */
		template<std::signed_integral NodeIndexType, std::signed_integral EdgeIndexType>
		class Graph : public Base<NodeIndexType, EdgeIndexType>
		{
		public:
			constexpr Graph();
		};

		template<std::signed_integral NodeIndexType, std::signed_integral EdgeIndexType>
		constexpr Graph<NodeIndexType, EdgeIndexType>::Graph()
		{
		}

		/**
		 * \brief A directed graph
		 * \tparam NodeIndexType The vertex index type
		 * \tparam EdgeIndexType The edge index type
		 */
		template<std::signed_integral NodeIndexType, std::signed_integral EdgeIndexType>
		class GraphBuilder : public BaseBuilder<NodeIndexType, EdgeIndexType>
		{
		public:
			constexpr GraphBuilder();

			Graph<NodeIndexType, EdgeIndexType> Build() const;
		};

		template<std::signed_integral NodeIndexType, std::signed_integral EdgeIndexType>
		Graph<NodeIndexType, EdgeIndexType> GraphBuilder<NodeIndexType, EdgeIndexType>::Build() const
		{
			return Graph<NodeIndexType, EdgeIndexType>();
		}

		template<std::signed_integral NodeIndexType, std::signed_integral EdgeIndexType>
		constexpr GraphBuilder<NodeIndexType, EdgeIndexType>::GraphBuilder() :
			BaseBuilder<NodeIndexType, EdgeIndexType>()
		{
		}

		template<std::signed_integral NodeIndexType, std::signed_integral EdgeIndexType, size_t N, size_t E>
		class StaticGraph : public Base<NodeIndexType, EdgeIndexType>
		{
		public:
			explicit constexpr StaticGraph(std::span<const Edge<NodeIndexType>, E> edges);

		private:
			/**
			 * \brief Stores the head node index at each edge element. The adjacency list for the node is stored via
			 *		contiguous elements
			 */
			std::array<NodeIndexType, E> head_;

			/**
			 * \brief Stores the first incident edge index at each node element.
			 *		Encoding: Positive sign points outwards and negative sign, inward.
			 */
			std::array<EdgeIndexType, N> incidentEdge_;
		};

		template<std::signed_integral NodeIndexType, std::signed_integral EdgeIndexType, size_t N, size_t E>
		constexpr StaticGraph<NodeIndexType, EdgeIndexType, N, E>::StaticGraph(
			std::span<const Edge<NodeIndexType>, E> edges) :
			head_(),
			incidentEdge_()
		{
		}
	}

	namespace undirected
	{
		/**
		 * \brief A undirected graph
		 * \tparam NodeIndexType The vertex index type
		 * \tparam EdgeIndexType The edge index type
		 */
		template<std::signed_integral NodeIndexType, std::signed_integral EdgeIndexType>
		class Graph : public Base<NodeIndexType, EdgeIndexType>
		{
			constexpr Graph();
		};

		template<std::signed_integral NodeIndexType, std::signed_integral EdgeIndexType>
		constexpr Graph<NodeIndexType, EdgeIndexType>::Graph()
		{
		}
	}
}
