#pragma once

namespace synodic
{
	class Structure
	{
	public:
		Structure()			 = default;
		virtual ~Structure() = default;
	};

	class SparseStructure : public Structure
	{
	public:
		SparseStructure()		   = default;
		virtual ~SparseStructure() = default;
	};

}