# UselessEngine
 没用的3d引擎（基于Vulkan，非专业C++选手）
 施工中。

Basic
-----

- [x] Simple ECS
- [x] Vulkan API
    - [x] global (instance & device & surface & swapchian)
    - [x] buffer
    - [x] cmd
    - [x] image
    - [x] pass
    - [x] pipeine
- [x] ImGui
    - [x] docking
    - [x] viewport

Engine
----

- [x] Editor ( ... )
- [x] Logic ( ... )
- [x] Render
    - [x] Light
	    - [x] direction light
	    - [x] point light
	    - [x] spot light
    - [x] Material
    - [x] Mesh
    - [x] PostProcess
    - [x] RenderPass
	    - [x] imgui
	    - [x] forward
	    - [x] deferred
	    - [x] shadow
	    - [x] post_process

Shader (VkPipeline)
-----

- [x] skybox
- [x] shadow
- [x] light_model
- [x] deferred_light_model
- [x] pbr
- [ ] ~~deferred_pbr~~
- [x] color
- [x] water
- [x] post_process::toon_mapping
- [x] post_process::bloom
- [x] post_process::SSAO
- [x] post_process::global

Preview (See the screenshots folder for details)
-----
![post-process](https://github.com/z11027747/UselessEngine/assets/24534845/fa20f7aa-3b61-44e2-afbc-2272eab18dde)
![deferred](https://github.com/z11027747/UselessEngine/assets/24534845/efc70378-d402-407d-9a55-cc2eb860977d)
![pbr_simple](https://github.com/z11027747/UselessEngine/assets/24534845/3c691016-9194-4b95-a7ca-8ad5273192fa)



