#pragma once
#include "../../math/vector/vector.h"  // Ajuste o path conforme sua estrutura

#define CONTENTS_HITBOX 0x40000000
#define MASK_SHOT 0x600400B

class IHandleEntity;

class Ray_t
{
public:
    Vector_t start;
    Vector_t delta;

    void Init(const Vector_t& start_, const Vector_t& end_)
    {
        start = start_;
        delta = end_ - start_;
    }
};

class CTraceFilterSimple
{
public:
    void* skip;

    void SetPassEntity(void* entity)
    {
        skip = entity;
    }

    bool ShouldHitEntity(void* entity, int contentsMask)
    {
        return entity != skip;
    }
};

struct CGameTrace_t
{
    Vector_t startpos;
    Vector_t endpos;
    float fraction;
    void* hit_entity;

    bool Hit() const
    {
        return (fraction < 1.0f);
    }
};
