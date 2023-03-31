#ifndef ROTATE_RC_FILE_DIR_H_
#define ROTATE_RC_FILE_DIR_H_

#include "basic/basic.h"
#include "file/file_name.h"

namespace FileDir {

void makeDir(const std::filesystem::path& dir, bool output = true);

void makeThetaDir(int theta_difference, const std::filesystem::path& grid_0_dir,
                  std::vector<std::filesystem::path>& output_dir_vec);

}  // namespace FileDir

#endif
