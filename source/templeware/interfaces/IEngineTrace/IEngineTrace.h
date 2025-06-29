#pragma once
#include "../../utils/math/vector/vector.h"

struct Ray_t {
    Vector_t start;
    Vector_t delta;
    Vector_t startOffset;
    Vector_t extents;
    bool isRay;
    bool isSwept;

    Ray_t() = default;

    Ray_t(const Vector_t& startPos, const Vector_t& endPos) {
        delta = endPos - startPos;
        isSwept = (delta.Length() != 0);
        extents = Vector_t(0, 0, 0);
        startOffset = Vector_t(0, 0, 0);
        start = startPos;
        isRay = true;
    }
};

class ITraceFilter {
public:
    virtual bool ShouldHitEntity(void* pEntity, int contentsMask) = 0;
    virtual int GetTraceType() const = 0;
};

class TraceFilterSkipEntity : public ITraceFilter {
public:
    void* skipEntity;

    TraceFilterSkipEntity(void* skip) : skipEntity(skip) {}

    bool ShouldHitEntity(void* pEntity, int contentsMask) override {
        return pEntity != skipEntity;
    }

    int GetTraceType() const override {
        return 0; // TRACE_EVERYTHING
    }
};

struct trace_t {
    Vector_t startPos;
    Vector_t endPos;
    Vector_t planeNormal;
    float fraction;
    int contents;
    bool allSolid;
    bool startSolid;
    void* hitEntity;
};

class IEngineTrace {
public:
    virtual void TraceRay(const Ray_t& ray, unsigned int mask, ITraceFilter* filter, trace_t* trace) = 0;
};
