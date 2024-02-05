# UselessEngine
 没用的3d引擎
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

- [ ] Editor
    - [x] wrap
    - [x] json
    - [ ] etc.
- [ ] Logic
    - [x] Transform
	    - [x] model matrix
    - [x] Camera
	    - [x] view&projection matrix
	    - [x] transform space
    - [ ] Hit
	    - [x] ray check (only sphere)
	    - [ ] etc.
    - [ ] Move
	    - [x] move follow
	    - [ ] move forward
	    - [ ] etc.
    - [ ] Rotate
	    - [x] rotate around
	    - [ ] etc.
    - [x] Scene
	    - [x] save to json
	    - [x] change
- [ ] Render
    - [ ] Light
	    - [x] direction light
	    - [x] point light
	    - [ ] spot light
    - [x] Material
    - [x] Mesh
    - [x] PostProcess
    - [ ] RenderPass
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
- [ ] pbr
- [ ] deferred_pbr
- [x] color
- [x] post_process::toon_mapping
- [x] post_process::bloom
- [ ] post_process::SSAO
- [x] post_process::global

Preview
-----
![image](https://github.com/z11027747/UselessEngine/assets/24534845/eb98c6b4-746b-4091-84ac-b28069847b8f)


