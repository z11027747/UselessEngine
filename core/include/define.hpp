#pragma once

#include <string>

class Define final
{
public:
    class EOName final
    {
    public:
        inline static std::string MainCamera{"MainCamera"};
        inline static std::string DirectionLight{"DirectionLight"};
        inline static std::string Skybox{"Skybox"};
        inline static std::string Axis{"Axis"};
        inline static std::string AxisX{"X"};
        inline static std::string AxisY{"Y"};
        inline static std::string AxisZ{"Z"};
    };

    class Pass final
    {
    public:
        inline static std::string ImGui{"imGui"};
        inline static std::string Forward{"forward"};
        inline static std::string Deferred{"deferred"};
        inline static std::string Shadow{"shadow"};
        inline static std::string PostProcess{"post_process"};
    };

    class Pipeline final
    {
    public:
        inline static std::string Skybox{"skybox"};
        inline static std::string Shadow{"shadow"};
        inline static std::string LightModel{"light_model"};
        inline static std::string Color{"color"};
        inline static std::string Dissolve{"dissolve"};
        inline static std::string Water{"water"};

        inline static std::string PBR_Simplest{"pbr_simplest"};

        inline static std::string Deferred_LightModel{"deferred_light_model"};
        inline static std::string Deferred_Shading{"deferred_shading"};
        inline static std::string Deferred_Volumn{"deferred_volumn"};
        inline static std::string Deferred_PointLight{"deferred_point_light"};

        // post process
        inline static std::string PostProcess_SSAO{"SSAO"};
        inline static std::string PostProcess_ToonMapping{"toon_mapping"};
        inline static std::string PostProcess_GaussBlur{"gauss_blur"};
        inline static std::string PostProcess_Bloom{"bloom"};
        inline static std::string PostProcess_Global{"global"};
    };

    class RenderQueue final
    {
    public:
        inline static int Skybox{1000};
        inline static int Geometry{2000};
        inline static int Transparent{3000};
    };

    class Res final
    {
    public:
        inline static std::string Img_White{"resource/texture/white.png"};
        inline static std::string Img_Bump{"resource/texture/bump.png"};
    };
};