#include "grenadeprediction.h"
#include "../../config/config.h"
#include "../../interfaces/interfaces.h"
#include "../../../cs2/entity/C_CSPlayerPawn/C_CSPlayerPawn.h"
#include "../../utils/math/vector/vector.h"
#include "../../../../external/imgui/imgui.h"
#include "../visuals/visuals.h"
#include "../../hooks/hooks.h"
#include "../../renderer/renderer.h"
#include "../../utils/physics/trace.h"
#include "../../utils/physics/masks.h"
#include <vector>
#include <cmath>
#include "../../../templeware/templeware.h"
extern TempleWare templeWare;

enum class GrenadeType {
    HE,
    Flashbang,
    Smoke,
    Molotov,
    Decoy,
    Unknown
};

GrenadeType GetGrenadeType(C_CSPlayerPawn* localPlayer) {
    auto weapon = localPlayer->GetActiveWeapon();
    if (!weapon)
        return GrenadeType::Unknown;

    int weaponId = weapon->m_iItemDefinitionIndex();

    switch (weaponId) {
    case 43: return GrenadeType::Flashbang;
    case 44: return GrenadeType::HE;
    case 45: return GrenadeType::Smoke;
    case 46: return GrenadeType::Molotov;
    case 47: return GrenadeType::Decoy;
    default: return GrenadeType::Unknown;
    }
}

struct GrenadePathPoint {
    Vector_t position;
    bool didCollide;
};

static std::vector<GrenadePathPoint> grenadePath;

void GrenadePrediction::Reset() {
    grenadePath.clear();
}

// Simples colisão com "mundo"
bool TraceCollisions(Vector_t& pos, Vector_t& velocity) {
    // Aqui você pode futuramente fazer um raytrace real com entidades do mapa

    // Fake chão (ground level Z = 0)
    if (pos.z < 0.0f) {
        pos.z = 0.0f;
        velocity.z *= -0.45f;  // Perda de energia na colisão vertical
        velocity.x *= 0.6f;
        velocity.y *= 0.6f;
        return true;
    }

    return false;
}

bool ShouldDetonate(GrenadeType type, float time, const Vector_t& velocity, const Vector_t& pos, const trace_t& tr) {
    switch (type) {
    case GrenadeType::HE:
    case GrenadeType::Flashbang:
        return time > 1.5f;
    case GrenadeType::Smoke:
    case GrenadeType::Decoy:
        return time > 3.0f;
    case GrenadeType::Molotov:
        return tr.fraction < 1.0f; // Explode ao tocar
    default:
        return false;
    }
}

void SimulatePhysics(Vector_t& position, Vector_t& velocity, GrenadeType type, C_CSPlayerPawn* localPawn) {

    constexpr float intervalPerTick = 1.0f / 64.0f;
    constexpr float gravity = 800.0f * intervalPerTick * intervalPerTick;
    constexpr int maxSteps = 512;
    constexpr float collisionLoss = 0.45f;

    float timeSimulated = 0.0f;

    for (int step = 0; step < maxSteps; ++step) {

        Vector_t nextPos = position + velocity * intervalPerTick;

        Ray_t ray(position, nextPos);
        TraceFilterSkipEntity filter(localPawn);
        trace_t tr;

        I::EngineTrace->TraceRay(ray, MASK_SOLID, &filter, &tr);

        if (tr.fraction < 1.0f) {
            float dot = velocity.x * tr.planeNormal.x + velocity.y * tr.planeNormal.y + velocity.z * tr.planeNormal.z;
            Vector_t reflect = velocity - (tr.planeNormal * (2.0f * dot));
            velocity = reflect * collisionLoss;

            grenadePath.push_back({ nextPos, true });

            if (ShouldDetonate(type, timeSimulated, velocity, nextPos, tr))
                break;

            if (velocity.Length() < 15.f)
                break;
        }
        else {
            grenadePath.push_back({ nextPos, false });
            position = nextPos;
        }

        velocity.z -= gravity;
        timeSimulated += intervalPerTick;

        if (ShouldDetonate(type, timeSimulated, velocity, position, tr))
            break;
    }
}

void GrenadePrediction::Simulate() {
    Reset();

    auto localPawn = H::oGetLocalPlayer(0);
    if (!localPawn || localPawn->getHealth() <= 0)
        return;

    Vector_t startPos = localPawn->m_vOldOrigin() + localPawn->m_vecViewOffset();

    QAngle_t viewAngles;
    I::EngineClient->get_view_angles(viewAngles);

    Vector_t throwDir;
    viewAngles.AngleVectors(&throwDir);

    float throwStrength = 1.0f;

    Vector_t velocity = throwDir * (750.0f * throwStrength);

    GrenadeType type = GetGrenadeType(localPawn);

    SimulatePhysics(startPos, velocity, type, localPawn); // <-- Passe o localPawn
}


void GrenadePrediction::Draw() {
    if (grenadePath.empty())
        return;

    ImDrawList* drawList = ImGui::GetBackgroundDrawList();

    ImU32 pathColor = IM_COL32(255, 255, 0, 255);    // Cor da trilha (amarelo)
    ImU32 collisionColor = IM_COL32(255, 0, 0, 255); // Cor nos pontos de colisão (vermelho)

    Vector_t previousScreenPos;
    bool hasLastPos = false;

    for (const auto& point : grenadePath) {
        Vector_t screenPos;

        if (!templeWare.renderer.visuals.viewMatrix.WorldToScreen(point.position, screenPos))
            continue;

        // Desenhar linha entre os pontos
        if (hasLastPos) {
            drawList->AddLine(
                ImVec2(previousScreenPos.x, previousScreenPos.y),
                ImVec2(screenPos.x, screenPos.y),
                pathColor,
                2.0f
            );
        }

        // Se houve colisão nesse ponto, desenha um círculo
        if (point.didCollide) {
            drawList->AddCircleFilled(
                ImVec2(screenPos.x, screenPos.y),
                4.0f,
                collisionColor
            );
        }

        previousScreenPos = screenPos;
        hasLastPos = true;
    }
}