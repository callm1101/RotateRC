#include "file/file_dir.h"

namespace FileDir {

void makeDir(const std::filesystem::path& dir, const bool output) {
  try {
    if (std::filesystem::exists(dir)) {
      std::filesystem::remove_all(dir);
      if (output) {
        std::cout << fmt::format("Remove exist directory: {}.", dir.string()) << std::endl;
      }
    }
    std::filesystem::create_directory(dir);
    if (output) {
      std::cout << fmt::format("Create directory: {}.", dir.string()) << std::endl;
    }
  } catch (const std::filesystem::filesystem_error& exception) {
    std::cerr << fmt::format("Explanatory string: {}.", exception.what()) << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

void makeThetaDir(const int theta_difference, const std::filesystem::path& grid_0_dir,
                  std::vector<std::filesystem::path>& output_dir_vec) {
  const auto times = static_cast<Index>(360 / theta_difference);
  for (Index i = 1; i < times; i++) {
    std::filesystem::path grid_rotate_dir = FileName::gridRotateDir(theta_difference * static_cast<int>(i), grid_0_dir);
    makeDir(grid_rotate_dir, false);
    output_dir_vec.emplace_back(grid_rotate_dir);
  }
  std::cout << fmt::format("Create all directory: {} ... {}.", output_dir_vec[1].string(),
                           output_dir_vec[output_dir_vec.size()].string());
}

}  // namespace FileDir
