#include "file/file_copy.h"

namespace FileCopy {

void copyFile(const std::filesystem::path& src_file, const std::filesystem::path& target_file, const bool output) {
  try {
    std::filesystem::copy_file(src_file, target_file);
  } catch (const std::filesystem::filesystem_error& exception) {
    std::cerr << fmt::format("Explanatory string: {}.", exception.what()) << std::endl;
    std::exit(EXIT_FAILURE);
  }
  if (output) {
    std::cout << fmt::format("Copy file {} to {}.", src_file.string(), target_file.string());
  }
}

void copyGridFile(const std::vector<std::filesystem::path>& copy_file_vec,
                  const std::vector<std::filesystem::path>& dir_vec) {
  for (const auto& copy_file : copy_file_vec) {
    for (const auto& dir : dir_vec) {
      std::filesystem::path target_file = dir / copy_file.filename();
      copyFile(copy_file, target_file, false);
    }
    std::cout << fmt::format("Copy file {} to {} ... {}.", copy_file.string(), dir_vec[0].string(),
                             dir_vec[dir_vec.size()].string());
  }
}

}  // namespace FileCopy
