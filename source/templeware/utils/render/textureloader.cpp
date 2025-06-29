#include "textureloader.h"
#include "../../../../external/stb_image/stb_image.h"
#include <d3d11.h>

// Se você já tem uma global do device, mantenha ela. Senão, defina assim:
extern ID3D11Device* g_pd3dDevice;

bool CreateTextureFromFile(const char* filePath, ID3D11ShaderResourceView** outSRV) {
    int width, height, channels;
    unsigned char* imageData = stbi_load(filePath, &width, &height, &channels, 4);
    if (!imageData)
        return false;

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = imageData;
    initData.SysMemPitch = width * 4;

    ID3D11Texture2D* texture = nullptr;
    if (FAILED(g_pd3dDevice->CreateTexture2D(&desc, &initData, &texture))) {
        stbi_image_free(imageData);
        return false;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    if (FAILED(g_pd3dDevice->CreateShaderResourceView(texture, &srvDesc, outSRV))) {
        texture->Release();
        stbi_image_free(imageData);
        return false;
    }

    texture->Release();
    stbi_image_free(imageData);
    return true;
}
