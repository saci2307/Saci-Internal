#pragma once
#include <cstddef>

namespace Offset {
	constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x188BF30;

	namespace C_BasePlayerPawn {
		constexpr std::ptrdiff_t m_vOldOrigin = 0x1324;
		constexpr std::ptrdiff_t m_iHealth = 0x344;
		constexpr std::ptrdiff_t m_iTeamNum = 0x3E3;
		constexpr std::ptrdiff_t m_vecViewOffset = 0xCB0; 
	}
	namespace C_CSWeaponBaseGun {
    constexpr std::ptrdiff_t m_iItemDefinitionIndex = 0x1BA; 
    constexpr std::ptrdiff_t m_pGameSceneNode = 0x310; 
	}

	namespace C_GameSceneNode {
		constexpr std::ptrdiff_t m_vecAbsOrigin = 0xC8; 
	}
}