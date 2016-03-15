// Copyright (C) 2015 The Regents of the University of California (Regents).
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//
//     * Neither the name of The Regents or University of California nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Please contact the author of this library if you have any questions.
// Author: Chris Sweeney (cmsweeney@cs.ucsb.edu)

#include "theia/sfm/set_camera_extrinsics_from_priors.h"

#include <glog/logging.h>

#include "theia/sfm/camera_extrinsics_prior.h"
#include "theia/sfm/camera/camera.h"
#include "theia/sfm/reconstruction.h"
#include "theia/sfm/types.h"
#include "theia/sfm/view.h"

namespace theia {

void SetViewCameraExtrinsicsFromPriors(View* view) {
  Camera* camera = view->MutableCamera();
  const CameraExtrinsicsPrior prior = view->CameraExtrinsicsPrior();

  // Set the camera position
  if (prior.position[0].is_set && prior.position[1].is_set && prior.position[2].is_set) {
    Eigen::Vector3d pos;
    pos(0) = prior.position[0].value;
    pos(1) = prior.position[1].value;
    pos(2) = prior.position[2].value;
    camera->SetPosition(pos);
  }

  // Set the camera position
  if (prior.rotation[0].is_set && prior.position[8].is_set) {
    Eigen::Matrix3d rot;
    rot(0,0) = prior.rotation[0].value;
    rot(0,1) = prior.rotation[1].value;
    rot(0,2) = prior.rotation[2].value;
    rot(1,0) = prior.rotation[3].value;
    rot(1,1) = prior.rotation[4].value;
    rot(1,2) = prior.rotation[5].value;
    rot(2,0) = prior.rotation[6].value;
    rot(2,1) = prior.rotation[7].value;
    rot(2,2) = prior.rotation[8].value;
    camera->SetOrientationFromRotationMatrix(rot);
  }
}

// Sets the camera extrinsics (poses) from the CameraExtrinsicsPrior of each view.
void SetCameraExtrinsicsFromPriors(Reconstruction* reconstruction) {
  const auto& view_ids = reconstruction->ViewIds();
  for (const ViewId view_id : view_ids) {
    View* view = CHECK_NOTNULL(reconstruction->MutableView(view_id));
    SetViewCameraExtrinsicsFromPriors(view);
  }
}

}  // namespace theia
