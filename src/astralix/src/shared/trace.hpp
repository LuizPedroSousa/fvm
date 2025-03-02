#pragma once

#ifdef ASTRA_TRACE
#include <tracy/Tracy.hpp>
#include <tracy/TracyC.h>
#endif

namespace astralix {

#ifdef ASTRA_TRACE

#define ASTRA_PROFILE() ZoneScoped
#define ASTRA_PROFILE_N(name, ...) ZoneScopedN(name)

// TracyMessage(__VA_ARGS__, strlen(__VA_ARGS__))
#define ASTRA_PROFILE_CHILD(name) ZoneScopedN(name) // For explicit child spans
#define ASTRA_PROFILE_BEGIN(name) TracyCZoneN(__astra_zone, name, true)
#define ASTRA_PROFILE_END() TracyCZoneEnd(__astra_zone)
#else
#define ASTRA_PROFILE(name, ...)
#define ASTRA_PROFILE_N(name, ...)
#define ASTRA_PROFILE_CHILD(name)
#define ASTRA_PROFILE_BEGIN(name)
#define ASTRA_PROFILE_END()
#endif

} // namespace astralix
