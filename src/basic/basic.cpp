#include "basic/basic.h"

#include "basic/file.h"

File::File() {
  FileDir::makeDir(this->output_dir_);
  FileDir::makeDir(this->mask_output_dir_);
}

HandleFile::HandleFile() { FileDir::makeDir(this->grid_handle_dir_); }

RotateFile::RotateFile() {
  FileDir::makeDir(this->test_output_dir_);
}
