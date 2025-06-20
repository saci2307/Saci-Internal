#include "vector.h"
#include <cmath>

void QAngle_t::AngleVectors(Vector_t* forward) const
{
    if (!forward)
        return;

    const float degToRad = 3.14159265358979323846f / 180.0f;

    float pitchRad = x * degToRad;
    float yawRad = y * degToRad;

    float cp = std::cosf(pitchRad);
    float sp = std::sinf(pitchRad);
    float cy = std::cosf(yawRad);
    float sy = std::sinf(yawRad);

    forward->x = cp * cy;
    forward->y = cp * sy;
    forward->z = -sp;
}
