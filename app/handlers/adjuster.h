// Copyright (c) 2018 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#ifndef ROBUST_PALM_ROI_APP_HANDLERS_ADJUSTER_H_
#define ROBUST_PALM_ROI_APP_HANDLERS_ADJUSTER_H_

#include <vector>
#include "handlers/handler.h"

namespace rpr {

class Adjuster : public Handler {
 public:
  virtual Status Handle(PalmInfoDTO& palm);
 protected:
  virtual Status Adjust(PalmInfoDTO& palm) = 0;
};

inline Status Adjuster::Handle(PalmInfoDTO& palm) {
  const cv::Mat& orig = palm.PrevHandleRes(); 
  if (orig.empty() || orig.channels() != 1) {
    return Status::LoadImageError("Original palm image must be binary.");
  }
  return Adjust(palm);
}


class NoiseAdjuster : public Adjuster {
 private:
  virtual Status Adjust(PalmInfoDTO& palm);
  void SmoothBoundary(const cv::Mat& src, cv::Mat* dst);
  int FindMaxContourAreaIndex(const std::vector< std::vector<cv::Point> >& contours,
                              const std::vector<cv::Vec4i>& hierarchy);
};


class AngleAdjuster : public Adjuster {
 private:
  virtual Status Adjust(PalmInfoDTO& palm);
  void ErodeFinger(const cv::Mat& src, cv::Mat* dst);
};

}   // namespace rpr

#endif  // ROBUST_PALM_ROI_APP_HANDLERS_ADJUSTER_H_