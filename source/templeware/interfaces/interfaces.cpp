#include "interfaces.h"
#include "CGameEntitySystem/CGameEntitySystem.h"

// @used: I::Get<template>
#include "../../templeware/utils/memory/Interface/Interface.h"
#include "IGlobalVars/IGlobalVars.h"
IGlobalVars* I::GlobalVars = nullptr;

bool I::Interfaces::init()
{
    const HMODULE tier0_base = GetModuleHandleA("tier0.dll");
    if (!tier0_base)
        return false;

    bool success = true;

    // Interfaces principais
    EngineClient = I::Get<IEngineClient>("engine2.dll", "Source2EngineToClient00");
    success &= (EngineClient != nullptr);

    GameEntity = I::Get<IGameResourceService>("engine2.dll", "GameResourceServiceClientV00");
    success &= (GameEntity != nullptr);

    // ✅ Inicialização de GlobalVars - (Substitua o pattern quando souber o correto)
    I::GlobalVars = *reinterpret_cast<IGlobalVars**>(M::patternScan("client", "pattern_to_find_globalvars"));
    if (!I::GlobalVars)
        printf("[Interfaces] Falha ao localizar GlobalVars!\n");
    else
        printf("[Interfaces] GlobalVars localizado em: 0x%p\n", I::GlobalVars);

    // Exports da tier0.dll
    ConstructUtlBuffer = reinterpret_cast<decltype(ConstructUtlBuffer)>(GetProcAddress(tier0_base, "??0CUtlBuffer@@QEAA@HHH@Z"));
    EnsureCapacityBuffer = reinterpret_cast<decltype(EnsureCapacityBuffer)>(GetProcAddress(tier0_base, "?EnsureCapacity@CUtlBuffer@@QEAAXH@Z"));
    PutUtlString = reinterpret_cast<decltype(PutUtlString)>(GetProcAddress(tier0_base, "?PutString@CUtlBuffer@@QEAAXPEBD@Z"));
    CreateMaterial = reinterpret_cast<decltype(CreateMaterial)>(M::FindPattern("materialsystem2.dll", "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 81 EC ? ? ? ? 48 8B 05"));
    LoadKeyValues = reinterpret_cast<decltype(LoadKeyValues)>(GetProcAddress(tier0_base, "?LoadKV3@@YA_NPEAVKeyValues3@@PEAVCUtlString@@PEBDAEBUKV3ID_t@@2@Z"));
    ConMsg = reinterpret_cast<decltype(ConMsg)>(GetProcAddress(tier0_base, "?ConMsg@@YAXPEBDZZ"));
    ConColorMsg = reinterpret_cast<decltype(ConColorMsg)>(GetProcAddress(tier0_base, "?ConColorMsg@@YAXAEBVColor@@PEBDZZ"));

    // Debug info
    printf("[Interfaces] EngineClient: 0x%p\n", reinterpret_cast<void*>(EngineClient));
    printf("[Interfaces] GameEntity: 0x%p\n", reinterpret_cast<void*>(GameEntity));
    printf("[Interfaces] CreateMaterial: 0x%p\n", reinterpret_cast<void*>(CreateMaterial));

    return success;
}
