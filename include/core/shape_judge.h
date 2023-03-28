#ifndef ROTATE_RC_SHAPE_JUDGE_H_
#define ROTATE_RC_SHAPE_JUDGE_H_

#include <omp.h>

#include "basic/basic.h"
#include "basic/file.h"
#include "basic/io.h"

namespace ShapeJudge {

struct EllipseCylinder {
  std::unique_ptr<Eigen::Vector3d> center_;
  std::unique_ptr<Eigen::Vector3d> semi_major_axe_;
  std::unique_ptr<Eigen::Vector3d> semi_minor_axe_;
  std::unique_ptr<Eigen::Vector3d> height_;
};

struct XZYEllipseCylinder {
  std::unique_ptr<Eigen::Vector3d> center_;
  double semi_major_axe_;
  double semi_minor_axe_;
  double height_;
};

void solve(const XZYEllipseCylinder& xzy_ellipse_cylinder, const std::filesystem::path& x_ii_file,
           const std::filesystem::path& mask_output_file, ReadMode read_mode);
void calAABB(const XZYEllipseCylinder& xzy_ellipse_cylinder, Eigen::Vector<double, 6>& box);
bool intersectBox(const Eigen::Vector3d& point, const Eigen::Vector<double, 6>& box);
bool intersectXZYEllipseCylinder(const Eigen::Vector3d& point, const XZYEllipseCylinder& xzy_ellipse_cylinder);

}  // namespace ShapeJudge

#endif
