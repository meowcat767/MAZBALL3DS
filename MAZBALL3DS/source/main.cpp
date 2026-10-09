
#include <3ds.h>
#include <citro3d.h>
#include <cstring>

#include "triangle_shbin.h"

struct Vertex
{
    float x, y, z;
};

static const Vertex vertices[] =
{
    {  0.0f,  0.65f, 0.5f },
    { -0.65f, -0.5f, 0.5f },
    {  0.65f, -0.5f, 0.5f }
};

static DVLB_s* vertexShader = nullptr;
static shaderProgram_s program;
static void* vertexBuffer = nullptr;

static bool sceneInit()
{
    // Load the compiled vertex shader.
    vertexShader = DVLB_ParseFile(
        (u32*)triangle_shbin,
        triangle_shbin_size
    );

    if (!vertexShader)
        return false;

    shaderProgramInit(&program);
    shaderProgramSetVsh(&program, &vertexShader->DVLE[0]);
    C3D_BindProgram(&program);

    // Attribute 0 contains each vertex position.
    C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
    AttrInfo_Init(attrInfo);
    AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3);

    // Attribute 1 supplies a constant white vertex colour.
    AttrInfo_AddFixed(attrInfo, 1);
    C3D_FixedAttribSet(1, 1.0f, 1.0f, 1.0f, 1.0f);

    // Make the fragment stage display the vertex colour.
    C3D_TexEnv* env = C3D_GetTexEnv(0);
    C3D_TexEnvInit(env);
    C3D_TexEnvSrc(
    env,
    C3D_Both,
    GPU_PRIMARY_COLOR,
    GPU_PRIMARY_COLOR,
    GPU_PRIMARY_COLOR
);
    C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);

    // Copy vertices into memory suitable for the GPU.
    vertexBuffer = linearAlloc(sizeof(vertices));

    if (!vertexBuffer)
    {
        shaderProgramFree(&program);
        DVLB_Free(vertexShader);
        vertexShader = nullptr;
        return false;
    }

    std::memcpy(vertexBuffer, vertices, sizeof(vertices));

    C3D_BufInfo* bufInfo = C3D_GetBufInfo();
    BufInfo_Init(bufInfo);
    BufInfo_Add(bufInfo, vertexBuffer, sizeof(Vertex), 1, 0x0);

    return true;
}

static void sceneExit()
{
    if (vertexBuffer)
        linearFree(vertexBuffer);

    if (vertexShader)
    {
        shaderProgramFree(&program);
        DVLB_Free(vertexShader);
    }
}

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

    if (!target)
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

    if (!sceneInit())
    {
        C3D_RenderTargetDelete(target);
        C3D_Fini();
        gfxExit();
        return 1;
    }

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

        C3D_FrameDrawOn(target);

        C3D_DrawArrays(GPU_TRIANGLES, 0, 3);

        C3D_FrameEnd(0);
    }

    sceneExit();
    C3D_RenderTargetDelete(target);
    C3D_Fini();
    gfxExit();

    return 0;
}
