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

#include "theia/io/read_extrinsics.h"

#include <Eigen/Core>
#include <glog/logging.h>

#include <cstdio>
#include <cstdlib>
#include <fstream>  // NOLINT
#include <iostream>  // NOLINT
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>

#include "theia/sfm/camera_intrinsics_prior.h"

namespace theia {

bool ReadExtrinsics(const std::string& extrinsics_file,
                     std::unordered_map<std::string, CameraExtrinsicsPrior>*
                         camera_extrinsics_prior) {

  // open extrinsics file
  std::ifstream ifs(extrinsics_file.c_str(), std::ios::in);
  if (!ifs.is_open()) {
    LOG(ERROR) << "Cannot read the list file from " << extrinsics_file;
    return false;
  }

  // get number of cameras from first line:
  std::string value;
  ifs >> value;
  const int num_views_in_extrinsics_file = std::stoi(value);
  LOG(INFO)
          << "read_extrinsics: num_views_in_extrinsics_file = " << num_views_in_extrinsics_file;

  // Read camera extrinsics
  for (int i=0; i<num_views_in_extrinsics_file; i++) {

    // read in camera i called filename
    std::string filename;
    ifs >> filename;
    if (filename.length() == 0) {
      break;
    }

    // read in position and rotation of camera i
    CameraExtrinsicsPrior temp_camera_extrinsics_prior;

    temp_camera_extrinsics_prior.position[0].is_set = true;
    ifs >> temp_camera_extrinsics_prior.position[0].value;
    temp_camera_extrinsics_prior.position[1].is_set = true;
    ifs >> temp_camera_extrinsics_prior.position[1].value;
    temp_camera_extrinsics_prior.position[2].is_set = true;
    ifs >> temp_camera_extrinsics_prior.position[2].value;

    temp_camera_extrinsics_prior.rotation[0].is_set = true;
    ifs >> temp_camera_extrinsics_prior.rotation[0].value;
    temp_camera_extrinsics_prior.rotation[1].is_set = true;
    ifs >> temp_camera_extrinsics_prior.rotation[1].value;
    temp_camera_extrinsics_prior.rotation[2].is_set = true;
    ifs >> temp_camera_extrinsics_prior.rotation[2].value;
    temp_camera_extrinsics_prior.rotation[3].is_set = true;
    ifs >> temp_camera_extrinsics_prior.rotation[3].value;
    temp_camera_extrinsics_prior.rotation[4].is_set = true;
    ifs >> temp_camera_extrinsics_prior.rotation[4].value;
    temp_camera_extrinsics_prior.rotation[5].is_set = true;
    ifs >> temp_camera_extrinsics_prior.rotation[5].value;
    temp_camera_extrinsics_prior.rotation[6].is_set = true;
    ifs >> temp_camera_extrinsics_prior.rotation[6].value;
    temp_camera_extrinsics_prior.rotation[7].is_set = true;
    ifs >> temp_camera_extrinsics_prior.rotation[7].value;
    temp_camera_extrinsics_prior.rotation[8].is_set = true;
    ifs >> temp_camera_extrinsics_prior.rotation[8].value;

    // write extrinsics to Prior
    (*camera_extrinsics_prior)[filename] = temp_camera_extrinsics_prior;

// write extrinsics directly to the reconstruction views:
//    // excess view and camera of reconstruction
//    View* view = reconstruction->MutableView(view_id);
//    Camera* camera = view->MutableCamera();
//    // set camera extrinsics
//    //view->SetEstimated(true);
//    camera->SetPosition(position);
//    camera->SetOrientationFromRotationMatrix(rotation);
  }
  return true;
}

}  // namespace theia
