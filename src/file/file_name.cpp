#include "file/file_name.h"

namespace FileName {

std::filesystem::path gridRotateDir(const int theta, const std::filesystem::path& grid_0_dir) {
  std::string dir = fmt::format("grid_{}", theta);
  std::filesystem::path grid_dir = grid_0_dir.parent_path() / dir;
  return grid_dir;
}

std::filesystem::path xiiInputFile(const Index i, const std::filesystem::path& x_ii_dir, const ReadMode read_mode) {
  std::string file;
  switch (read_mode) {
  case ReadMode::kBinary:
    file = fmt::format("X_ii_{}.binary", i);
    break;
  case ReadMode::kText:
    file = fmt::format("X_ii_{}.txt", i);
    break;
  }
  std::filesystem::path x_ii_file = x_ii_dir / file;
  return x_ii_file;
}

std::filesystem::path maskOutputFile(const Index i, const int theta, const std::filesystem::path& mask_output_dir) {
  std::string file = fmt::format("mask_{}_iter_{}.binary", i, theta);
  std::filesystem::path mask_output_file = mask_output_dir / file;
  return mask_output_file;
}

std::filesystem::path maskOutputFile(const Index i, const std::filesystem::path& mask_output_dir) {
  std::string file = fmt::format("mask_{}.binary", i);
  std::filesystem::path mask_output_file = mask_output_dir / file;
  return mask_output_file;
}

std::filesystem::path testOutputFile(const int theta, const std::filesystem::path& test_output_dir) {
  std::string file = fmt::format("test_{}.plt", theta);
  std::filesystem::path test_output_file = test_output_dir / file;
  return test_output_file;
}

}  // namespace FileName
