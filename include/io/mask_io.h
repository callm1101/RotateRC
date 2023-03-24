#ifndef ROTATE_RC_MASK_IO_H_
#define ROTATE_RC_MASK_IO_H_

#include "basic/basic.h"

namespace MaskIO {

void readMask(Mask& mask, const std::filesystem::path& x_ii_file, const std::filesystem::path& test_input_file,
              ReadMode read_mode);

void outputMask(Mask& mask, const std::filesystem::path& mask_output_file);

}  // namespace MaskIO

#endif
