#include "basic/solve.h"

void makeRotateDir(const int theta_difference, File& file) {
  const auto times = static_cast<Index>(360 / theta_difference);
  file.grid_rotate_output_dir_vec_.emplace_back(file.grid_0_dir_);
  try {
    for (const auto& dir : std::filesystem::directory_iterator(file.grid_0_dir_.parent_path())) {
      if (dir.path() != file.grid_0_dir_) {
        std::filesystem::remove_all(dir);
      }
    }
  } catch (const std::filesystem::filesystem_error& exception) {
    std::cerr << fmt::format("Explanatory string: {}.", exception.what()) << std::endl;
    std::exit(EXIT_FAILURE);
  }
  for (Index i = 1; i < times; i++) {
    std::filesystem::path grid_rotate_dir = gridRotateDir(theta_difference * static_cast<int>(i), file.grid_0_dir_);
    try {
      std::filesystem::create_directory(grid_rotate_dir);
      for (const auto& grid_copy_file : file.grid_copy_file_vec_) {
        std::filesystem::path target_file = grid_rotate_dir / grid_copy_file.filename();
        std::filesystem::copy_file(grid_copy_file, target_file);
      }
    } catch (const std::filesystem::filesystem_error& exception) {
      std::cerr << fmt::format("Explanatory string: {}.", exception.what()) << std::endl;
      std::exit(EXIT_FAILURE);
    }
    file.grid_rotate_output_dir_vec_.emplace_back(grid_rotate_dir);
  }
}

void delRotateDir(File& file) {
  std::cout << "Remove temporary grid." << std::endl;
  try {
    for (const auto i : tq::trange(1UL, file.grid_rotate_output_dir_vec_.size())) {
      std::filesystem::remove_all(file.grid_rotate_output_dir_vec_[i]);
    }
  } catch (const std::filesystem::filesystem_error& exception) {
    std::cerr << fmt::format("Explanatory string: {}.", exception.what()) << std::endl;
    std::exit(EXIT_FAILURE);
  }
  std::cout << std::endl;
}

void rotateGrid(const int theta_difference, const std::filesystem::path& grid_rotate_file, const File& file,
                std::unique_ptr<Eigen::Vector3d>&& point1, std::unique_ptr<Eigen::Vector3d>&& point2) {
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
        file.grid_rotate_output_dir_vec_[static_cast<Index>(i - 1)] / grid_rotate_file.filename();
    GridRotate::solve(grid_rotate_file, grid_rotate_output_file, rotation);
  }
  std::cout << std::endl;
}

void scaleGrid(const double scale, File& file) {
  std::filesystem::path grid_handle_dir = gridHandleDir(file.grid_0_dir_);
  try {
    if (std::filesystem::exists(grid_handle_dir)) {
      std::cout << fmt::format("Remove directory: {}.", grid_handle_dir.string()) << std::endl;
      std::filesystem::remove_all(grid_handle_dir);
    }
    std::filesystem::create_directory(grid_handle_dir);
  } catch (const std::filesystem::filesystem_error& exception) {
    std::cerr << fmt::format("Explanatory string: {}.", exception.what()) << std::endl;
    std::exit(EXIT_FAILURE);
  }
  for (const auto& file : std::filesystem::directory_iterator(file.grid_0_dir_)) {
    GridScale::solve(scale, file, grid_handle_dir / dbg(file.path().filename()));
  }
}

void sliceGrid(const OutputMode output_mode, File& file) {
  std::filesystem::path grid_handle_dir = gridHandleDir(file.grid_0_dir_);
  for (const auto& file : std::filesystem::directory_iterator(file.grid_0_dir_)) {
    GridSlice::solve(file, grid_handle_dir / dbg(file.path().filename()), output_mode);
  }
}

void makeMaskDir(File& file) {
  try {
    if (std::filesystem::exists(file.output_dir_)) {
      std::cout << fmt::format("Remove directory: {}.", file.output_dir_.string()) << std::endl;
      std::filesystem::remove_all(file.output_dir_);
    }
    std::filesystem::create_directory(file.output_dir_);
  } catch (const std::filesystem::filesystem_error& exception) {
    std::cerr << fmt::format("Explanatory string: {}.", exception.what()) << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

void calMask(const Index x_ii_num, const File& file, const ReadMode read_mode, const MaskMode mask_mode) {
  if (!std::filesystem::exists(file.mask_output_dir_)) {
    std::filesystem::create_directory(file.mask_output_dir_);
  }
  std::cout << "Begin output mask file:" << std::endl;
  for (const auto i : tq::trange(x_ii_num)) {
    std::filesystem::path x_ii_file = xiiInputFile(i, file.x_ii_dir_, read_mode);
    std::filesystem::path mask_output_file = maskOutputFile(i, file.mask_output_dir_);
    RayCasting::solve(file.grid_0_dir_, x_ii_file, mask_output_file, read_mode, mask_mode);
  }
  std::cout << std::endl;
}

void calMask(const Index x_ii_num, const int theta, const std::filesystem::path& grid_input_dir, const File& file,
             const ReadMode read_mode, const MaskMode mask_mode) {
  if (!std::filesystem::exists(file.mask_output_dir_)) {
    std::filesystem::create_directory(file.mask_output_dir_);
  }
  std::cout << fmt::format("<<<<<<<<<< Rotate theta = {} >>>>>>>>>>", theta) << std::endl;
  for (const auto i : tq::trange(x_ii_num)) {
    std::filesystem::path x_ii_file = xiiInputFile(i, file.x_ii_dir_, read_mode);
    std::filesystem::path mask_output_file = maskOutputFile(i, theta, file.mask_output_dir_);
    RayCasting::solve(grid_input_dir, x_ii_file, mask_output_file, read_mode, mask_mode);
  }
  std::cout << std::endl;
}

void testPoint(const Index x_ii_num, const File& file, const ReadMode read_mode) {
  PointTest::solve(x_ii_num, file.x_ii_dir_, file.test_input_dir_, file.test_output_file_, read_mode);
}

void testPoint(const Index x_ii_num, const int theta, const File& file, const ReadMode read_mode) {
  if (!std::filesystem::exists(file.test_output_dir_)) {
    std::filesystem::create_directory(file.test_output_dir_);
  }
  PointTest::solve(x_ii_num, theta, file.x_ii_dir_, file.test_input_dir_, file.test_output_dir_, read_mode);
}
