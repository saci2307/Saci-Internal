#pragma once
#include <cstdint>
#include <d3d11.h>
extern ID3D11Device* g_pd3dDevice;

bool CreateTextureFromFile(const char* filePath, ID3D11ShaderResourceView** outSRV);
