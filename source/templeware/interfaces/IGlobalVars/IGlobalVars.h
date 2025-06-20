#pragma once

class IGlobalVars
{
public:
    float realtime;          // 0x00
    int framecount;          // 0x04
    float absoluteframetime; // 0x08
    float curtime;           // 0x0C
    float frametime;         // 0x10
    int maxClients;          // 0x14
    float interval_per_tick; // 0x18
};
