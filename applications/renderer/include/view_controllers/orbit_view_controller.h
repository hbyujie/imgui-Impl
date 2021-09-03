///////////////////////////////////////////////////////////////////////////////
// orbit_view_controller.h
// ===============
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "view_controllers/view_controller.h"

class Camera;

class OrbitViewController : public ViewController
{
  public:
    OrbitViewController();
    ~OrbitViewController() override;

    void Zero() override;

    void RotateCamera(const float x, const float y) override;

    void MoveCamera(const float x, const float y) override;

    void ZoomCamera(const float delta) override;
};
