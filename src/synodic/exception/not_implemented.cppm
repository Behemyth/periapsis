#pragma once

export module Exception;

import std;

export namespace synodic
{
	class NotImplemented final : public std::logic_error
	{
	public:
		NotImplemented() : std::logic_error("Function is not yet implemented")
		{
		}
	};
}
