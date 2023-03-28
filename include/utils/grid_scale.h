#ifndef ROTATE_RC_GRID_SCALE_H_
#define ROTATE_RC_GRID_SCALE_H_

#include "basic/basic.h"
#include "basic/io.h"

namespace GridScale {

void solve(double scale, const std::filesystem::path& grid_input_file, const std::filesystem::path& grid_output_file);

void scaleGrid(double scale, Grid& grid, GridHandle& grid_handle);

}  // namespace GridScale

#endif
