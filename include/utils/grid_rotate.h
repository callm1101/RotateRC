#ifndef ROTATE_RC_GRID_ROTATE_H_
#define ROTATE_RC_GRID_ROTATE_H_

#include "basic/basic.h"
#include "basic/io.h"

namespace GridRotate {

struct Rotation {
  int theta_;
  std::unique_ptr<Eigen::Vector3d> point_;
  std::unique_ptr<Eigen::Vector3d> vec_;
};

void solve(const std::filesystem::path& grid_input_file, const std::filesystem::path& grid_output_file,
           const Rotation& rotation);
void rotateGrid(const Rotation& rotation, Grid& grid, GridHandle& grid_handle);

}  // namespace GridRotate

#endif
