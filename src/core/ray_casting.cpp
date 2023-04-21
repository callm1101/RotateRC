#include "core/ray_casting.h"

namespace RayCasting {

void solve(const std::filesystem::path& grid_input_dir, const std::filesystem::path& x_ii_file,
           const std::filesystem::path& mask_output_file, const ReadMode read_mode, const MaskMode mask_mode) {
  std::vector<Grid> grid_vec;
  Xii x_ii;
  read(grid_input_dir, x_ii_file, grid_vec, x_ii, read_mode);
  Mask mask;
  mask.number_ = x_ii.number_;
  mask.result_ = std::make_unique<Eigen::VectorXi>(mask.number_);
  mask.result_->setZero();
  std::vector<Box> box_vec;
  calAxisBox(grid_vec, box_vec);
#pragma omp parallel for num_threads(omp_get_thread_num())
  for (Eigen::Index i = 0; i < static_cast<Eigen::Index>(x_ii.number_); i++) {
    const Eigen::Vector3d point = x_ii.point_->col(i);
    for (Index j = 0; j < grid_vec.size(); j++) {
      if (castRay(point, grid_vec[j], box_vec[j])) {
        switch (mask_mode) {
        case MaskMode::kBool:
          mask.result_->operator()(i) = 1;
          break;
        case MaskMode::kIndex:
          mask.result_->operator()(i) = static_cast<Eigen::VectorXi::Scalar>(j + 1);
          break;
        };
        break;
      }
    }
  }
  MaskIO::outputMask(mask, mask_output_file);
}

void calAxisBox(std::vector<Grid>& grid_vec, std::vector<Box>& box_vec) {
  for (const auto& grid : grid_vec) {
    Box box;
    box.axis_box_ = std::make_unique<Eigen::Matrix<double, 6, Eigen::Dynamic>>(6, grid.number_[1]);
    box.object_box_ = std::make_unique<Eigen::Vector<double, 6>>();
    Eigen::Matrix3d triangle_coord;
    for (Eigen::Index i = 0; i < static_cast<Eigen::Index>(grid.number_[1]); i++) {
      for (Eigen::Index j = 0; j < 3; j++) {
        triangle_coord.col(j) = grid.point_->col(grid.index_->operator()(j, i) - 1);
      }
      box.axis_box_->col(i) << triangle_coord.rowwise().minCoeff(), triangle_coord.rowwise().maxCoeff();
    }
    (*box.object_box_) << box.axis_box_->topRows(3).rowwise().minCoeff(),
        box.axis_box_->bottomRows(3).rowwise().maxCoeff();
    box_vec.emplace_back(std::move(box));
  }
}

bool castRay(const Eigen::Vector3d& point, Grid& grid, Box& box) {
  Index number = 0;
  std::vector<Eigen::Index> triangle_list;
  Eigen::Matrix3d triangle_coord;
  if ((point - box.object_box_->head<3>()).minCoeff() < 0 || (point - box.object_box_->tail<3>()).maxCoeff() > 0) {
    return false;
  }
  for (Eigen::Index i = 0; i < static_cast<Eigen::Index>(grid.number_[1]); i++) {
    if (point.y() >= box.axis_box_->operator()(1, i) && point(1) <= box.axis_box_->operator()(4, i) &&
        point.z() >= box.axis_box_->operator()(2, i) && point(2) <= box.axis_box_->operator()(5, i)) {
      for (Eigen::Index j = 0; j < 3; j++) {
        triangle_coord.col(j) = grid.point_->col(grid.index_->operator()(j, i) - 1);
      }
      TriFlag flag = intersectTri(point, triangle_coord);
      switch (flag) {
      case TriFlag::kOnTri:
        return true;
        break;
      case TriFlag::kCrossTri:
        number++;
        break;
      case TriFlag::kOnCorner:
        triangle_list.push_back(i);
        break;
      case TriFlag::kOutTri:
        break;
      }
    }
  }
  return judgeSingularity(number, triangle_list, point, grid, box);
}

TriFlag intersectTri(const Eigen::Vector3d& point, Eigen::Matrix3d& triangle_coord) {
  const Eigen::Vector3d direction{1.0, 0.0, 0.0};  // !!! do not modify drection, it is coupled to box.
  Eigen::Vector3d t_vec = point - triangle_coord.col(0);
  Eigen::Vector3d edge_1 = triangle_coord.col(1) - triangle_coord.col(0);
  Eigen::Vector3d edge_2 = triangle_coord.col(2) - triangle_coord.col(0);
  Eigen::Vector3d p_vec = direction.cross(edge_2);
  Eigen::Vector3d q_vec = t_vec.cross(edge_1);
  const double determinant = edge_1.dot(p_vec);
  if (std::abs(determinant) > kEpsilon) {
    const double u = p_vec.dot(t_vec) / determinant;
    const double v = q_vec.dot(direction) / determinant;
    const double t = q_vec.dot(edge_2) / determinant;
    if (t < -kEpsilon) {
      return TriFlag::kOutTri;
    }
    if (t > kEpsilon) {
      if (u < -kEpsilon || v < -kEpsilon || u + v - 1.0 > kEpsilon) {
        return TriFlag::kOutTri;
      }
      if (std::abs(u) < kEpsilon || std::abs(v) < kEpsilon || std::abs(u + v - 1.0) < kEpsilon) {
        return TriFlag::kOnCorner;
      }
      return TriFlag::kCrossTri;
    }
    return TriFlag::kOnTri;
  }
  const double u = p_vec.dot(t_vec);
  const double v = q_vec.dot(direction);
  if (std::abs(u) < kEpsilon && std::abs(v) < kEpsilon) {
    const double determinant = edge_1.dot(edge_1) * edge_2.dot(edge_2) - std::pow(edge_1.dot(edge_2), 2);
    const double u_2d = (edge_1.dot(t_vec) * edge_2.dot(edge_2) - edge_2.dot(t_vec) * edge_1.dot(edge_2)) / determinant;
    const double v_2d = (edge_2.dot(t_vec) * edge_1.dot(edge_1) - edge_1.dot(t_vec) * edge_1.dot(edge_2)) / determinant;
    if (u_2d < -kEpsilon || v_2d < -kEpsilon || u_2d + v_2d - 1.0 > kEpsilon) {
      if (((triangle_coord.row(0).array() - point.operator()(0)) > 0).count() > 1) {
        return TriFlag::kOnCorner;
      }
      return TriFlag::kOutTri;
    }
    return TriFlag::kOnTri;
  }
  return TriFlag::kOutTri;
}

bool judgeSingularity(Index num, std::vector<Eigen::Index>& triangle_list, const Eigen::Vector3d& point, Grid& grid,
                      Box& box) {
  std::vector<Eigen::Index> triangle_group;
  Eigen::Vector<double, 6> all_axis_box;
  while (!triangle_list.empty()) {
    for (auto iter = triangle_list.begin(); iter != triangle_list.end();) {
      if (!triangle_group.empty()) {
        if (detectIntersect(triangle_group[0], *iter, grid)) {
          triangle_group.push_back(*iter);
          triangle_list.erase(iter);
        } else {
          iter++;
        }
      } else {
        triangle_group.push_back(*iter);
        triangle_list.erase(iter);
      }
    }
    Eigen::Matrix<double, 6, Eigen::Dynamic> mul_axis_box(6, triangle_group.size());
    all_axis_box << box.axis_box_->col(triangle_group[0]);
    for (Index i = 0; i < triangle_group.size(); i++) {
      mul_axis_box.col(static_cast<Eigen::Index>(i)) << box.axis_box_->col(triangle_group[i]);
    }
    all_axis_box << mul_axis_box.topRows(3).rowwise().minCoeff(), mul_axis_box.bottomRows(3).rowwise().maxCoeff();
    if (point(1) > all_axis_box(1) && point(1) < all_axis_box(4) && point(2) > all_axis_box(2) &&
        point(2) < all_axis_box(5)) {
      num++;
    }
    triangle_group.clear();
  }
  return (num & 1) == 1;
}

bool detectIntersect(const Eigen::Index tri1, const Eigen::Index tri2, Grid& grid) {
  for (Eigen::Index i = 0; i < 3; i++) {
    for (Eigen::Index j = 0; j < 3; j++) {
      if (grid.index_->operator()(i, tri1) == grid.index_->operator()(j, tri2)) {
        return true;
      }
    }
  }
  return false;
}

}  // namespace RayCasting
