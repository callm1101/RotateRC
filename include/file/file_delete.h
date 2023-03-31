#ifndef ROTATE_RC_FILE_DELETE_H_
#define ROTATE_RC_FILE_DELETE_H_

#include "basic/basic.h"

namespace FileDelete {

void deleteDir(const std::filesystem::path& dir, bool output = true);

void deleteGridDir(const std::filesystem::path& grid_0_dir);

void deleteDirVec(const std::vector<std::filesystem::path>& dir_vec);

}  // namespace FileDelete

#endif
