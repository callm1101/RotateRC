#ifndef ROTATE_RC_GRID_SLICE_H_
#define ROTATE_RC_GRID_SLICE_H_

#include "basic/basic.h"
#include "basic/io.h"

namespace GridSlice {

void solve(const std::filesystem::path& grid_input_file, const std::filesystem::path& grid_output_file, OutputMode output_mode);

}  // namespace GridSlice

#endif
