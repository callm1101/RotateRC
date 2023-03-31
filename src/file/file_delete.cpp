#include "file/file_delete.h"

namespace FileDelete {

void deleteDir(const std::filesystem::path& dir, const bool output) {
  try {
    std::filesystem::remove_all(dir);
    if (output) {
      std::cout << fmt::format("Remove directory: {}.", dir.string());
    }
  } catch (const std::filesystem::filesystem_error& exception) {
    std::cerr << fmt::format("Explanatory string: {}.", exception.what()) << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

void deleteGridDir(const std::filesystem::path& grid_0_dir) {
  for (const auto& dir : std::filesystem::directory_iterator(grid_0_dir.parent_path())) {
    if (dir.path() != grid_0_dir) {
      deleteDir(dir, false);
    }
  }
  std::cout << fmt::format("Remove all directory in {} except {}.", grid_0_dir.parent_path().string(),
                           grid_0_dir.filename().string());
}

void deleteDirVec(const std::vector<std::filesystem::path>& dir_vec) {
  for (const auto& dir : dir_vec) {
    deleteDir(dir, false);
  }
  std::cout << fmt::format("Remove all directory: {} ... {}.", dir_vec[0].string(), dir_vec[dir_vec.size()].string());
}

}  // namespace FileDelete
