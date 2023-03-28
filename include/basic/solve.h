#ifndef ROTATE_RC_SOLVE_H_
#define ROTATE_RC_SOLVE_H_

#include <tqdm.hpp>

#include "basic/basic.h"
#include "basic/file.h"
#include "core/ray_casting.h"
#include "core/shape_judge.h"
#include "test/point_test.h"
#include "utils/grid_rotate.h"
#include "utils/grid_scale.h"
#include "utils/grid_slice.h"

void makeRotateDir(int theta_difference, File& file);

void delRotateDir(File& file);

void rotateGrid(int theta_difference, const std::filesystem::path& grid_rotate_file, const File& file,
                std::unique_ptr<Eigen::Vector3d>&& point1, std::unique_ptr<Eigen::Vector3d>&& point2);

void scaleGrid(double scale, File& file);

void sliceGrid(OutputMode output_mode, File& file);

void makeMaskDir(File& file);

void calMask(Index x_ii_num, const File& file, ReadMode read_mode, MaskMode mask_mode);

void calMask(Index x_ii_num, int theta, const std::filesystem::path& grid_input_dir, const File& file,
             ReadMode read_mode, MaskMode mask_mode);

void calMask(Index x_ii_num, const ShapeJudge::XZYEllipseCylinder& xzy_ellipse_cylinder, const File& file,
             ReadMode read_mode);

void testPoint(Index x_ii_num, const File& file, ReadMode read_mode);

void testPoint(Index x_ii_num, int theta, const File& file, ReadMode read_mode);

#endif
