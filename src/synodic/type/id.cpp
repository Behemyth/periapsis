
#include "id.h"

#include <atomic>

std::atomic<size_t> ClassID::counter_{0};
