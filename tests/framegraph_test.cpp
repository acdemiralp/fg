#include "catch.hpp"

#include <array>
#include <cstddef>

#include <fg/framegraph.hpp>
#include <fg/registry.hpp>
#include <fg/resource.hpp>

namespace gl
{
  using buffer       = int;
  using texture      = int;
  using program      = int;
  using vertex_array = int;
}

struct buffer_resource : fg::resource<gl::buffer>
{
  std::size_t size;

  void realize() override
  {
    realization_.emplace(size);
  }
};

struct texture_resource : fg::resource<gl::texture>
{
  std::array<std::size_t, 3> size;

};

TEST_CASE("Framegraph test.", "[framegraph]")
{
  fg::registry<gl::buffer, gl::texture, gl::program, gl::vertex_array> registry;

  auto& buffers = registry.get<buffer_resource>();
  buffers.emplace_back();
  for (auto& buffer : buffers)
    buffer.realize();

  fg::framegraph framegraph;
}