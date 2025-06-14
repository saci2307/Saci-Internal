#include "radar.h"
#include "../../interfaces/interfaces.h"
#include "../../interfaces/IEngineClient/IEngineClient.h"
#include "../../config/config.h"
#include "../../players/players.h"
#include "../visuals/visuals.h"
#include "../../hooks/hooks.h"
#include "../../../../external/imgui/imgui.h"
#include <cmath>

#define DEG2RAD(x) ((x) * 0.017453292519943295f)

void Radar::Render() {
    if (!Config::Radar)
        return;

    auto localPlayer = H::oGetLocalPlayer(0);
    if (!localPlayer || localPlayer->m_iHealth() <= 0)
        return;

    Vector_t localPos = localPlayer->m_vOldOrigin();

    // Substituir por leitura via interfaces
    QAngle_t viewAngles;
    I::EngineClient->get_view_angles(viewAngles);
    float localYaw = viewAngles.y;


    ImDrawList* drawList = ImGui::GetForegroundDrawList();
    ImVec2 radarPos = ImVec2(20, 300);
    ImVec2 radarSize = ImVec2(200, 200);
    float radarRange = 2500.0f;

    drawList->AddRectFilled(radarPos, ImVec2(radarPos.x + radarSize.x, radarPos.y + radarSize.y), IM_COL32(20, 20, 20, 180));
    drawList->AddRect(radarPos, ImVec2(radarPos.x + radarSize.x, radarPos.y + radarSize.y), IM_COL32(255, 255, 255, 255));
    drawList->AddCircleFilled(ImVec2(radarPos.x + radarSize.x / 2, radarPos.y + radarSize.y / 2), 4.0f, IM_COL32(0, 255, 0, 255));

    for (const auto& player : cached_players) {
        if (!player.handle.valid() || player.health <= 0)
            continue;

        if (Config::teamCheck && player.team_num == cached_local.team)
            continue;

        Vector_t delta = player.position - localPos;
        float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);
        if (distance > radarRange)
            continue;

        float yawRad = DEG2RAD(localYaw);
        float rotatedX = delta.x * cosf(yawRad) - delta.y * sinf(yawRad);
        float rotatedY = delta.x * sinf(yawRad) + delta.y * cosf(yawRad);

        float radarX = radarPos.x + radarSize.x / 2 + (rotatedX / radarRange) * (radarSize.x / 2);
        float radarY = radarPos.y + radarSize.y / 2 + (rotatedY / radarRange) * (radarSize.y / 2);

        drawList->AddCircleFilled(ImVec2(radarX, radarY), 3.0f, IM_COL32(255, 0, 0, 255));
    }
}
