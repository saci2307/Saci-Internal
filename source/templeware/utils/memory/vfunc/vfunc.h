#pragma once

#include <cstddef>
#include <cstdint>

namespace M {
	template <typename T, std::size_t nIndex, class CBaseClass, typename... Args_t>
	static inline T vfunc(CBaseClass* thisptr, Args_t... argList) {
		using VirtualFn_t = T(__thiscall*)(const void*, decltype(argList)...);
		return (*reinterpret_cast<VirtualFn_t* const*>(reinterpret_cast<std::uintptr_t>(thisptr)))[nIndex](thisptr, argList...);
	}
}