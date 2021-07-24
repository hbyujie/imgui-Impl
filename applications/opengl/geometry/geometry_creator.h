#pragma once

#include <memory>

class GLMesh;

class GeometryCreator
{
  public:
	  static void GetCube(const std::shared_ptr<GLMesh> mesh);
};
