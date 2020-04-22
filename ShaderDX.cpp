//--------------------------------------------------------------------------------------
// File: ShaderDX.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "DXUT.h"
#include "resource.h"
#include <iostream>
#include "SDKmisc.h"
using namespace std;

#define PI           3.14159265f
#define FOV          (PI/4.0f)                     // 시야각
#define ASPECT_RATIO (WIN_WIDTH/(float)WIN_HEIGHT) // 화면의 종횡비
#define NEAR_PLANE   1                             // 근접 평면
#define FAR_PLANE    10000                         // 원거리 평면
#define WIN_WIDTH  800
#define WIN_HEIGHT  600

ID3DXEffect* g_pEffect = NULL;       // D3DX effect interface
ID3DXMesh* g_pMesh = NULL;         // Mesh object
//IDirect3DTexture9* g_pMeshTexture = NULL;  // Mesh texture
// 빛의 위치
D3DXVECTOR4                gWorldLightPosition(500.0f, 500.0f, -500.0f, 1.0f);
// 카메라 위치
D3DXVECTOR4                gWorldCameraPosition(0.0f, 0.0f, -200.0f, 1.0f);

HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                     void* pUserContext )
{
    DWORD flag = 0;
#if _DEBUG
    flag |= D3DXSHADER_DEBUG;
#endif
    WCHAR str[MAX_PATH];
    (DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"PixelShader.fx"));
    (D3DXCreateEffectFromFile(pd3dDevice, str, NULL, NULL, flag, NULL, &g_pEffect, NULL));
    (DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"pixellight.x"));
    (D3DXLoadMeshFromX(str, D3DXMESH_MANAGED, pd3dDevice, NULL, NULL, NULL, NULL, &g_pMesh));
 /*   (DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"Earth.jpg"));
    (D3DXCreateTextureFromFileEx(pd3dDevice, str, D3DX_DEFAULT, D3DX_DEFAULT,
        D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
        D3DX_DEFAULT, D3DX_DEFAULT, 0,
        NULL, NULL, &g_pMeshTexture));*/
    //g_pEffect->SetTexture((D3DXHANDLE)"DiffuseMap_Tex", g_pMeshTexture);
    if (g_pEffect)
        cout << "EFFECT ON" << endl;
    if (g_pMesh)
        cout << "MESH ON" << endl;
   /* if (g_pMeshTexture)
        cout << "TEXTURE ON" << endl;*/
    return S_OK;
}
float     gRotationY = 0.0f;
//--------------------------------------------------------------------------------------
// Render the scene using the D3D9 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    HRESULT hr;

    // Clear the render target and the zbuffer 
    V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 45, 50, 170 ), 1.0f, 0 ) );

    // Render the scene
    if( SUCCEEDED( pd3dDevice->BeginScene() ) )
    {
        UINT numPasses = 0;
        D3DXMATRIXA16 matView;
        D3DXMATRIXA16 matProjection;
        D3DXVECTOR3 vEyePt(gWorldCameraPosition.x, gWorldCameraPosition.y,
            gWorldCameraPosition.z);
        D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
        D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
        D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
        D3DXMatrixPerspectiveFovLH(&matProjection, FOV, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);

        D3DXMATRIXA16 matWorld;
        D3DXMatrixRotationY(&matWorld, gRotationY);
        g_pEffect->SetMatrix((D3DXHANDLE)"gWorldMatrix", &matWorld);
        g_pEffect->SetMatrix((D3DXHANDLE)"gViewMatrix", &matView);
        g_pEffect->SetMatrix((D3DXHANDLE)"gProjectionMatrix", &matProjection);
        g_pEffect->SetVector((D3DXHANDLE)"gWorldLightPosition", &gWorldLightPosition);
        g_pEffect->SetVector((D3DXHANDLE)"gWorldCameraPosition", &gWorldCameraPosition);
        g_pEffect->Begin(&numPasses, NULL);
        {
            for (UINT i = 0; i < numPasses; i++)
            {
                g_pEffect->BeginPass(i);
                {
                    // 구체를 그린다.
                    g_pMesh->DrawSubset(0);
                }
                g_pEffect->EndPass();
            }
        }
        g_pEffect->End();
        V( pd3dDevice->EndScene() );
    }
}




//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9DestroyDevice( void* pUserContext )
{
    if (g_pEffect)
    {
        g_pEffect->Release();
        g_pEffect = nullptr;
    }
    if (g_pMesh)
    {
        g_pMesh->Release();
        g_pMesh = nullptr;
    }
    //if (g_pMeshTexture)
    //{
    //    g_pMeshTexture->Release();
    //    g_pMeshTexture = nullptr;
    //}
    exit(0);
}


//--------------------------------------------------------------------------------------
// Initialize everything and go into a render loop
//--------------------------------------------------------------------------------------
//INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
int main(void)
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    // Set the callback functions
    DXUTSetCallbackD3D9DeviceCreated( OnD3D9CreateDevice );
    DXUTSetCallbackD3D9FrameRender( OnD3D9FrameRender );
    DXUTSetCallbackD3D9DeviceDestroyed( OnD3D9DestroyDevice );

    // TODO: Perform any application-level initialization here

    // Initialize DXUT and create the desired Win32 window and Direct3D device for the application
    DXUTInit( true, true ); // Parse the command line and show msgboxes
    DXUTSetHotkeyHandling( true, true, true );  // handle the default hotkeys
    DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
    DXUTCreateWindow( L"ShaderDX" );
    DXUTCreateDevice( true, WIN_WIDTH, WIN_HEIGHT );

    // Start the render loop
    DXUTMainLoop();

    // TODO: Perform any application-level cleanup here

    return DXUTGetExitCode();
}


