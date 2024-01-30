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
        inline static std::string Main{"main"};
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