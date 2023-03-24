#include "io/mask_io.h"

namespace MaskIO {

void readMask(Mask& mask, const std::filesystem::path& x_ii_file, const std::filesystem::path& test_input_file,
              const ReadMode read_mode) {
  std::ifstream fin;
  fin.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    switch (read_mode) {
    case ReadMode::kText:
      fin.open(x_ii_file, std::ios_base::in);
      fin >> mask.number_;
      break;
    case ReadMode::kBinary:
      fin.open(x_ii_file, std::ios_base::in | std::ios_base::binary);
      fin.read(reinterpret_cast<char*>(&mask.number_), static_cast<std::streamsize>(sizeof(int)));
      break;
    }
    fin.close();
    fin.open(test_input_file, std::ios_base::in | std::ios_base::binary);
    mask.result_ = std::make_unique<Eigen::VectorXi>(mask.number_);
    fin.read(reinterpret_cast<char*>(mask.result_->data()),
             mask.result_->size() * static_cast<std::streamsize>(sizeof(Eigen::VectorXi::Scalar)));
    fin.close();
  } catch (const std::ifstream::failure& exception) {
    std::cerr << fmt::format("Explanatory string: {}.", exception.what()) << std::endl;
    std::cerr << fmt::format("File: {}.", x_ii_file.string()) << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

void outputMask(Mask& mask, const std::filesystem::path& mask_output_file) {
  std::ofstream fout;
  fout.exceptions(std::ofstream::failbit | std::ofstream::badbit);
  try {
    fout.open(mask_output_file, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
    fout.write(reinterpret_cast<const char*>(mask.result_->data()),
               mask.result_->size() * static_cast<std::streamsize>(sizeof(Eigen::VectorXi::Scalar)));
    fout.close();
  } catch (const std::ofstream::failure& exception) {
    std::cerr << fmt::format("Explanatory string: {}.", exception.what()) << std::endl;
    std::cerr << fmt::format("File: {}.", mask_output_file.string()) << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

}  // namespace MaskIO
