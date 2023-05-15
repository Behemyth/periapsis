export module Exception;

import std;

export namespace synodic
{
	class NotImplementedException  final : public std::logic_error
	{
	public:
		NotImplementedException () : std::logic_error("Function is not yet implemented")
		{
		}
	};
}
