#pragma once
#include <cstdint>
#include "../cutlbuffer/cutlbuffer.h"
#include "keyvalues.h"
#include "../../../templeware/utils/memory/vfunc/vfunc.h"
#include "../../../templeware/utils/memory/patternscan/patternscan.h"
#include "../../../templeware/utils/memory/memorycommon.h"
#include "../cutl/utlhash/utlhash.h"
#include "..\..\..\templeware\utils\math\utlvector\utlvector.h"
#include "..\..\..\templeware\interfaces\interfaces.h"



void CKeyValues3::LoadFromBuffer(const char* szString)
{
	CUtlBuffer buffer(0, (std::strlen(szString) + 10), 1);

	buffer.PutString(szString);
	this->LoadKV3(&buffer);
}

bool CKeyValues3::LoadKV3(CUtlBuffer* buffer)
{
	using fnLoadKeyValues = bool(__fastcall*)(CKeyValues3*, void*, CUtlBuffer*, KV3ID_t*, void*, void*, void*, void*, const char*);
	static const fnLoadKeyValues oLoadKeyValues = reinterpret_cast<fnLoadKeyValues>(M::abs(M::FindPattern("tier0.dll", "E8 ? ? ? ? EB 36 8B 43 10"), 0x1, 0x0));

	const char* szName = ("");
	KV3ID_t kv3ID = KV3ID_t(("generic"), 0x41B818518343427E, 0xB5F447C23C0CDF8C);
	return oLoadKeyValues(this, nullptr, buffer, &kv3ID, nullptr, nullptr, nullptr, nullptr, (""));
}

CKeyValues3* CKeyValues3::create_material_from_resource()
{
	using fnSetTypeKV3 = CKeyValues3 * (__fastcall*)(CKeyValues3*, unsigned int, unsigned int);
	static const fnSetTypeKV3 oSetTypeKV3 = reinterpret_cast<fnSetTypeKV3>(M::FindPattern("tier0.dll", ("40 53 48 83 EC 30 48 8B D9 49")));


	CKeyValues3* pKeyValue = new CKeyValues3[0x10];
	return oSetTypeKV3(pKeyValue, 1U, 6U);
}