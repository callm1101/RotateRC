#include "grid/grid_handle.h"

namespace GridHandle {

void solve(const std::filesystem::path& grid_handle_file, const std::filesystem::path& grid_handle_output_file,
           const Rotation& rotation) {
  GridStructure grid;
  readGrid(grid_handle_file, grid);
  rotateGrid(rotation, grid);
  outputGrid(ReadMode::kText, true, grid_handle_output_file, grid);
}

void solve(const double scale, const std::filesystem::path& grid_handle_file,
           const std::filesystem::path& grid_handle_output_file, const ReadMode output_mode, const bool flag) {
  GridStructure grid;
  readGrid(grid_handle_file, grid);
  scaleGrid(scale, grid);
  outputGrid(output_mode, flag, grid_handle_output_file, grid);
}

void readGrid(const std::filesystem::path& grid_handle_file, GridStructure& grid) {
  std::ifstream fin;
  fin.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    fin.open(grid_handle_file, std::ios_base::in);
    fin >> grid.number_[0];
    grid.point_ = std::make_unique<Eigen::Matrix3Xd>(3, grid.number_[0]);
    for (Eigen::Index i = 0; i < static_cast<Eigen::Index>(grid.number_[0]); i++) {
      fin >> grid.point_->operator()(0, i) >> grid.point_->operator()(1, i) >> grid.point_->operator()(2, i);
    }
    fin >> grid.number_[1];
    grid.index_ = std::make_unique<Eigen::Matrix4Xi>(4, grid.number_[1]);
    for (Eigen::Index i = 0; i < static_cast<Eigen::Index>(grid.number_[1]); i++) {
      fin >> grid.index_->operator()(0, i) >> grid.index_->operator()(1, i) >> grid.index_->operator()(2, i) >>
          grid.index_->operator()(3, i);
    }
    fin.close();
  } catch (const std::ifstream::failure& exception) {
    std::cerr << fmt::format("Explanatory string: {}.", exception.what()) << std::endl;
    std::cerr << fmt::format("File: {}.", grid_handle_file.string()) << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

void rotateGrid(const Rotation& rotation, GridStructure& grid) {
  grid.handle_point_ = std::make_unique<Eigen::Matrix3Xd>(3, grid.number_[0]);
  const double sin_theta = std::sin(rotation.theta_ * kPi / 180);
  const double cos_theta = std::cos(rotation.theta_ * kPi / 180);
  Eigen::Matrix3d rotate_matrix;
  Eigen::Vector3d rotate_vector;
  const double m = rotation.vec_->dot(*rotation.point_);
  rotate_vector << (rotation.vec_->operator()(2) * rotation.point_->operator()(1) -
                    rotation.vec_->operator()(1) * rotation.point_->operator()(2)) *
                       sin_theta,
      (rotation.vec_->operator()(0) * rotation.point_->operator()(2) -
       rotation.vec_->operator()(2) * rotation.point_->operator()(0)) *
          sin_theta,
      (rotation.vec_->operator()(1) * rotation.point_->operator()(0) -
       rotation.vec_->operator()(0) * rotation.point_->operator()(1)) *
          sin_theta;
  rotate_matrix << cos_theta, -rotation.vec_->operator()(2) * sin_theta, rotation.vec_->operator()(1) * sin_theta,
      rotation.vec_->operator()(2) * sin_theta, cos_theta, -rotation.vec_->operator()(0) * sin_theta,
      -rotation.vec_->operator()(1) * sin_theta, rotation.vec_->operator()(0) * sin_theta, cos_theta;
  rotate_vector += ((*rotation.point_) - (*rotation.vec_) * m) * (1 - cos_theta);
  rotate_matrix += (*rotation.vec_) * rotation.vec_->transpose() * (1 - cos_theta);
  for (Eigen::Index i = 0; i < static_cast<Eigen::Index>(grid.number_[0]); i++) {
    grid.handle_point_->col(i) = rotate_matrix * grid.point_->col(i) + rotate_vector;
  }
}

void scaleGrid(const double scale, GridStructure& grid) {
  grid.handle_point_ = std::make_unique<Eigen::Matrix3Xd>(3, grid.number_[0]);
  (*grid.handle_point_) *= scale;
}

void outputGrid(const std::filesystem::path& grid_handle_output_file, GridStructure& grid) {
  std::ofstream fout;
  fout.exceptions(std::ofstream::failbit | std::ofstream::badbit);
  fout.setf(std::ios_base::left, std::ios_base::adjustfield);
  fout.setf(std::ios_base::scientific, std::ios_base::floatfield);
  fout.precision(15);
  try {
    fout.open(grid_handle_output_file, std::ios_base::out | std::ios_base::trunc);
    fout << grid.number_[0] << std::endl;
    fout << grid.handle_point_->transpose() << std::endl;
    fout << grid.number_[1] << std::endl;
    fout << grid.index_->transpose() << std::endl;
    fout.close();
  } catch (const std::ofstream::failure& exception) {
    std::cerr << fmt::format("Explanatory string: {}.", exception.what()) << std::endl;
    std::cerr << fmt::format("File: {}.", grid_handle_output_file.string()) << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

void outputGrid(ReadMode output_mode, bool flag, const std::filesystem::path& grid_handle_output_file,
                GridStructure& grid) {
  std::ofstream fout;
  fout.exceptions(std::ofstream::failbit | std::ofstream::badbit);
  fout.setf(std::ios_base::left, std::ios_base::adjustfield);
  fout.setf(std::ios_base::scientific, std::ios_base::floatfield);
  fout.precision(15);
  try {
    switch (output_mode) {
    case ReadMode::kText:
      fout.open(grid_handle_output_file, std::ios_base::out | std::ios_base::trunc);
      fout << grid.number_[0] << std::endl;
      fout << grid.handle_point_->transpose() << std::endl;
      break;
    case ReadMode::kBinary:
      fout.open(grid_handle_output_file, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
      fout.write(reinterpret_cast<const char*>(&grid.number_), static_cast<std::streamsize>(sizeof(int)));
      fout.write(reinterpret_cast<const char*>(grid.handle_point_->transpose().data()),
                 grid.handle_point_->size() * static_cast<std::streamsize>(sizeof(Eigen::Matrix3Xd::Scalar)));
      break;
    }
    if (flag) {
      fout << grid.number_[1] << std::endl;
      fout << grid.index_->transpose() << std::endl;
    }
    fout.close();
  } catch (std::ofstream::failure exception) {
    std::cerr << fmt::format("Explanatory string: {}.", exception.what()) << std::endl;
    std::cerr << fmt::format("File: {}.", grid_handle_output_file.string()) << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

}  // namespace GridHandle
