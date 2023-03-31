#ifndef ROTATE_RC_FILE_NAME_H_
#define ROTATE_RC_FILE_NAME_H_

#include "basic/basic.h"

namespace FileName {

std::filesystem::path gridRotateDir(int theta, const std::filesystem::path& grid_0_dir);

std::filesystem::path xiiInputFile(Index i, const std::filesystem::path& x_ii_dir, ReadMode read_mode);

std::filesystem::path maskOutputFile(Index i, const std::filesystem::path& mask_output_dir);

std::filesystem::path maskOutputFile(Index i, int theta, const std::filesystem::path& mask_output_dir);

std::filesystem::path testOutputFile(int theta, const std::filesystem::path& test_output_dir);

}  // namespace FileName

#endif
