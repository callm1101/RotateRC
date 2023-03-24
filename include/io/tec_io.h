#ifndef ROTATE_RC_TEC_IO_H_
#define ROTATE_RC_TEC_IO_H_

#include "basic/basic.h"

namespace TecIO {

void outputPlt(Index num, GridOut& grid_out, const std::filesystem::path& test_output_file);

}  // namespace TecIO

#endif
