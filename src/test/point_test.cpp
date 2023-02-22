#include "test/point_test.h"

namespace PointTest {

void solve(const Index x_ii_num, const std::filesystem::path& x_ii_dir, const std::filesystem::path& test_input_dir,
           const std::filesystem::path& test_output_file, ReadMode read_mode) {
  Mask mask;
  mask.out_num_ = std::make_shared<Eigen::VectorXi>(x_ii_num);
  mask.out_num_->setZero();
  std::cout << "Begin test.";
  for (Index i = 0; i < x_ii_num; i++) {
    Xii x_ii;
    std::filesystem::path test_input_file = testInputFile(i, test_input_dir);
    std::filesystem::path x_ii_file = xiiFile(i, x_ii_dir, read_mode);
    readXii(x_ii_file, x_ii, read_mode);
    readResult(test_input_file, x_ii);
    calPoints(i, mask, x_ii);
  }
  std::cout << fmt::format("Output file: {}", test_output_file.filename().string()) << std::endl;
  outputPlt(x_ii_num, mask, test_output_file);
}

void solve(const Index x_ii_num, const int theta, const std::filesystem::path& x_ii_dir,
           const std::filesystem::path& test_input_dir, const std::filesystem::path& test_output_dir, ReadMode read_mode) {
  Mask mask;
  mask.out_num_ = std::make_shared<Eigen::VectorXi>(x_ii_num);
  mask.out_num_->setZero();
  std::cout << "Begin test.";
  for (Index i = 0; i < x_ii_num; i++) {
    Xii x_ii;
    std::filesystem::path test_input_file = testInputFile(i, theta, test_input_dir);
    std::filesystem::path x_ii_file = xiiFile(i, x_ii_dir, read_mode);
    readXii(x_ii_file, x_ii, read_mode);
    readResult(test_input_file, x_ii);
    calPoints(i, mask, x_ii);
  }
  std::filesystem::path test_output_file = testOutputFile(theta, test_output_dir);
  std::cout << fmt::format("Output file: {}", test_output_file.filename().string()) << std::endl;
  outputPlt(x_ii_num, mask, test_output_file);
}

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

void readResult(const std::filesystem::path& test_input_file, Xii& x_ii) {
  std::ifstream fin;
  fin.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    fin.open(test_input_file, std::ios_base::in | std::ios_base::binary);
    x_ii.result_ = std::make_unique<Eigen::VectorXi>(x_ii.number_);
    fin.read(reinterpret_cast<char*>(x_ii.result_->data()),
             x_ii.result_->size() * static_cast<std::streamsize>(sizeof(Eigen::VectorXi::Scalar)));
    fin.close();
  } catch (const std::ifstream::failure& exception) {
    std::cerr << fmt::format("Explanatory string: {}.", exception.what()) << std::endl;
    std::cerr << fmt::format("File: {}.", test_input_file.string()) << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

void calPoints(const Index i, Mask& mask, Xii& x_ii) {
  int num = 0;
  for (Eigen::Index i = 0; i < static_cast<Eigen::Index>(x_ii.number_); i++) {
    if (x_ii.result_->operator()(i) > 0) {
      num++;
    }
  }
  mask.out_num_->operator()(static_cast<int>(i)) = num;
  std::unique_ptr<Eigen::Matrix3Xd> out_points = std::make_unique<Eigen::Matrix3Xd>(3, num);
  for (Eigen::Index i = 0, j = 0; j < static_cast<Eigen::Index>(x_ii.number_); j++) {
    if (x_ii.result_->operator()(j) > 0) {
      out_points->col(i++) << x_ii.point_->col(j);
    }
  }
  mask.out_points_.emplace_back(std::move(out_points));
}

void outputPlt(const Index num, Mask& mask, const std::filesystem::path& test_output_file) {
  int number = mask.out_num_->sum();
  std::ofstream fout;
  fout.exceptions(std::ofstream::failbit | std::ofstream::badbit);
  fout.setf(std::ios_base::left, std::ios_base::adjustfield);
  fout.setf(std::ios_base::scientific, std::ios_base::floatfield);
  fout.precision(15);
  try {
    fout.open(test_output_file, std::ios_base::out | std::ios_base::trunc);
    fout << R"(VARIABLES = "X","Y","Z")" << std::endl;
    fout << fmt::format("ZONE I={} DATAPACKING=POINT", number) << std::endl;
    for (Index i = 0; i < num; i++) {
      if (mask.out_num_->operator()(static_cast<Eigen::Index>(i)) > 0) {
        fout << mask.out_points_[i]->transpose() << std::endl;
      }
    }
    fout.close();
  } catch (const std::ofstream::failure& exception) {
    std::cerr << fmt::format("Explanatory string: {}.", exception.what()) << std::endl;
    std::cerr << fmt::format("File: {}.", test_output_file.string()) << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

}  // namespace PointTest
