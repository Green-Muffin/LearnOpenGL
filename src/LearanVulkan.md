
Instance
Validation layers
Physical devices and queue families
Logical device and queues
Window surface
Swap chain
Image views
Shader modules
Fixed functions
Render passes
Framebuffers
Command buffers
Rendering and presentation


RenderSystem::initialize
	initialize rhi
	load global rendering resource
	upload global rendering resource
	setup render camera
	setup render scene
	initialize render pipeline and pass

RenderSystem::tick
	process swap data between logic and render contexts
	prepare render command context
	update per-frame buffer
	update per-frame visible objects
	prepare pipeline's visible objcts
	render one frame
