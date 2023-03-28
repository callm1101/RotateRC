#ifndef ROTATE_RC_IO_H_
#define ROTATE_RC_IO_H_

#include "io/grid_io.h"
#include "io/mask_io.h"
#include "io/tec_io.h"
#include "io/xii_io.h"

namespace RayCasting {

void read(const std::filesystem::path& grid_input_dir, const std::filesystem::path& x_ii_file,
          std::vector<Grid>& grid_vec, Xii& x_ii, ReadMode read_mode);

}  // namespace RayCasting

namespace ShapeJudge {

void read(const std::filesystem::path& x_ii_file, Xii& x_ii, ReadMode read_mode);

}  // namespace ShapeJudge


namespace PointTest {

void read(const std::filesystem::path& x_ii_file, const std::filesystem::path& test_input_file, Xii& x_ii, Mask& mask,
          ReadMode read_mode);

}  // namespace PointTest

#endif
