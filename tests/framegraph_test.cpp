#include "catch.hpp"

#include <gl/buffer.hpp>
#include <gl/texture.hpp>

#include <fg/framegraph.hpp>
#include <fg/resource.hpp>

namespace glr
{
struct buffer_description
{
  std::size_t size;
};
std::unique_ptr<gl::buffer>          actualize(const buffer_description&  description)
{
  auto actual = std::make_unique<gl::buffer>(); 
  actual->set_data_immutable(static_cast<GLsizeiptr>(description.size));
  return actual;
}

struct texture_description
{
  std::size_t                levels;
  GLenum                     format;
  std::array<std::size_t, 3> size  ;
};
template<GLenum target>
std::unique_ptr<gl::texture<target>> actualize(const texture_description& description)
{
  auto actual = std::make_unique<gl::texture<target>>();
  if      (target == GL_TEXTURE_1D) actual->set_storage(static_cast<GLsizei>(description.levels), description.format, static_cast<GLsizei>(description.size[0]));
  else if (target == GL_TEXTURE_2D) actual->set_storage(static_cast<GLsizei>(description.levels), description.format, static_cast<GLsizei>(description.size[0]), static_cast<GLsizei>(description.size[1]));
  else if (target == GL_TEXTURE_3D) actual->set_storage(static_cast<GLsizei>(description.levels), description.format, static_cast<GLsizei>(description.size[0]), static_cast<GLsizei>(description.size[1]), static_cast<GLsizei>(description.size[2]));
  return actual;
}

using buffer_resource     = fg::resource<buffer_description , gl::buffer    >;
using texture_1d_resource = fg::resource<texture_description, gl::texture_1d>;
using texture_2d_resource = fg::resource<texture_description, gl::texture_2d>;
using texture_3d_resource = fg::resource<texture_description, gl::texture_3d>;
}

TEST_CASE("Framegraph test.", "[framegraph]")
{
  fg::framegraph framegraph;

  // First render task declaration.
  struct render_task_1_data
  {
    glr::texture_2d_resource* output;
  };

  auto render_task_1 = framegraph.add_render_task<render_task_1_data>(
  [&] (      render_task_1_data& data,       fg::render_task_builder&   builder  )
  {
    data.output = builder.create<glr::texture_2d_resource>(glr::texture_description());
  },
  [=] (const render_task_1_data& data, const fg::render_task_resources& resources)
  {
    // Perform actual rendering. You may load resources from CPU by capturing them.
  });

  auto& data_1 = render_task_1->data();
  REQUIRE(data_1.output->id() == 0);
  
  // Second render pass declaration.
  struct render_task_2_data
  {
    glr::texture_2d_resource* input ;
    glr::texture_2d_resource* output;
  };
  
  auto render_task_2 = framegraph.add_render_task<render_task_2_data>(
  [&] (      render_task_2_data& data,       fg::render_task_builder&   builder  )
  {
    data.input  = builder.read                            (data_1.output);
    data.output = builder.create<glr::texture_2d_resource>(glr::texture_description());
  },
  [=] (const render_task_2_data& data, const fg::render_task_resources& resources)
  {
    // Perform actual rendering. You may load resources from CPU by capturing them.
  });

  auto& data_2 = render_task_2->data();
  REQUIRE(data_2.output->id() == 1);

  framegraph.traverse();
  framegraph.clear   ();
}