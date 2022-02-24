#pragma once

extern "C" EXPORT ImTextureID load_texture(const unsigned char* image_data,
                                           int image_width,
                                           int image_height)
{
    // Create texture
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width            = image_width;
    desc.Height           = image_height;
    desc.MipLevels        = 1;
    desc.ArraySize        = 1;
    desc.Format           = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage            = D3D11_USAGE_DEFAULT;
    desc.BindFlags        = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags   = 0;

    ID3D11Texture2D* pTexture = NULL;
    D3D11_SUBRESOURCE_DATA subResource;
    subResource.pSysMem          = image_data;
    subResource.SysMemPitch      = desc.Width * 4;
    subResource.SysMemSlicePitch = 0;
    g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

    // Create texture view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format                    = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels       = desc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    ID3D11ShaderResourceView* out_srv = nullptr;
    g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, &out_srv);
    pTexture->Release();

    return out_srv;
}

extern "C" EXPORT void unload_texture(ImTextureID texture)
{
    auto image_texture = static_cast<ID3D11ShaderResourceView*>(texture);
    image_texture->Release();
}