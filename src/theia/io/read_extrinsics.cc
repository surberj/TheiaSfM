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

#include "theia/sfm/reconstruction.h"

namespace theia {

bool ReadExtrinsics(const std::string& extrinsics_file,
                     Reconstruction* reconstruction) {
  std::ifstream ifs(extrinsics_file.c_str(), std::ios::in);
  if (!ifs.is_open()) {
    LOG(ERROR) << "Cannot read the list file from " << extrinsics_file;
    return false;
  }

  const int num_views = reconstruction->NumViews();

  // check if size of file matches reconstruction->NumViews()
  std::string num_views_in_extrinsics_file;
  ifs >> num_views_in_extrinsics_file;
  CHECK_EQ(num_views, std::stoi(num_views_in_extrinsics_file)) << "read_extrinsics: number of views "
                          "between reconstruction and extrinsics file do not match.";

  // Read camera extrinsics
  int i = 0;
  for (const ViewId view_id : reconstruction->ViewIds()) {
    // read in file and break if end of file is reached.
    std::string filename;
    ifs >> filename;
    if (!ifs.eof()) {
      break;
    }
    // excess view and camera of reconstruction
    //const View& view = *reconstruction->View(view_id);
    //static int kCameraParametersSize = 13;
    //Camera* camera = view.MutableCamera();

    // set estimated (since the given extrinsics are a prior estimate)
    View* view = reconstruction->MutableView(view_id);
    Camera* camera = view->MutableCamera();
    view->SetEstimated(true);
    // read in position and rotation of camera i
    Eigen::Vector3d position;
    Eigen::Matrix3d rotation;
    std::string value;
    ifs >> value;
    position(0) = std::stod(value);
    ifs >> value;
    position(1) = std::stod(value);
    ifs >> value;
    position(2) = std::stod(value);
    ifs >> value;
    rotation(0,0) = std::stod(value);
    ifs >> value;
    rotation(0,1) = std::stod(value);
    ifs >> value;
    rotation(0,2) = std::stod(value);
    ifs >> value;
    rotation(1,0) = std::stod(value);
    ifs >> value;
    rotation(1,1) = std::stod(value);
    ifs >> value;
    rotation(1,2) = std::stod(value);
    ifs >> value;
    rotation(2,0) = std::stod(value);
    ifs >> value;
    rotation(2,1) = std::stod(value);
    ifs >> value;
    rotation(2,2) = std::stod(value);

    camera->SetPosition(position);
    camera->SetOrientationFromRotationMatrix(rotation);

    ++i;
  }
  return true;
}

}  // namespace theia
