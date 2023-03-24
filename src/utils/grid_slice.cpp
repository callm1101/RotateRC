#include "utils/grid_slice.h"

namespace GridSlice {

void solve(const std::filesystem::path& grid_input_file, const std::filesystem::path& grid_output_file,
           const OutputMode output_mode) {
  Grid grid;
  GridIO::readGrid(grid_input_file, grid);
  GridIO::outputGrid(grid_output_file, grid, output_mode);
}

}  // namespace GridSlice
