#pragma once

#include <concepts>

namespace synodic::graph
{

	namespace Static
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

		/**
		 * \brief The implementation of a static forward graph
		 * \tparam VertexIndexType The vertex index type
		 * \tparam EdgeIndexType The edge index type
		 */
		template<std::unsigned_integral VertexIndexType, std::unsigned_integral EdgeIndexType>
		class Directional : public Base<VertexIndexType, EdgeIndexType>
		{
		public:
			constexpr Directional();
		};
	}

	namespace dynamic
	{

		/**
		 * \brief The base implementation details for dynamic graphs
		 * \tparam VertexIndexType The vertex index type
		 * \tparam EdgeIndexType The edge index type
		 */
		template<std::unsigned_integral VertexIndexType, std::unsigned_integral EdgeIndexType>
		class DynamicBase : public Static::Base<VertexIndexType, EdgeIndexType>
		{
		};

		/**
		 * \brief A forward graph
		 * \tparam VertexIndexType The vertex index type
		 * \tparam EdgeIndexType The edge index type
		 */
		template<std::unsigned_integral VertexIndexType, std::unsigned_integral EdgeIndexType>
		class Directional : public DynamicBase<VertexIndexType, EdgeIndexType>
		{
		};

		/**
		 * \brief The base implementation details for all graphs
		 * \tparam VertexIndexType The vertex index type
		 * \tparam EdgeIndexType The edge index type
		 */
		template<std::unsigned_integral VertexIndexType, std::unsigned_integral EdgeIndexType>
		class Unidirectional : public DynamicBase<VertexIndexType, EdgeIndexType>
		{
		};
	}
}
