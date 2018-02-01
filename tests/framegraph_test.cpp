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
}

TEST_CASE("Framegraph test.", "[framegraph]")
{
  fg::framegraph<
    fg::resource<glr::buffer_description , gl::buffer    >,
    fg::resource<glr::texture_description, gl::texture_1d>,
    fg::resource<glr::texture_description, gl::texture_2d>,
    fg::resource<glr::texture_description, gl::texture_3d>> framegraph;
}