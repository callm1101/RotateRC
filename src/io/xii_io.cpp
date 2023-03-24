#include "io/xii_io.h"

namespace XiiIO {

void readXii(const std::filesystem::path& x_ii_file, Xii& x_ii, const ReadMode read_mode) {
  std::ifstream fin;
  fin.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    switch (read_mode) {
    case ReadMode::kText:
      fin.open(x_ii_file, std::ios_base::in);
      fin >> x_ii.number_;
      x_ii.point_ = std::make_unique<Eigen::Matrix3Xd>(3, x_ii.number_);
      for (Eigen::Index i = 0; i < static_cast<Eigen::Index>(x_ii.number_); i++) {
        fin >> x_ii.point_->operator()(0, i) >> x_ii.point_->operator()(1, i) >> x_ii.point_->operator()(2, i);
      }
      break;
    case ReadMode::kBinary:
      fin.open(x_ii_file, std::ios_base::in | std::ios_base::binary);
      fin.read(reinterpret_cast<char*>(&x_ii.number_), static_cast<std::streamsize>(sizeof(int)));
      x_ii.point_ = std::make_unique<Eigen::Matrix3Xd>(3, x_ii.number_);
      fin.read(reinterpret_cast<char*>(x_ii.point_->data()),
               x_ii.point_->size() * static_cast<std::streamsize>(sizeof(Eigen::Matrix3Xd::Scalar)));
      break;
    }
    fin.close();
  } catch (const std::ifstream::failure& exception) {
    std::cerr << fmt::format("Explanatory string: {}.", exception.what()) << std::endl;
    std::cerr << fmt::format("File: {}.", x_ii_file.string()) << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

}  // namespace XiiIO
