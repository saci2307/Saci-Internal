#pragma once
#include "../../../cs2/entity/C_CSPlayerPawn/C_CSPlayerPawn.h"
#include "../../../templeware/utils/schema/schema.h"
#include "../../../templeware/interfaces/interfaces.h"

inline Vector_t GetEntityEyePos(C_CSPlayerPawn* entity)
{
    if (!entity)
        return {};

    uintptr_t gameSceneNode = *(uintptr_t*)((uintptr_t)entity + SchemaFinder::Get(hash_32_fnv1a_const("C_BaseEntity->m_pGameSceneNode")));
    Vector_t origin = *(Vector_t*)(gameSceneNode + SchemaFinder::Get(hash_32_fnv1a_const("CGameSceneNode->m_vecAbsOrigin")));
    Vector_t viewOffset = *(Vector_t*)((uintptr_t)entity + SchemaFinder::Get(hash_32_fnv1a_const("C_BaseModelEntity->m_vecViewOffset")));

    return origin + viewOffset;
}
