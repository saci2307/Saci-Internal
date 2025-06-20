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

template<typename T>
T Clamp(T value, T min, T max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

#define DEG2RAD(x) ((x) * 0.017453292519943295f)

namespace Radar {

void Render() {
    if (!Config::Radar)
        return;

    if (!ImGui::GetCurrentContext())  // ImGui não iniciado
        return;

    auto localPawn = H::oGetLocalPlayer(0);
    if (!localPawn || localPawn->m_iHealth() <= 0)
        return;

    Vector_t localPos = localPawn->m_vOldOrigin();

    QAngle_t viewAngles{};
    I::EngineClient->get_view_angles(viewAngles);
    float localYaw = viewAngles.y;

    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    if (!drawList)
        return;

    const ImVec2 radarPos = ImVec2(20.0f, 300.0f);
    const ImVec2 radarSize = ImVec2(200.0f, 200.0f);
    const float radarRange = 2500.0f;

    const ImVec2 center = ImVec2(radarPos.x + radarSize.x / 2.0f, radarPos.y + radarSize.y / 2.0f);

    // Fundo
    drawList->AddRectFilled(radarPos, ImVec2(radarPos.x + radarSize.x, radarPos.y + radarSize.y), IM_COL32(20, 20, 20, 200));
    drawList->AddRect(radarPos, ImVec2(radarPos.x + radarSize.x, radarPos.y + radarSize.y), IM_COL32(255, 255, 255, 255));

    // Local Player no centro
    drawList->AddCircleFilled(center, 4.0f, IM_COL32(0, 255, 0, 255));

    for (const auto& player : cached_players) {
        if (!player.handle.valid() || player.health <= 0 || player.handle.index() == INVALID_EHANDLE_INDEX)
            continue;

        if (Config::teamCheck && player.team_num == cached_local.team)
            continue;

        Vector_t delta = player.position - localPos;

        float distance = sqrtf(delta.x * delta.x + delta.y * delta.y);
        if (distance > radarRange)
            continue;

        // Rotacionar baseado no yaw local
        float yawRad = DEG2RAD(-localYaw);
        float rotatedX = delta.x * cosf(yawRad) - delta.y * sinf(yawRad);
        float rotatedY = delta.x * sinf(yawRad) + delta.y * cosf(yawRad);

        float normalizedX = (rotatedX / radarRange) * (radarSize.x / 2.0f);
        float normalizedY = (rotatedY / radarRange) * (radarSize.y / 2.0f);

        ImVec2 playerPos = ImVec2(center.x + normalizedX, center.y + normalizedY);

        // Clamp: Não deixa o ponto sair do radar
        playerPos.x = Clamp(playerPos.x, radarPos.x, radarPos.x + radarSize.x);
        playerPos.y = Clamp(playerPos.y, radarPos.y, radarPos.y + radarSize.y);

        // Desenhar inimigo
        drawList->AddCircleFilled(playerPos, 3.0f, IM_COL32(255, 0, 0, 255));
    }
}

} // namespace Radar