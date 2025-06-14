#include "radar.h"
#include "../../interfaces/interfaces.h"
#include "../../config/config.h"
#include "../../players/players.h"
#include "../visuals/visuals.h"
#include "../../hooks/hooks.h"
#include "../../../../external/imgui/imgui.h"
#include <cmath>

void Radar::Render() {
    if (!Config::Radar)
        return;

    auto localPlayer = H::oGetLocalPlayer(0);
    if (!localPlayer || localPlayer->m_iHealth() <= 0)
        return;

    Vector_t localPos = localPlayer->m_vOldOrigin();

    Vector_t viewAngles = H::oGetViewAngles();
    float localYaw = viewAngles.y;

    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    ImVec2 radarPos = ImVec2(20, 300);   // Posição do radar na tela
    ImVec2 radarSize = ImVec2(200, 200); // Tamanho do radar
    float radarRange = 2000.0f;          // Raio de alcance do radar

    // Fundo do radar
    drawList->AddRectFilled(
        radarPos,
        ImVec2(radarPos.x + radarSize.x, radarPos.y + radarSize.y),
        IM_COL32(20, 20, 20, 180)
    );

    // Borda do radar
    drawList->AddRect(
        radarPos,
        ImVec2(radarPos.x + radarSize.x, radarPos.y + radarSize.y),
        IM_COL32(255, 255, 255, 255)
    );

    for (const auto& player : Esp::cached_players) {
        if (!player.handle.valid() || player.health <= 0)
            continue;

        if (Config::teamCheck && player.team_num == Esp::cached_local.team)
            continue;

        Vector_t delta = player.position - localPos;
        float distance = sqrtf(delta.x * delta.x + delta.y * delta.y);

        if (distance > radarRange)
            continue;

        // Se quiser radar rotativo (baseado no yaw da visão):
        float yawRad = (localYaw) * (3.14159265f / 180.0f);
        float rotatedX = delta.x * cosf(yawRad) - delta.y * sinf(yawRad);
        float rotatedY = delta.x * sinf(yawRad) + delta.y * cosf(yawRad);

        float radarX = radarPos.x + radarSize.x / 2 + (rotatedX / radarRange) * (radarSize.x / 2);
        float radarY = radarPos.y + radarSize.y / 2 + (rotatedY / radarRange) * (radarSize.y / 2);

        drawList->AddCircleFilled(ImVec2(radarX, radarY), 3.0f, IM_COL32(255, 0, 0, 255));
    }
}
