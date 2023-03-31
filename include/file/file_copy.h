#ifndef ROTATE_RC_FILE_COPY_H_
#define ROTATE_RC_FILE_COPY_H_

#include "basic/basic.h"

namespace FileCopy {

void copyFile(const std::filesystem::path& src_file, const std::filesystem::path& target_file, bool output = true);

void copyGridFile(const std::vector<std::filesystem::path>& copy_file_vec,
                   const std::vector<std::filesystem::path>& dir_vec);

}  // namespace FileCopy

#endif
