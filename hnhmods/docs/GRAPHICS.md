# Part IV: Rendering & Pipeline Graphics**

## Renderer Abstraction

EXAMPLE: Render-graph design, command queue submissions, state-cache tracking, and shader compilation/reflection pipeline.
NOTES:
- Using `BeginScene`, `EndScene` and `PresentFrame`.

## Asset & Geometry Pipeline

EXAMPLE: Mesh layout specs, vertex format packing, bone animation systems (skeletal hierarchy, matrix blending), and material handling.
NOTES:
- see CHanaMesh, CHanaVert, CHanaFrame, CHanaMate, CHanaAnim.

## Lighting, Post-Processing & Pipelines

EXAMPLE: Deferred vs. Forward rendering pathways, shadow mapping mechanics, and post-processing buffers.
NOTES:
- see `SetRenderState`, `SetTextureStageState`.