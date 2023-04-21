#include "io/tec_io.h"

namespace TecIO {

void outputPlt(const Index num, GridOut& grid_out, const std::filesystem::path& test_output_file) {
  int number = grid_out.out_num_->sum();
  std::ofstream fout;
  fout.exceptions(std::ofstream::failbit | std::ofstream::badbit);
  fout.setf(std::ios_base::left, std::ios_base::adjustfield);
  fout.setf(std::ios_base::scientific, std::ios_base::floatfield);
  fout.precision(15);
  try {
    fout.open(test_output_file, std::ios_base::out | std::ios_base::trunc);
    fout << R"(VARIABLES = "X", "Y", "Z")" << std::endl;
    fout << fmt::format("ZONE I={} DATAPACKING=POINT", number) << std::endl;
    for (Index i = 0; i < num; i++) {
      if (grid_out.out_num_->operator()(static_cast<Eigen::Index>(i)) > 0) {
        fout << grid_out.out_points_[i]->transpose() << std::endl;
      }
    }
    fout.close();
  } catch (const std::ofstream::failure& exception) {
    std::cerr << fmt::format("Explanatory string: {}.", exception.what()) << std::endl;
    std::cerr << fmt::format("File: {}.", test_output_file.string()) << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

}  // namespace TecIO
