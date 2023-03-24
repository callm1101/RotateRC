#include "rotate_rc.h"

int main(int argc, char* argv[]) {
  static_cast<void>(argc);
  static_cast<void>(argv);
  File file;

  constexpr ReadMode kReadMode = ReadMode::kText;
  // constexpr ReadMode kReadMode = ReadMode::kBinary;

  // constexpr OutputMode kOutputMode = OutputMode::kText;
  constexpr OutputMode kOutputMode = OutputMode::kBinary;

  rayCasting(kReadMode, MaskMode::kBool, file);
  // rotateRC(1, kReadMode, MaskMode::kIndex, file);

  // scaleGrid(1e-3, file);
  // sliceGrid(kOutputMode, file);

  return EXIT_SUCCESS;
}

void rayCasting(const ReadMode read_mode, const MaskMode mask_mode, File& file) {
  makeMaskDir(file);
  const auto x_ii_num = static_cast<Index>(
      std::distance(std::filesystem::directory_iterator{file.x_ii_dir_}, std::filesystem::directory_iterator{}));
  calMask(x_ii_num, file, read_mode, mask_mode);
  testPoint(x_ii_num, file, read_mode);
}

void rotateRC(const int theta_difference, const ReadMode read_mode, const MaskMode mask_mode, File& file) {
  file.grid_copy_file_vec_.emplace_back(file.root_ / "dat/grid/grid_0/body.dat");
  file.grid_rotate_file_vec_.emplace_back(file.root_ / "dat/grid/grid_0/lxj1.dat");
  file.grid_rotate_file_vec_.emplace_back(file.root_ / "dat/grid/grid_0/lxj2.dat");

  // ---------------------------------------------------------------------------
  makeRotateDir(theta_difference, file);
  {
    std::unique_ptr<Eigen::Vector3d> point1 = std::make_unique<Eigen::Vector3d>(
        std::initializer_list<std::initializer_list<double>>{{2.6, -3.5475194, 0.97825646}});
    std::unique_ptr<Eigen::Vector3d> point2 = std::make_unique<Eigen::Vector3d>(
        std::initializer_list<std::initializer_list<double>>{{2.6, -7.3689222, 0.64392703}});
    rotateGrid(theta_difference, file.grid_rotate_file_vec_[0], file, std::move(point1), std::move(point2));
  }
  {
    std::unique_ptr<Eigen::Vector3d> point1 = std::make_unique<Eigen::Vector3d>(
        std::initializer_list<std::initializer_list<double>>{{7.06, -7.0850068, 0.66876642}});
    std::unique_ptr<Eigen::Vector3d> point2 = std::make_unique<Eigen::Vector3d>(
        std::initializer_list<std::initializer_list<double>>{{7.06, -3.6710475, 0.96744915}});
    rotateGrid(theta_difference, file.grid_rotate_file_vec_[1], file, std::move(point1), std::move(point2));
  }
  // ---------------------------------------------------------------------------
  makeMaskDir(file);
  const auto x_ii_num = static_cast<Index>(
      std::distance(std::filesystem::directory_iterator{file.x_ii_dir_}, std::filesystem::directory_iterator{}));
  for (Index i = 0; const auto& grid_input_dir : file.grid_rotate_output_dir_vec_) {
    calMask(x_ii_num, static_cast<int>(i) * theta_difference, grid_input_dir, file, read_mode, mask_mode);
    testPoint(x_ii_num, static_cast<int>(i++) * theta_difference, file, read_mode);
  }
  // ---------------------------------------------------------------------------
  delRotateDir(file);
}
