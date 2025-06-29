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

    static auto lastShotTime = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();

    // Cooldown entre disparos para evitar flood no SendInput
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastShotTime).count() < 100)
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

    // Opcional: trigger delay antes do tiro (se quiser manter)
    if (Config::Triggerdelay > 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(Config::Triggerdelay));

    // Disparo otimizado (Down + Up no mesmo frame)
    INPUT input[2] = {};
    input[0].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    input[1].type = INPUT_MOUSE;
    input[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    SendInput(2, input, sizeof(INPUT));

    lastShotTime = now;
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
    short keyState = GetAsyncKeyState(Config::Triggerbotkey) & 1;
    if (keyState)
        triggerToggled = !triggerToggled;
}


void TriggerbotIndicator() {
    if (!(triggerToggled && Config::Triggerbot))
        return;

    ImDrawList* drawList = ImGui::GetForegroundDrawList();

    ImVec2 barPos = ImVec2(250, 20);
    ImVec2 barSize = ImVec2(110, 20);
    ImU32 barColor = IM_COL32(255, 255, 0, 180); // amarelo suave com transparência

    // Fundo da barra com cantos arredondados
    drawList->AddRectFilled(barPos, ImVec2(barPos.x + barSize.x, barPos.y + barSize.y), barColor, 5.0f);

    // Borda da barra
    drawList->AddRect(barPos, ImVec2(barPos.x + barSize.x, barPos.y + barSize.y), IM_COL32(255, 0, 0, 255), 5.0f);

    // Texto centralizado
    const char* label = "Triggerbot ON";
    ImVec2 textSize = ImGui::CalcTextSize(label);
    ImVec2 textPos = ImVec2(
        barPos.x + (barSize.x - textSize.x) * 0.5f,
        barPos.y + (barSize.y - textSize.y) * 0.5f
    );
    drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), label);
}