#include "basic/io.h"

namespace RayCasting {

void read(const std::filesystem::path& grid_input_dir, const std::filesystem::path& x_ii_file,
          std::vector<Grid>& grid_vec, Xii& x_ii, ReadMode read_mode) {
  GridIO::readGrid(grid_input_dir, grid_vec);
  XiiIO::readXii(x_ii_file, x_ii, read_mode);
}

}  // namespace RayCasting

namespace ShapeJudge {

void read(const std::filesystem::path& x_ii_file, Xii& x_ii, ReadMode read_mode) {
  XiiIO::readXii(x_ii_file, x_ii, read_mode);
}

}  // namespace ShapeJudge

namespace PointTest {

void read(const std::filesystem::path& x_ii_file, const std::filesystem::path& test_input_file, Xii& x_ii, Mask& mask,
          ReadMode read_mode) {
  XiiIO::readXii(x_ii_file, x_ii, read_mode);
  MaskIO::readMask(mask, x_ii_file, test_input_file, read_mode);
}

}  // namespace PointTest
