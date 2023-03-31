#include "basic/solve.h"

void rotateGrid(const int theta_difference, const std::filesystem::path& grid_rotate_file,
                const RotateFile& rotate_file, std::unique_ptr<Eigen::Vector3d>&& point1,
                std::unique_ptr<Eigen::Vector3d>&& point2) {
  const auto times = static_cast<Index>(360 / theta_difference);
  GridRotate::Rotation rotation{0, nullptr, nullptr};
  rotation.vec_ = std::make_unique<Eigen::Vector3d>();
  (*rotation.vec_) = ((*point2) - (*point1)).normalized();
  rotation.point_ = std::move(point1);
  std::cout << fmt::format("Begin rotate grid {}. Theta = {}.", grid_rotate_file.filename().string(), theta_difference)
            << std::endl;
  std::cout << "Point is " << rotation.point_->transpose() << '.' << std::endl;
  std::cout << "Vec is " << rotation.vec_->transpose() << '.' << std::endl;
  for (const auto i : tq::trange(1UL, times)) {
    rotation.theta_ = theta_difference * static_cast<int>(i);
    std::filesystem::path grid_rotate_output_file =
        rotate_file.grid_rotate_output_dir_vec_[static_cast<Index>(i - 1)] / grid_rotate_file.filename();
    GridRotate::solve(grid_rotate_file, grid_rotate_output_file, rotation);
  }
  std::cout << std::endl;
}

void calMask(const Index x_ii_num, const File& file, const ReadMode read_mode, const MaskMode mask_mode) {
  std::cout << "Begin output mask file:" << std::endl;
  for (const auto i : tq::trange(x_ii_num)) {
    std::filesystem::path x_ii_file = FileName::xiiInputFile(i, file.x_ii_dir_, read_mode);
    std::filesystem::path mask_output_file = FileName::maskOutputFile(i, file.mask_output_dir_);
    RayCasting::solve(file.grid_0_dir_, x_ii_file, mask_output_file, read_mode, mask_mode);
  }
  std::cout << std::endl;
}

void calMask(const Index x_ii_num, const int theta, const std::filesystem::path& grid_input_dir,
             const RotateFile& rotate_file, const ReadMode read_mode, const MaskMode mask_mode) {
  std::cout << fmt::format("<<<<<<<<<< Rotate theta = {} >>>>>>>>>>", theta) << std::endl;
  for (const auto i : tq::trange(x_ii_num)) {
    std::filesystem::path x_ii_file = FileName::xiiInputFile(i, rotate_file.x_ii_dir_, read_mode);
    std::filesystem::path mask_output_file = FileName::maskOutputFile(i, theta, rotate_file.mask_output_dir_);
    RayCasting::solve(grid_input_dir, x_ii_file, mask_output_file, read_mode, mask_mode);
  }
  std::cout << std::endl;
}

void calMask(Index x_ii_num, const ShapeJudge::XZYEllipseCylinder& xzy_ellipse_cylinder, const File& file,
             ReadMode read_mode) {
  std::cout << "Begin output mask file:" << std::endl;
  for (const auto i : tq::trange(x_ii_num)) {
    std::filesystem::path x_ii_file = FileName::xiiInputFile(i, file.x_ii_dir_, read_mode);
    std::filesystem::path mask_output_file = FileName::maskOutputFile(i, file.mask_output_dir_);
    ShapeJudge::solve(xzy_ellipse_cylinder, x_ii_file, mask_output_file, read_mode);
  }
  std::cout << std::endl;
}

void testPoint(const Index x_ii_num, const File& file, const ReadMode read_mode) {
  PointTest::solve(x_ii_num, file.x_ii_dir_, file.test_input_dir_, file.test_output_file_, read_mode);
}

void testPoint(const Index x_ii_num, const int theta, const RotateFile& rotate_file, const ReadMode read_mode) {
  PointTest::solve(x_ii_num, theta, rotate_file.x_ii_dir_, rotate_file.test_input_dir_, rotate_file.test_output_dir_,
                   read_mode);
}
