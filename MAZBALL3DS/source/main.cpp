
#include <3ds.h>
#include <citro3d.h>

int main()
{
    gfxInitDefault();

    if (!C3D_Init(C3D_DEFAULT_CMDBUF_SIZE))
    {
        gfxExit();
        return 1;
    }

    C3D_RenderTarget* target = C3D_RenderTargetCreate(
        240,
        400,
        GPU_RB_RGBA8,
        GPU_RB_DEPTH24_STENCIL8
    );

    if (target == nullptr)
    {
        C3D_Fini();
        gfxExit();
        return 1;
    }

    C3D_RenderTargetSetOutput(
        target,
        GFX_TOP,
        GFX_LEFT,
        GX_TRANSFER_FLIP_VERT(0) |
        GX_TRANSFER_OUT_TILED(0) |
        GX_TRANSFER_RAW_COPY(0) |
        GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) |
        GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8)
    );

    while (aptMainLoop())
    {
        hidScanInput();

        if (hidKeysDown() & KEY_START)
            break;

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

        C3D_RenderTargetClear(
            target,
            C3D_CLEAR_ALL,
            0x182848FF,
            0
        );

        C3D_FrameEnd(0);
    }

    C3D_RenderTargetDelete(target);
    C3D_Fini();
    gfxExit();

    return 0;
}
