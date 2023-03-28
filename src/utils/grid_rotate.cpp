#include "utils/grid_rotate.h"

namespace GridRotate {

void solve(const std::filesystem::path& grid_input_file, const std::filesystem::path& grid_output_file,
           const Rotation& rotation) {
  Grid grid;
  GridHandle grid_handle;
  GridIO::readGrid(grid_input_file, grid);
  rotateGrid(rotation, grid, grid_handle);
  GridIO::outputGrid(grid_output_file, grid, grid_handle);
}

void rotateGrid(const Rotation& rotation, Grid& grid, GridHandle& grid_handle) {
  grid_handle.number_ = grid.number_[0];
  grid_handle.point_ = std::make_unique<Eigen::Matrix3Xd>(3, grid_handle.number_);
  const double sin_theta = std::sin(rotation.theta_ * kPi / 180);
  const double cos_theta = std::cos(rotation.theta_ * kPi / 180);
  Eigen::Matrix3d rotate_matrix;
  Eigen::Vector3d rotate_vector;
  const double m = rotation.vec_->dot(*rotation.point_);
  rotate_vector << (rotation.vec_->z() * rotation.point_->y() - rotation.vec_->y() * rotation.point_->z()) * sin_theta,
      (rotation.vec_->x() * rotation.point_->z() - rotation.vec_->z() * rotation.point_->x()) * sin_theta,
      (rotation.vec_->y() * rotation.point_->x() - rotation.vec_->x() * rotation.point_->y()) * sin_theta;
  rotate_matrix << cos_theta, -rotation.vec_->z() * sin_theta, rotation.vec_->y() * sin_theta,
      rotation.vec_->z() * sin_theta, cos_theta, -rotation.vec_->x() * sin_theta, -rotation.vec_->y() * sin_theta,
      rotation.vec_->x() * sin_theta, cos_theta;
  rotate_vector += ((*rotation.point_) - (*rotation.vec_) * m) * (1 - cos_theta);
  rotate_matrix += (*rotation.vec_) * rotation.vec_->transpose() * (1 - cos_theta);
  for (Eigen::Index i = 0; i < static_cast<Eigen::Index>(grid_handle.number_); i++) {
    grid_handle.point_->col(i) = rotate_matrix * grid.point_->col(i) + rotate_vector;
  }
}

}  // namespace GridRotate
