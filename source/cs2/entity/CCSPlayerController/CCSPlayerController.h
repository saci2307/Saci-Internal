#pragma once
#include <cstdint>
#pragma once
#include "../../../templeware/utils/memory/memorycommon.h"
#include "../../../templeware/utils/math/vector/vector.h"
#include "../../../templeware/utils/schema/schema.h"
#include "../C_CSWeaponBase/C_CSWeaponBase.h"
#include <cstdint>
class CCSPlayerController {
public:
	CCSPlayerController(uintptr_t address);
	const char* getName() const;
	uintptr_t getAddress() const;

	SCHEMA_ADD_OFFSET(bool, IsLocalPlayer, 0x6F0);
	SCHEMA_ADD_OFFSET(CBaseHandle, m_hPawn, 0x62C);
	SCHEMA_ADD_OFFSET(const char*, m_sSanitizedPlayerName, 0x770);

private:
	uintptr_t address;
};
