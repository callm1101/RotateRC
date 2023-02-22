#include "core/solve.h"

void rayCasting(const ReadMode read_mode, const MaskMode mask_mode, File& file) {
  makeMaskDir(file);
  const auto x_ii_num = static_cast<Index>(
      std::distance(std::filesystem::directory_iterator{file.x_ii_dir_}, std::filesystem::directory_iterator{}));
  calMask(x_ii_num, file, read_mode, mask_mode);
  testPoint(x_ii_num, file, read_mode);
}

void rotateRC(const ReadMode read_mode, const MaskMode mask_mode, File& file) {
  file.grid_copy_file_vec_.emplace_back(file.root_ / "dat/grid/grid_0/body.dat");
  file.grid_rotate_file_vec_.emplace_back(file.root_ / "dat/grid/grid_0/lxj1.dat");
  file.grid_rotate_file_vec_.emplace_back(file.root_ / "dat/grid/grid_0/lxj2.dat");

  constexpr int kThetaDifference = 1;

  // ---------------------------------------------------------------------------
  makeRotateDir(kThetaDifference, file);
  {
    std::unique_ptr<Eigen::Vector3d> point1 = std::make_unique<Eigen::Vector3d>(
        std::initializer_list<std::initializer_list<double>>{{2.6, -3.5475194, 0.97825646}});
    std::unique_ptr<Eigen::Vector3d> point2 = std::make_unique<Eigen::Vector3d>(
        std::initializer_list<std::initializer_list<double>>{{2.6, -7.3689222, 0.64392703}});
    rotateGrid(kThetaDifference, file.grid_rotate_file_vec_[0], file, std::move(point1), std::move(point2));
  }
  {
    std::unique_ptr<Eigen::Vector3d> point1 = std::make_unique<Eigen::Vector3d>(
        std::initializer_list<std::initializer_list<double>>{{7.06, -7.0850068, 0.66876642}});
    std::unique_ptr<Eigen::Vector3d> point2 = std::make_unique<Eigen::Vector3d>(
        std::initializer_list<std::initializer_list<double>>{{7.06, -3.6710475, 0.96744915}});
    rotateGrid(kThetaDifference, file.grid_rotate_file_vec_[1], file, std::move(point1), std::move(point2));
  }
  // ---------------------------------------------------------------------------
  makeMaskDir(file);
  const auto x_ii_num = static_cast<Index>(
      std::distance(std::filesystem::directory_iterator{file.x_ii_dir_}, std::filesystem::directory_iterator{}));
  for (Index i = 0; const auto& grid_input_dir : file.grid_rotate_output_dir_vec_) {
    calMask(x_ii_num, static_cast<int>(i) * kThetaDifference, grid_input_dir, file, read_mode, mask_mode);
    testPoint(x_ii_num, static_cast<int>(i++) * kThetaDifference, file, read_mode);
  }
  // ---------------------------------------------------------------------------
  delRotateDir(file);
}

int main(int argc, char* argv[]) {
  static_cast<void>(argc);
  static_cast<void>(argv);
  File file;
  file.root_ = std::filesystem::path{kProjectSourceDir};
  file.grid_0_dir_ = file.root_ / "dat/grid/grid_0";
  file.x_ii_dir_ = file.root_ / "dat/X_ii";
  file.output_dir_ = file.root_ / "build/out";
  file.mask_output_dir_ = file.root_ / "build/out/mask";
  file.test_input_dir_ = file.root_ / "build/out/mask";
  file.test_output_dir_ = file.root_ / "build/out/test";
  file.test_output_file_ = file.root_ / "build/out/test.plt";

  constexpr ReadMode kReadMode = ReadMode::kText;
  // constexpr ReadMode kReadMode = ReadMode::kBinary;

  // scaleGrid(1e-3, file);
  rayCasting(kReadMode, MaskMode::kBool, file);
  rotateRC(kReadMode, MaskMode::kIndex, file);

  return EXIT_SUCCESS;
}
