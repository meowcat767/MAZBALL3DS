#include <3ds.h>
#include <citro3d.h>

int main()
{
    gfxInitDefault();
    
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    
    C3D_RenderTarget* target = 
        C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
    
    C3D_RenderTargetSetOutput(target, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);
    
    while (aptMainLoop())
    {
        hidScanInput();
        
        const u32 kDown = hidKeysDown();
        
        if (kDown & KEY_START)
            break;
        
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C3D_FrameDrawOn(target);
        
        C3D_FVUnifMtx4x4(
            C3D_GetMatrixStack(),
            0,
            (C3D_Mtx*)nullptr
            );
        
        C3D_FrameEnd(0);
    }
    
    C3D_RenderSystemDelete(target);
    C3D_Fini();
    gfxExit();
    
    return 0;
}