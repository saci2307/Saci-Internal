#pragma once
#include "../../utils/math/vector/vector.h"

struct TraceResult {
    bool didCollide;
    Vector_t collisionNormal;
};

// Simples detecção de colisão com um "mapa em forma de caixa" + chão plano
inline TraceResult TraceRay(const Vector_t& start, const Vector_t& end) {
    TraceResult result{};
    result.didCollide = false;

    // Fake ground collision (Z=0)
    if (end.z <= 0.0f && start.z > 0.0f) {
        result.didCollide = true;
        result.collisionNormal = Vector_t(0, 0, 1); // Normal pointing up
        return result;
    }

    // Fake map boundaries (exemplo de caixa 4000x4000)
    const float worldMin = -4000.0f;
    const float worldMax = 4000.0f;

    if (end.x < worldMin || end.x > worldMax) {
        result.didCollide = true;
        result.collisionNormal = Vector_t(end.x < worldMin ? 1 : -1, 0, 0);
        return result;
    }

    if (end.y < worldMin || end.y > worldMax) {
        result.didCollide = true;
        result.collisionNormal = Vector_t(0, end.y < worldMin ? 1 : -1, 0);
        return result;
    }

    return result;
}
