#include "grid/ray_casting.h"

namespace RayCasting {

void solve(const std::filesystem::path& grid_input_dir, const std::filesystem::path& x_ii_file,
           const std::filesystem::path& mask_output_file, const ReadMode read_mode, const MaskMode mask_mode) {
  std::vector<GridStructure> grid_vec;
  std::vector<Box> box_vec;
  Xii x_ii;
  readGrid(grid_input_dir, grid_vec);
  readXii(x_ii_file, x_ii, read_mode);
  calAABB(grid_vec, box_vec);
#pragma omp parallel for num_threads(omp_get_thread_num())
  for (Eigen::Index i = 0; i < static_cast<Eigen::Index>(x_ii.number_); i++) {
    Eigen::Vector3d point = x_ii.point_->col(i);
    for (Index j = 0; j < grid_vec.size(); j++) {
      if (castRay(point, grid_vec[j], box_vec[j])) {
        switch (mask_mode) {
        case MaskMode::kBool:
          x_ii.result_->operator()(i) = 1;
          break;
        case MaskMode::kIndex:
          x_ii.result_->operator()(i) = static_cast<Eigen::VectorXi::Scalar>(j + 1);
          break;
        };
        break;
      }
    }
  }
  outputMask(x_ii, mask_output_file);
}

void readGrid(const std::filesystem::path& grid_input_dir, std::vector<GridStructure>& grid_vec) {
  std::ifstream fin;
  fin.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  int ignore;
  std::vector<std::filesystem::path> grid_file_vec;
  std::copy(std::filesystem::directory_iterator{grid_input_dir}, std::filesystem::directory_iterator{},
            std::back_inserter(grid_file_vec));
  std::sort(grid_file_vec.begin(), grid_file_vec.end());
  for (const auto& grid_file : grid_file_vec) {
    GridStructure grid;
    try {
      fin.open(grid_file, std::ios_base::in);
      fin >> grid.number_[0];
      grid.point_ = std::make_unique<Eigen::Matrix3Xd>(3, grid.number_[0]);
      for (Eigen::Index i = 0; i < static_cast<Eigen::Index>(grid.number_[0]); i++) {
        fin >> grid.point_->operator()(0, i) >> grid.point_->operator()(1, i) >> grid.point_->operator()(2, i);
      }
      fin >> grid.number_[1];
      grid.index_ = std::make_unique<Eigen::Matrix3Xi>(3, grid.number_[1]);
      for (Eigen::Index i = 0; i < static_cast<Eigen::Index>(grid.number_[1]); i++) {
        fin >> grid.index_->operator()(0, i) >> grid.index_->operator()(1, i) >> grid.index_->operator()(2, i) >>
            ignore;
      }
      fin.close();
    } catch (const std::ifstream::failure& exception) {
      std::cerr << fmt::format("Explanatory string: {}.", exception.what()) << std::endl;
      std::cerr << fmt::format("File: {}.", grid_file.string()) << std::endl;
      std::exit(EXIT_FAILURE);
    }
    grid_vec.emplace_back(std::move(grid));
  }
}

void readXii(const std::filesystem::path& x_ii_file, Xii& x_ii, const ReadMode read_mode) {
  std::ifstream fin;
  fin.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    switch (read_mode) {
    case ReadMode::kText:
      fin.open(x_ii_file, std::ios_base::in);
      fin >> x_ii.number_;
      x_ii.point_ = std::make_unique<Eigen::Matrix3Xd>(3, x_ii.number_);
      for (Eigen::Index i = 0; i < static_cast<Eigen::Index>(x_ii.number_); i++) {
        fin >> x_ii.point_->operator()(0, i) >> x_ii.point_->operator()(1, i) >> x_ii.point_->operator()(2, i);
      }
      break;
    case ReadMode::kBinary:
      fin.open(x_ii_file, std::ios_base::in | std::ios_base::binary);
      fin.read(reinterpret_cast<char*>(&x_ii.number_), static_cast<std::streamsize>(sizeof(int)));
      x_ii.point_ = std::make_unique<Eigen::Matrix3Xd>(3, x_ii.number_);
      fin.read(reinterpret_cast<char*>(x_ii.point_->data()),
               x_ii.point_->size() * static_cast<std::streamsize>(sizeof(Eigen::Matrix3Xd::Scalar)));
      break;
    }
    x_ii.result_ = std::make_unique<Eigen::VectorXi>(x_ii.number_);
    x_ii.result_->setZero();
    fin.close();
  } catch (const std::ifstream::failure& exception) {
    std::cerr << fmt::format("Explanatory string: {}.", exception.what()) << std::endl;
    std::cerr << fmt::format("File: {}.", x_ii_file.string()) << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

void calAABB(std::vector<GridStructure>& grid_vec, std::vector<Box>& box_vec) {
  for (const auto& grid : grid_vec) {
    Box box;
    box.axis_box_ = std::make_unique<Eigen::Matrix<double, 6, Eigen::Dynamic>>(6, grid.number_[1]);
    box.object_box_ = std::make_unique<Eigen::Vector<double, 6>>();
    Eigen::Matrix3d tri_coord;
    for (Eigen::Index i = 0; i < static_cast<Eigen::Index>(grid.number_[1]); i++) {
      for (Eigen::Index j = 0; j < 3; j++) {
        tri_coord.col(j) = grid.point_->col(grid.index_->operator()(j, i) - 1);
      }
      box.axis_box_->col(i) << tri_coord.rowwise().minCoeff(), tri_coord.rowwise().maxCoeff();
    }
    (*box.object_box_) << box.axis_box_->topRows(3).rowwise().minCoeff(),
        box.axis_box_->bottomRows(3).rowwise().maxCoeff();
    box_vec.emplace_back(std::move(box));
  }
}

bool castRay(const Eigen::Vector3d& point, GridStructure& grid, Box& box) {
  Index num = 0;
  std::vector<Eigen::Index> tri_index;
  Eigen::Matrix3d tri_coord;
  if ((point - box.object_box_->head<3>()).minCoeff() < 0 || (point - box.object_box_->tail<3>()).maxCoeff() > 0) {
    return false;
  }
  for (Eigen::Index i = 0; i < static_cast<Eigen::Index>(grid.number_[1]); i++) {
    if (point(1) >= box.axis_box_->operator()(1, i) && point(1) <= box.axis_box_->operator()(4, i) &&
        point(2) >= box.axis_box_->operator()(2, i) && point(2) <= box.axis_box_->operator()(5, i)) {
      for (Eigen::Index j = 0; j < 3; j++) {
        tri_coord.col(j) = grid.point_->col(grid.index_->operator()(j, i) - 1);
      }
      TriFlag tri_flag = intersectTri(point, tri_coord);
      switch (tri_flag) {
      case TriFlag::kOnTri:
        return true;
        break;
      case TriFlag::kCrossTri:
        num++;
        break;
      case TriFlag::kOnCorner:
        tri_index.push_back(i);
        break;
      case TriFlag::kOutTri:
        break;
      }
      // std::cout << "tri_flag:" << tri_flag << '\t' << "flag:" << flag << std::endl << (*tri_coord) << std::endl;
    }
  }
  return judgeSingularity(num, tri_index, point, grid, box);
}

TriFlag intersectTri(const Eigen::Vector3d& point, Eigen::Matrix3d& tri_coord) {
  const Eigen::Vector3d direction{1.0, 0.0, 0.0};  // !!!
  Eigen::Vector3d t_vec = point - tri_coord.col(0);
  Eigen::Vector3d edge_1 = tri_coord.col(1) - tri_coord.col(0);
  Eigen::Vector3d edge_2 = tri_coord.col(2) - tri_coord.col(0);
  Eigen::Vector3d p_vec = direction.cross(edge_2);
  Eigen::Vector3d q_vec = t_vec.cross(edge_1);
  const double determinant = edge_1.dot(p_vec);
  if (std::abs(determinant) > kEpsilon) {
    const double u = p_vec.dot(t_vec) / determinant;
    const double v = q_vec.dot(direction) / determinant;
    const double t = q_vec.dot(edge_2) / determinant;
    // std::cout << determinant << '\t' << t << '\t' << u << '\t' << v << std::endl;
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
  // std::cout << determinant << '\t' << u << '\t' << v << std::endl;
  if (std::abs(u) < kEpsilon && std::abs(v) < kEpsilon) {
    const double det = edge_1.dot(edge_1) * edge_2.dot(edge_2) - std::pow(edge_1.dot(edge_2), 2);
    const double u_2d = (edge_1.dot(t_vec) * edge_2.dot(edge_2) - edge_2.dot(t_vec) * edge_1.dot(edge_2)) / det;
    const double v_2d = (edge_2.dot(t_vec) * edge_1.dot(edge_1) - edge_1.dot(t_vec) * edge_1.dot(edge_2)) / det;
    // std::cout << det << '\t' << u_2d << '\t' << v_2d << std::endl;
    if (u_2d < -kEpsilon || v_2d < -kEpsilon || u_2d + v_2d - 1.0 > kEpsilon) {
      if (((tri_coord.row(0).array() - point.operator()(0)) > 0).count() > 1) {
        return TriFlag::kOnCorner;
      }
      return TriFlag::kOutTri;
    }
    return TriFlag::kOnTri;
  }
  return TriFlag::kOutTri;
}

bool judgeSingularity(Index num, std::vector<Eigen::Index>& tri_index, const Eigen::Vector3d& point,
                      GridStructure& grid, Box& box) {
  std::vector<Eigen::Index> tri_group;
  Eigen::Vector<double, 6> all_axis_box;
  while (!tri_index.empty()) {
    for (auto iter = tri_index.begin(); iter != tri_index.end();) {
      if (!tri_group.empty()) {
        if (detectIntersect(tri_group[0], *iter, grid)) {
          tri_group.push_back(*iter);
          tri_index.erase(iter);
        } else {
          iter++;
        }
      } else {
        tri_group.push_back(*iter);
        tri_index.erase(iter);
      }
    }
    Eigen::Matrix<double, 6, Eigen::Dynamic> mul_axis_box(6, tri_group.size());
    all_axis_box << box.axis_box_->col(tri_group[0]);
    for (Index i = 0; i < tri_group.size(); i++) {
      mul_axis_box.col(static_cast<Eigen::Index>(i)) << box.axis_box_->col(tri_group[i]);
    }
    all_axis_box << mul_axis_box.topRows(3).rowwise().minCoeff(), mul_axis_box.bottomRows(3).rowwise().maxCoeff();
    if (point(1) > all_axis_box(1) && point(1) < all_axis_box(4) && point(2) > all_axis_box(2) &&
        point(2) < all_axis_box(5)) {
      num++;
    }
    tri_group.clear();
  }
  return (num & 1) == 1;
}

bool detectIntersect(const Eigen::Index tri1, const Eigen::Index tri2, GridStructure& grid) {
  for (Eigen::Index i = 0; i < 3; i++) {
    for (Eigen::Index j = 0; j < 3; j++) {
      if (grid.index_->operator()(i, tri1) == grid.index_->operator()(j, tri2)) {
        return true;
      }
    }
  }
  return false;
}

void outputMask(Xii& x_ii, const std::filesystem::path& mask_output_file) {
  std::ofstream fout;
  fout.exceptions(std::ofstream::failbit | std::ofstream::badbit);
  try {
    fout.open(mask_output_file, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
    fout.write(reinterpret_cast<const char*>(x_ii.result_->data()),
               x_ii.result_->size() * static_cast<std::streamsize>(sizeof(Eigen::VectorXi::Scalar)));
    fout.close();
  } catch (const std::ofstream::failure& exception) {
    std::cerr << fmt::format("Explanatory string: {}.", exception.what()) << std::endl;
    std::cerr << fmt::format("File: {}.", mask_output_file.string()) << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

}  // namespace RayCasting
