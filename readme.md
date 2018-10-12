**What is a framegraph?** 

A rendering abstraction which describes a frame as a directed acyclic graph of render tasks and resources.
Based on the [Game Developers Conference (GDC) presentation by Yuriy O’Donnell](https://www.gdcvault.com/play/1024612/FrameGraph-Extensible-Rendering-Architecture-in).

**What is a render task?** 

A compute or graphics task to be performed as part of a rendering pipeline.

**What is a resource?** 

Data created, read or written by a render task. Alternates between two states; virtual and real.
While virtual, the resource is not instantiated but contains the necessary information to do so. 
While real, the resource is instantiated and ready for use.
A **transient** resource is owned, realized and virtualized by the framegraph.
A **retained** resource is always real and is imported into the framegraph.

**Usage**

First, create descriptions for your rendering resources (e.g. buffers, textures) and declare them as framegraph resources.
```cxx
struct buffer_description
{
  std::size_t size;
};
struct texture_description
{
  std::size_t                levels;
  GLenum                     format;
  std::array<std::size_t, 3> size  ;
};

using buffer_resource     = fg::resource<buffer_description , gl::buffer    >;
using texture_1d_resource = fg::resource<texture_description, gl::texture_1d>;
using texture_2d_resource = fg::resource<texture_description, gl::texture_2d>;
using texture_3d_resource = fg::resource<texture_description, gl::texture_3d>;
```

Then, specialize `fg::realize<description_type, actual_type>` for each declared resource. This function takes in a resource description and returns an actual resource.
```cxx
namespace fg
{
template<>
std::unique_ptr<gl::buffer>     realize(const buffer_description&  description)
{
  auto actual = std::make_unique<gl::buffer>(); 
  actual->set_size(static_cast<GLsizeiptr>(description.size));
  return actual;
}
template<>
std::unique_ptr<gl::texture_2d> realize(const texture_description& description)
{
  auto actual = std::make_unique<gl::texture_2d>();
  actual->set_storage(
    description.levels , 
    description.format , 
    description.size[0], 
    description.size[1]);
  return actual;
}
}
```

You are now ready to create a framegraph and add your render tasks / retained resources to it.
```cxx
fg::framegraph framegraph;

gl::texture_2d backbuffer;
auto retained_resource = framegraph.add_retained_resource(
  "Backbuffer", 
  texture_description(), 
  &backbuffer);

struct render_task_data
{
  texture_2d_resource* input1;
  texture_2d_resource* input2;
  texture_2d_resource* input3;
  texture_2d_resource* output;
};
auto render_task = framegraph.add_render_task<render_task_data>(
  "Render Task",
  [&] (render_task_data& data, fg::render_task_builder& builder)
  {
    data.input1 = builder.create<texture_2d_resource>("Texture 1", texture_description());
    data.input2 = builder.create<texture_2d_resource>("Texture 2", texture_description());
    data.input3 = builder.create<texture_2d_resource>("Texture 3", texture_description());
    data.output = builder.write <texture_2d_resource>(retained_resource);
  },
  [=] (const render_task_data& data)
  {
    auto actual1 = data.input1->actual();
    auto actual2 = data.input2->actual();
    auto actual3 = data.input3->actual();
    auto actual4 = data.output->actual();
    // Perform actual rendering. You may load resources from CPU by capturing them.
  });

auto& data = render_task->data();
```

Once all render tasks and resources are added, call `framegraph.compile()`. Then, `framegraph.execute()` in each update. It is also possible to export to GraphViz for debugging / visualization via `framegraph.export_graphviz(filename)`:

![alt text](https://github.com/acdemiralp/fg/blob/develop/docs/images/example.png "Example 1")

![alt text](https://github.com/acdemiralp/fg/blob/develop/docs/images/example2.png "Example 2")

**Next Steps** 
- Asynchronous render tasks (+ resource / aliasing barriers).
