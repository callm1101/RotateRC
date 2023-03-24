#ifndef ROTATE_RC_RAY_CASTING_H_
#define ROTATE_RC_RAY_CASTING_H_

#include <omp.h>

#include "basic/basic.h"
#include "basic/file.h"
#include "basic/io.h"

namespace RayCasting {

enum class TriFlag : Index { kOnTri, kCrossTri, kOutTri, kOnCorner };

struct Box {
  std::unique_ptr<Eigen::Matrix<double, 6, Eigen::Dynamic>> axis_box_;
  std::unique_ptr<Eigen::Vector<double, 6>> object_box_;
};

void solve(const std::filesystem::path& grid_input_dir, const std::filesystem::path& x_ii_file,
           const std::filesystem::path& mask_output_file, ReadMode read_mode, MaskMode mask_mode);
void calAABB(std::vector<Grid>& grid_vec, std::vector<Box>& box_vec);
TriFlag intersectTri(const Eigen::Vector3d& point, Eigen::Matrix3d& tri_coord);
bool castRay(const Eigen::Vector3d& point, Grid& grid, Box& box);
bool judgeSingularity(Index num, std::vector<Eigen::Index>& tri_index, const Eigen::Vector3d& point, Grid& grid,
                      Box& box);
bool detectIntersect(Eigen::Index tri1, Eigen::Index tri2, Grid& grid);

}  // namespace RayCasting

#endif
