#ifndef ROTATE_RC_GRID_IO_H_
#define ROTATE_RC_GRID_IO_H_

#include "basic/basic.h"

namespace GridIO {

void readGrid(const std::filesystem::path& grid_input_file, Grid& grid);

void readGrid(const std::filesystem::path& grid_input_dir, std::vector<Grid>& grid_vec);

void outputGrid(const std::filesystem::path& grid_output_file, Grid& grid, OutputMode output_mode);

void outputGrid(const std::filesystem::path& grid_output_file, Grid& grid, GridHandle& grid_handle);

}  // namespace GridIO

#endif
