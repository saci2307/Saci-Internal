#pragma once

#include "../../../templeware/utils/math/vector/vector.h"
#include "../../../templeware/offsets/offsets.h"

class C_C4Planted {
public:
    uintptr_t address;

    C_C4Planted(uintptr_t addr) : address(addr) {}

    bool m_bBombTicking() const {
        return *reinterpret_cast<bool*>(address + Offset::C_PlantedC4::m_bBombTicking);
    }

    float m_flC4Blow() const {
        return *reinterpret_cast<float*>(address + Offset::C_PlantedC4::m_flC4Blow);
    }

    Vector_t m_vecOrigin() const {
        return *reinterpret_cast<Vector_t*>(address + Offset::C_BaseEntity::m_vecOrigin);
    }

    uint8_t m_nBombSite() const {
        return *reinterpret_cast<uint8_t*>(address + Offset::C_PlantedC4::m_nBombSite);
    }
};
