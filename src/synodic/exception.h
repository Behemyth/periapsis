#pragma once

#include <stdexcept>


class NotImplemented final : public std::logic_error
{

public:
	NotImplemented() : std::logic_error("Function is not yet implemented") {}
};