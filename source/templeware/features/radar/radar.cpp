#include "radar.h"
#include "../../interfaces/interfaces.h"
#include "../visuals/visuals.h"
#include <unordered_map>
#include <string>
#include <d3d11.h>
#include <fstream>
#include <vector>
#include "../../utils/render/textureloader.h"
#include <iostream>

namespace Radar {
    std::string currentMap = "de_dust2";
}

std::string Radar::GetCurrentMapName() {
    const char* fullName = I::EngineClient->get_level_name();
    if (!fullName || strlen(fullName) < 5)
        return "unknown";

    std::string mapName(fullName);

    // Exemplo: "maps/de_dust2.bsp" → "de_dust2"
    size_t start = mapName.find_last_of('/');
    size_t end = mapName.find(".bsp");

    if (start == std::string::npos || end == std::string::npos || end <= start + 1)
        return "unknown";

    return mapName.substr(start + 1, end - start - 1);
}

std::string Radar::GetRadarTextureNameForCurrentHeight(const Vector_t& localPos) {
if (currentMap == "de_vertigo") {
    return (localPos.z < 1500.0f) ? "de_vertigo_lower" : "de_vertigo";
}
else if (currentMap == "de_nuke") {
    return (localPos.z < 200.0f) ? "de_nuke_lower" : "de_nuke";
}
return currentMap;
}

// Config de cada mapa
static std::unordered_map<std::string, RadarMapSettings> mapConfigs = {
    {"de_vertigo", {9.9f, -1650.f, 50.f, 235.f}},
    {"de_mirage",  {14.f, -650.f, -850.f, 300.f}},
    {"de_inferno", {15.f, 450.f, 1450.f, 300.f}},
    {"de_dust2",   {14.8f, -250.f, 950.f, 300.f}},
    {"de_anubis",  {13.f, -100.f, 600.f, 300.f}},
    {"de_ancient", {13.f, -450.f, -450.f, 300.f}},
    {"de_nuke",    {12.65f, 450.f, -680.f, 300.f}},
    {"de_train",   {12.4f, 30.f, 20.f, 300.f}}
};

static std::unordered_map<std::string, ImTextureID> loadedTextures;

ImTextureID LoadTextureRaw(const std::string& path) {
    ID3D11ShaderResourceView* textureView = nullptr;
    if (!CreateTextureFromFile(path.c_str(), &textureView))
        return nullptr;
        std::cout << "Failed to load texture: " << path << "\n";
    return (ImTextureID)textureView;
    
}

void Radar::SetCurrentMap(const std::string& mapName) {
    if (mapConfigs.find(mapName) != mapConfigs.end())
        currentMap = mapName;
}

void Radar::LoadTextures() {
    for (const auto& [mapName, _] : mapConfigs) {
        std::vector<std::string> variants = { mapName };

        if (mapName == "de_vertigo" || mapName == "de_nuke") {
            variants.push_back(mapName + "_lower");
        }

        for (const auto& variant : variants) {
            std::string path = "../../../external/radars/" + variant + "_radar.png";
            loadedTextures[variant] = LoadTextureRaw(path);
        }
    }
}

ImVec2 WorldToRadar(const Vector_t& pos, const RadarMapSettings& settings, ImVec2 radarPos) {
    float scale = 1.0f / settings.mapScale;
    float x = (pos.x - settings.mapOffsetX) * scale;
    float y = (pos.y - settings.mapOffsetY) * scale;

    float centerX = radarPos.x + settings.radarSize / 2.0f;
    float centerY = radarPos.y + settings.radarSize / 2.0f;

    return ImVec2(centerX + x, centerY - y);
}

void Radar::Draw(const Vector_t& localPos, float localYaw) {

    std::string detectedMap = GetCurrentMapName();
    if (mapConfigs.find(detectedMap) != mapConfigs.end()) {
        Radar::currentMap = detectedMap;
    }

    if (!Config::Radar || loadedTextures.find(currentMap) == loadedTextures.end())
        return;

    const RadarMapSettings& settings = mapConfigs[currentMap];
    std::string textureName = GetRadarTextureNameForCurrentHeight(localPos);
    ImTextureID texture = loadedTextures[textureName];
    if (!texture)
        return;
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    ImVec2 radarPos(25, 25);

    // Radar background
    drawList->AddImage(texture, radarPos, ImVec2(radarPos.x + settings.radarSize, radarPos.y + settings.radarSize));

    // Players
    for (const auto& player : cached_players) {
        if (!player.handle.valid() || player.health <= 0)
            continue;

        ImVec2 dotPos = WorldToRadar(player.position, settings, radarPos);
        ImU32 color = (player.team_num == cached_local.team) ? IM_COL32(0, 255, 0, 255) : IM_COL32(255, 0, 0, 255);
        drawList->AddCircleFilled(dotPos, 4.0f, color);
    }

    // Local Player
    ImVec2 selfPos = WorldToRadar(localPos, settings, radarPos);
    drawList->AddTriangleFilled(
        ImVec2(selfPos.x, selfPos.y - 6),
        ImVec2(selfPos.x - 4, selfPos.y + 4),
        ImVec2(selfPos.x + 4, selfPos.y + 4),
        IM_COL32(255, 255, 255, 255)
    );
}
