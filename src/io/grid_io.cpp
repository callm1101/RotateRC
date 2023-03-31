#include "io/grid_io.h"

namespace GridIO {

void readGrid(const std::filesystem::path& grid_input_file, Grid& grid) {
  std::ifstream fin;
  fin.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    fin.open(grid_input_file, std::ios_base::in);
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
    std::cerr << fmt::format("File: {}.", grid_input_file.string()) << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

void readGrid(const std::filesystem::path& grid_input_dir, std::vector<Grid>& grid_vec) {
  std::ifstream fin;
  fin.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  std::vector<std::filesystem::path> grid_file_vec;
  std::copy(std::filesystem::directory_iterator{grid_input_dir}, std::filesystem::directory_iterator{},
            std::back_inserter(grid_file_vec));
  std::sort(grid_file_vec.begin(), grid_file_vec.end());
  for (const auto& grid_file : grid_file_vec) {
    Grid grid;
    readGrid(grid_file, grid);
    grid_vec.emplace_back(std::move(grid));
  }
}

void outputGrid(const std::filesystem::path& grid_output_file, Grid& grid, OutputMode output_mode) {
  std::ofstream fout;
  fout.exceptions(std::ofstream::failbit | std::ofstream::badbit);
  fout.setf(std::ios_base::left, std::ios_base::adjustfield);
  fout.setf(std::ios_base::scientific, std::ios_base::floatfield);
  fout.precision(15);
  try {
    switch (output_mode) {
    case OutputMode::kText:
      fout.open(grid_output_file, std::ios_base::out | std::ios_base::trunc);
      fout << grid.number_[0] << std::endl;
      fout << grid.point_->transpose() << std::endl;
      break;
    case OutputMode::kBinary:
      fout.open(grid_output_file, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
      fout.write(reinterpret_cast<const char*>(&grid.number_), static_cast<std::streamsize>(sizeof(int)));
      fout.write(reinterpret_cast<const char*>(grid.point_->transpose().data()),
                 grid.point_->size() * static_cast<std::streamsize>(sizeof(Eigen::Matrix3Xd::Scalar)));
      break;
    }
    fout.close();
  } catch (std::ofstream::failure exception) {
    std::cerr << fmt::format("Explanatory string: {}.", exception.what()) << std::endl;
    std::cerr << fmt::format("File: {}.", grid_output_file.string()) << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

void outputGrid(const std::filesystem::path& grid_output_file, Grid& grid, GridHandle& grid_handle) {
  std::ofstream fout;
  fout.exceptions(std::ofstream::failbit | std::ofstream::badbit);
  fout.setf(std::ios_base::left, std::ios_base::adjustfield);
  fout.setf(std::ios_base::scientific, std::ios_base::floatfield);
  fout.precision(15);
  try {
    fout.open(grid_output_file, std::ios_base::out | std::ios_base::trunc);
    fout << grid.number_[0] << std::endl;
    fout << grid_handle.point_->transpose() << std::endl;
    fout << grid.number_[1] << std::endl;
    fout << grid.index_->transpose() << std::endl;
    fout.close();
  } catch (std::ofstream::failure exception) {
    std::cerr << fmt::format("Explanatory string: {}.", exception.what()) << std::endl;
    std::cerr << fmt::format("File: {}.", grid_output_file.string()) << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

}  // namespace GridIO
