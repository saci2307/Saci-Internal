#include "trigger.h"
#include "../../../templeware/config/config.h"
#include "../../../templeware/interfaces/interfaces.h"
#include "../../../cs2/entity/CCSPlayerController/CCSPlayerController.h"
#include "../../../cs2/entity/C_CSPlayerPawn/C_CSPlayerPawn.h"
#include "../../../templeware/utils/schema/schema.h"
#include "../../../../external/imgui/imgui.h"

#include <Windows.h>
#include <thread>
#include <chrono>


CCSPlayerController* GetLocalControllerByIteration() {
    int maxEntities = I::GameEntity->Instance->GetHighestEntityIndex();

    for (int i = 1; i <= maxEntities; i++) {
        auto ent = I::GameEntity->Instance->Get(i);
        if (!ent)
            continue;

        SchemaClassInfoData_t* classInfo = nullptr;
        ent->dump_class_info(&classInfo);
        if (!classInfo)
            continue;

        if (HASH(classInfo->szName) == HASH("CCSPlayerController")) {
            auto controller = reinterpret_cast<CCSPlayerController*>(ent);
            if (controller->IsLocalPlayer())
                return controller;
        }
    }
    return nullptr;
}

void Triggerbot() {
    if (!Config::Triggerbot || !triggerToggled)
        return;

    auto localController = GetLocalControllerByIteration();
    if (!localController)
        return;

    int pawnIndex = localController->m_hPawn().index();
    if (pawnIndex <= 0 || pawnIndex > I::GameEntity->Instance->GetHighestEntityIndex())
        return;

    auto localPlayer = reinterpret_cast<C_CSPlayerPawn*>(I::GameEntity->Instance->Get(pawnIndex));
    if (!localPlayer || localPlayer->getHealth() <= 0)
        return;

    int targetIndex = *(int*)((uintptr_t)localPlayer + 0x1458);
    if (targetIndex <= 0 || targetIndex > I::GameEntity->Instance->GetHighestEntityIndex())
        return;

    auto targetEntity = I::GameEntity->Instance->Get(targetIndex);
    if (!targetEntity)
        return;

    SchemaClassInfoData_t* classInfo = nullptr;
    targetEntity->dump_class_info(&classInfo);
    if (!classInfo || HASH(classInfo->szName) != HASH("C_CSPlayerPawn"))
        return;

    auto targetPawn = reinterpret_cast<C_CSPlayerPawn*>(targetEntity);
    if (targetPawn->getHealth() <= 0)
        return;
    if (targetPawn->getTeam() == localPlayer->getTeam())
        return;

    if (Config::Triggerdelay > 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(Config::Triggerdelay));

    INPUT input = {};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &input, sizeof(INPUT));

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &input, sizeof(INPUT));
}

void StartTriggerbotThread() {
    std::thread([]() {
        while (true) {
            // Handle toggle key
            bool keyDown = (GetAsyncKeyState(Config::Triggerbotkey) & 1);
            if (keyDown && !lastKeyState) {
                triggerToggled = !triggerToggled;
            }
            lastKeyState = keyDown;

            Triggerbot();

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }).detach();
}

bool triggerToggled = false;
bool lastKeyState = false;

void UpdateTriggerToggle() {
    short keyState = GetAsyncKeyState(Config::Triggerbotkey) & 0x8000;

    if (keyState && !lastKeyState) {
        triggerToggled = !triggerToggled;
    }

    lastKeyState = keyState;
}

void TriggerbotIndicator() {
    if (triggerToggled && Config::Triggerbot) {
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();

        // Exemplo: canto superior esquerdo (x=10, y=10), cor vermelha
        drawList->AddText(
            ImVec2(10, 10),
            IM_COL32(255, 0, 0, 255),
            "T"
        );
    }
}

