#include "catch.hpp"

#include <gl/buffer.hpp>
#include <gl/texture.hpp>

#include <fg/framegraph.hpp>
#include <fg/render_task_builder.hpp>
#include <fg/render_task_resources.hpp>
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

using buffer_resource     = fg::resource<glr::buffer_description , gl::buffer    >;
using texture_1d_resource = fg::resource<glr::texture_description, gl::texture_1d>;
using texture_2d_resource = fg::resource<glr::texture_description, gl::texture_2d>;
using texture_3d_resource = fg::resource<glr::texture_description, gl::texture_3d>;
}

TEST_CASE("Framegraph test.", "[framegraph]")
{
  fg::framegraph<glr::buffer_resource, glr::texture_1d_resource, glr::texture_2d_resource, glr::texture_3d_resource> framegraph;

  struct render_task_data
  {
    glr::buffer_resource     vertex_buffer  ;
    glr::buffer_resource     normal_buffer  ;
    glr::buffer_resource     texcoord_buffer;
    glr::buffer_resource     index_buffer   ;
    glr::texture_2d_resource diffuse_texture;
  };
  
  auto render_task = framegraph.add_render_task<render_task_data>(
  [&] (      render_task_data& data,       fg::render_task_builder&   builder  )
  {
    
  },
  [=] (const render_task_data& data, const fg::render_task_resources& resources)
  {
    
  });

  auto& output = render_task->data();

  REQUIRE(output.vertex_buffer.id() == 0);
}