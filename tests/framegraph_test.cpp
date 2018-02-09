#include "catch.hpp"

#include <array>
#include <cstddef>

#include <fg/framegraph.hpp>

namespace gl
{
  using buffer     = std::size_t;
  using texture_1d = std::size_t;
  using texture_2d = std::size_t;
  using texture_3d = std::size_t;
}

namespace glr
{
struct buffer_description
{
  std::size_t size;
};
struct texture_description
{
  std::size_t                levels;
  std::size_t                format;
  std::array<std::size_t, 3> size  ;
};

using buffer_resource     = fg::resource<buffer_description , gl::buffer    >;
using texture_1d_resource = fg::resource<texture_description, gl::texture_1d>;
using texture_2d_resource = fg::resource<texture_description, gl::texture_2d>;
using texture_3d_resource = fg::resource<texture_description, gl::texture_3d>;
}

namespace fg
{
template<>
std::unique_ptr<gl::buffer>     realize(const glr::buffer_description&  description)
{
  return std::make_unique<gl::buffer>(description.size);
}
template<>
std::unique_ptr<gl::texture_2d> realize(const glr::texture_description& description)
{
  return std::make_unique<gl::buffer>(description.levels);
}
}

TEST_CASE("Framegraph test.", "[framegraph]")
{
  fg::framegraph framegraph;

  auto retained_resource = framegraph.add_retained_resource("Retained Resource 1", glr::texture_description(), static_cast<gl::texture_2d*>(nullptr));

  // First render task declaration.
  struct render_task_1_data
  {
    glr::texture_2d_resource* output1;
    glr::texture_2d_resource* output2;
    glr::texture_2d_resource* output3;
    glr::texture_2d_resource* output4;
  };
  auto render_task_1 = framegraph.add_render_task<render_task_1_data>(
    "Render Task 1",
    [&] (render_task_1_data& data, fg::render_task_builder& builder)
    {
      data.output1 = builder.create<glr::texture_2d_resource>("Resource 1", glr::texture_description());
      data.output2 = builder.create<glr::texture_2d_resource>("Resource 2", glr::texture_description());
      data.output3 = builder.create<glr::texture_2d_resource>("Resource 3", glr::texture_description());
      data.output4 = builder.write <glr::texture_2d_resource>(retained_resource);
    },
    [=] (const render_task_1_data& data)
    {
      // Perform actual rendering. You may load resources from CPU by capturing them.
      auto actual1 = data.output1->actual();
      auto actual2 = data.output2->actual();
      auto actual3 = data.output3->actual();
      auto actual4 = data.output4->actual();
    });

  auto& data_1 = render_task_1->data();
  REQUIRE(data_1.output1->id() == 1);
  REQUIRE(data_1.output2->id() == 2);
  REQUIRE(data_1.output3->id() == 3);
  
  // Second render pass declaration.
  struct render_task_2_data
  {
    glr::texture_2d_resource* input1;
    glr::texture_2d_resource* input2;
    glr::texture_2d_resource* output1;
    glr::texture_2d_resource* output2;
  };
  auto render_task_2 = framegraph.add_render_task<render_task_2_data>(
    "Render Task 2",
    [&] (render_task_2_data& data, fg::render_task_builder& builder)
    {
      data.input1  = builder.read                            (data_1.output1);
      data.input2  = builder.read                            (data_1.output2);
      data.output1 = builder.write                           (data_1.output3);
      data.output2 = builder.create<glr::texture_2d_resource>("Resource 4", glr::texture_description());
    },
    [=] (const render_task_2_data& data)
    {
      // Perform actual rendering. You may load resources from CPU by capturing them.
      auto actual1 = data.input1 ->actual();
      auto actual2 = data.input2 ->actual();
      auto actual3 = data.output1->actual();
      auto actual4 = data.output2->actual();
    });

  auto& data_2 = render_task_2->data();
  REQUIRE(data_2.output2->id() == 4);

  struct render_task_3_data
  {
    glr::texture_2d_resource* input1;
    glr::texture_2d_resource* input2;
    glr::texture_2d_resource* output;
  };
  auto render_task_3 = framegraph.add_render_task<render_task_3_data>(
    "Render Task 3",
    [&] (render_task_3_data& data, fg::render_task_builder& builder)
    {
      data.input1 = builder.read (data_2.output1);
      data.input2 = builder.read (data_2.output2);
      data.output = builder.write(retained_resource);
    },
    [=] (const render_task_3_data& data)
    {
      // Perform actual rendering. You may load resources from CPU by capturing them.
      auto actual1 = data.input1->actual();
      auto actual2 = data.input2->actual();
      auto actual3 = data.output->actual();
    });
  
  framegraph.compile        ();
  for(auto i = 0; i < 100; i++)
    framegraph.execute      ();
  framegraph.export_graphviz("framegraph.gv");
  framegraph.clear          ();
}