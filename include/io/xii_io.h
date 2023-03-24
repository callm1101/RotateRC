#ifndef ROTATE_RC_XII_IO_H_
#define ROTATE_RC_XII_IO_H_

#include "basic/basic.h"

namespace XiiIO {

void readXii(const std::filesystem::path& x_ii_file, Xii& x_ii, ReadMode read_mode);

}  // namespace XiiIO

#endif
