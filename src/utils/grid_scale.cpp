#include "utils/grid_scale.h"

namespace GridScale {

void solve(const double scale, const std::filesystem::path& grid_input_file,
           const std::filesystem::path& grid_output_file) {
  Grid grid;
  GridHandle grid_handle;
  GridIO::readGrid(grid_input_file, grid);
  scaleGrid(scale, grid, grid_handle);
  GridIO::outputGrid(grid_output_file, grid, grid_handle);
}

void scaleGrid(const double scale, Grid& grid, GridHandle& grid_handle) {
  grid_handle.number_ = grid.number_[0];
  grid_handle.point_ = std::make_unique<Eigen::Matrix3Xd>(3, grid_handle.number_);
  (*grid_handle.point_) = (*grid.point_) * scale;
}

}  // namespace GridScale
