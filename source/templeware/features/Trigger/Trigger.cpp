#include "../../../cs2/entity/C_CSPlayerPawn/C_CSPlayerPawn.h"
#include "../../../templeware/interfaces/CGameEntitySystem/CGameEntitySystem.h"
#include "../../../templeware/interfaces/interfaces.h"
#include "../../../templeware/config/config.h"
#include "../../../templeware/utils/schema/schema.h"
#include "../../../templeware/features/Trigger/trigger_utils.h"

#include <Windows.h>
#include <chrono>
#include <thread>

void Triggerbot()
{
    if (!Config::Triggerbot)
        return;

    if (!(GetAsyncKeyState(Config::Triggerbotkey) & 0x8000))
        return;

    C_CSPlayerPawn* localPlayer = nullptr;
    int maxEntities = I::GameEntity->Instance->GetHighestEntityIndex();

    int localTeam = -1;

    for (int i = 1; i <= maxEntities; i++) {
        auto ent = I::GameEntity->Instance->Get(i);
        if (!ent)
            continue;

        SchemaClassInfoData_t* classInfo = nullptr;
        ent->dump_class_info(&classInfo);
        if (!classInfo)
            continue;

        if (HASH(classInfo->szName) != HASH("C_CSPlayerPawn"))
            continue;

        auto pawn = reinterpret_cast<C_CSPlayerPawn*>(ent);
        // Assume the first valid pawn is the local player and get its team
        if (pawn->getHealth() > 0 /* add more checks if needed */) {
            localTeam = pawn->getTeam();
            localPlayer = pawn;
            break;
        }
    }

    if (!localPlayer)
        return;

    int targetIndex = *(int*)((uintptr_t)localPlayer + SchemaFinder::Get(hash_32_fnv1a_const("C_CSPlayerPawn->m_iIDEntIndex")));
    if (targetIndex <= 0 || targetIndex > maxEntities)
        return;

    auto target = I::GameEntity->Instance->Get(targetIndex);
    if (!target)
        return;

    SchemaClassInfoData_t* classInfo = nullptr;
    target->dump_class_info(&classInfo);
    if (!classInfo || HASH(classInfo->szName) != HASH("C_CSPlayerPawn"))
        return;

    auto enemyPawn = reinterpret_cast<C_CSPlayerPawn*>(target);

    if (enemyPawn->getHealth() <= 0)
        return;

    if (enemyPawn->getTeam() == localPlayer->getTeam())
        return;

    // Disparo com SendInput
    INPUT input = {};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &input, sizeof(INPUT));

    std::this_thread::sleep_for(std::chrono::milliseconds(Config::Triggerdelay));

    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &input, sizeof(INPUT));
}