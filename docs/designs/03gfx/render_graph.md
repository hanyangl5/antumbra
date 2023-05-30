draft design 

render graph is resposible to handle gpu resource dependency, aliasing, and lifetime management.
shader/pipeline hot reload 
need an scripting language to describe the render pipeline

the scripting api:

1. create graph

```python
rg = InitializeRenderGraph()

RenderGraphDesc desc1

graph1 = rg.create_render_graph(&desc1)

# you can create many graph at the same time, availble to runtime switch 

GraphDesc desc2
graph2 rg.CreateRenderGraph(&desc2)

...

if game_play:
    rg.SetActiveGraph(graph1)
 else if cut_scene:
    rg.SetActiveGraph(graph2)
```

2. dependency

```python
gfx_shader = CreateShader("\
RWStructureBuffer<float> buffer;\
RWTexture<float> tex; \
vs\
...\
ps\
buffer = ...\
tex = ...\
")

comp_shader = CreateShader("\
RWStructureBuffer buffer;\
Texture2D<float> tex;\
Texture2D<float4> color\
vs\
...\
ps\
modify buffer\
sample tex\
sample color\
")

pass1 :{
    pass_type: graphics
    shader: gfx_shader
    pipeline_desc :{
        vertex_input_attributes: {
            vertex: stride, format, ...
            normal:
            uv:
        }
        input_assembly_state: {
            primitive_topology = triangle_list
        }
        raster_state :{
            ...
        }
        blend_state: {
            ...
        }
        multi_sample_state:{
            ...
        }
        viewport_state: {
            ...
        }
        render_targets:{
        rt0 = RGBA8, # don't need to specify rw state manualy
        rt1 = D24_S8
        }
    }
}

pass2 = function {
    pass_type : compute,
    shader : compute_shader #auto generate pipeline layouts
    #compute pipeline is stateless
}

...
rg_buffer buffer = rg.create_buffer(size, update_freq = per_frame)


graph.add_pass(pass1)
graph.add_pass(pass2)

graph.execute {
pass1.set_buffer(buffer,"buffer") # set buffer will update a descriptor set with the buffer
pass1.set_texture(texture, "tex")
gfx_cmd = graph.request_command(gfx)
gfx_cmd.draw();
graph.submit(gfx_cmd)
# add barrier of buffer, texture, pass1.rt0


pass2.set_buffer(buffer, "buffer")
pass2.set_texture(buffer, "buffer")
pass2.set_texture(pass1.rt0, "color")
# print warning if required resource is not bound
comp_cmd = graph.request_command(gfx)
comp_cmd.dispatch(111);
comp_cmd.submit(gfx_cmd)
}

```

3. resource aliasing

4. resource lifetime 

5. hotload
6. 