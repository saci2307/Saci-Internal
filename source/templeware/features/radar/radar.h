#pragma once
#include "../../config/config.h"
#include "../../utils/math/vector/vector.h"
#include <unordered_map>
#include <string>

struct RadarMapSettings {
    float mapScale;
    float mapOffsetX;
    float mapOffsetY;
    float radarSize;
};

namespace Radar {
    void LoadTextures();
    void SetCurrentMap(const std::string& mapName);
    void Draw(const Vector_t& localPos, float localYaw);
    std::string GetCurrentMapName();
    std::string GetRadarTextureNameForCurrentHeight(const Vector_t& localPos);
}
