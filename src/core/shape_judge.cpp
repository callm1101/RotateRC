#include "core/shape_judge.h"

namespace ShapeJudge {

void solve(const XZYEllipseCylinder& xzy_ellipse_cylinder, const std::filesystem::path& x_ii_file,
           const std::filesystem::path& mask_output_file, ReadMode read_mode) {
  Xii x_ii;
  read(x_ii_file, x_ii, read_mode);
  Mask mask;
  mask.number_ = x_ii.number_;
  mask.result_ = std::make_unique<Eigen::VectorXi>(mask.number_);
  mask.result_->setZero();
  Eigen::Vector<double, 6> box;
  calAABB(xzy_ellipse_cylinder, box);
#pragma omp parallel for num_threads(omp_get_thread_num())
  for (Eigen::Index i = 0; i < static_cast<Eigen::Index>(x_ii.number_); i++) {
    const Eigen::Vector3d point = x_ii.point_->col(i);
    mask.result_->operator()(i) =
        static_cast<int>(intersectBox(point, box) && intersectXZYEllipseCylinder(point, xzy_ellipse_cylinder));
  }
  MaskIO::outputMask(mask, mask_output_file);
}

void calAABB(const XZYEllipseCylinder& xzy_ellipse_cylinder, Eigen::Vector<double, 6>& box) {
  box << (*xzy_ellipse_cylinder.center_) -
             (Eigen::Vector3d(xzy_ellipse_cylinder.semi_major_axe_, 0, xzy_ellipse_cylinder.semi_minor_axe_)),
      (*xzy_ellipse_cylinder.center_) +
          (Eigen::Vector3d(xzy_ellipse_cylinder.semi_major_axe_, xzy_ellipse_cylinder.height_,
                           xzy_ellipse_cylinder.semi_minor_axe_));
}

bool intersectBox(const Eigen::Vector3d& point, const Eigen::Vector<double, 6>& box) {
  return (point - box.head<3>()).minCoeff() >= 0 && (point - box.tail<3>()).maxCoeff() <= 0;
}

bool intersectXZYEllipseCylinder(const Eigen::Vector3d& point, const XZYEllipseCylinder& xzy_ellipse_cylinder) {
  return (std::pow((point.x() - xzy_ellipse_cylinder.center_->x()) / xzy_ellipse_cylinder.semi_major_axe_, 2) +
          std::pow((point.z() - xzy_ellipse_cylinder.center_->z()) / xzy_ellipse_cylinder.semi_minor_axe_, 2)) <= 1;
}

}  // namespace ShapeJudge
