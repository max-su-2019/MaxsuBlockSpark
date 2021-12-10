#pragma once

#include "effolkronium/random.hpp"
using Random = effolkronium::random_static;

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

#include "DKUtil/Logger.hpp"

#include "Version.h"


using namespace std::literals;

namespace logger = SKSE::log;

#define DLLEXPORT __declspec(dllexport)
