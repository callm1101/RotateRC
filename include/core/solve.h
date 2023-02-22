#ifndef ROTATE_RC_SOLVE_H_
#define ROTATE_RC_SOLVE_H_

#include <dbg.h>

#include <tqdm.hpp>

#include "cmake.h"
#include "core/basic.h"
#include "grid/grid_handle.h"
#include "grid/ray_casting.h"
#include "test/point_test.h"

void scaleGrid(double scale, File& file);

void makeRotateDir(int theta_difference, File& file);

void delRotateDir(File& file);

void rotateGrid(int theta_difference, const std::filesystem::path& grid_rotate_file, const File& file,
                std::unique_ptr<Eigen::Vector3d>&& point1, std::unique_ptr<Eigen::Vector3d>&& point2);

void makeMaskDir(File& file);

void calMask(Index x_ii_num, const File& file, ReadMode read_mode, MaskMode mask_mode);

void calMask(Index x_ii_num, int theta, const std::filesystem::path& grid_dir, const File& file, ReadMode read_mode,
             MaskMode mask_mode);

void testPoint(Index x_ii_num, const File& file, ReadMode read_mode);

void testPoint(Index x_ii_num, int theta, const File& file, ReadMode read_mode);

#endif
