#pragma once

#include <stdexcept>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <memory>
#include <string>
#include <functional>
#include <vector>
#include <array>
#include <iostream>
#include <chrono>
#include <map>
#include <numeric>

#include <typeindex>
#include <typeinfo>

// Logger
#include "Core/Log.h"

#ifdef DEBUG
#define ASSERT(x) {if (!x) {L_ERROR("ASSERTION FAILED"); __debugbreak();} };
#else
#define ASSERT(x)
#endif // DEBUG
