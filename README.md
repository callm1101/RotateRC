### 射线相交法程序

1. 网格数据存在 `dat/grid/grid_0` 中, `dat/lxj` 中存的是螺旋桨的网格, `dat/car` 中存的是汽车网格,需要时可以将这两套网格用于替换 `dat/grid/grid_0` 中的网格数据,对于螺旋桨需要旋转的情况,代码会生成 `dat/grid/grid_1, dat/grid/grid_2, dat/grid/grid_3 ...` 等文件夹来存放每个角度的网格文件,并且会在运行完毕后删除.

2. 老板给的 `X_ii` 文件解压后存放在 `dat/X_ii` 中,注意解压时不要多建立文件夹,直接将 `X_ii.zip` 放在 `dat` 文件夹下接着执行 `unzip X_ii.zip` 即可.

3. 代码生成的结果文件在 `build/out` 中,对于螺旋桨的情况,结果文件有两个文件夹分别为 `build/out/mask` 和 `build/out/test` ,先 `cd` 到 `build/out/mask` 文件夹内,接着执行 `zip -q -r ../mask.zip ./` 会在上层文件夹 `build/out` 内生成一个 `mask.zip` 文件,这个文件直接发给老板, `build/out/test` 文件夹中存放的程序输出的散点图,可以用 `tecplot` 打开,文件命名的格式为 `test_{theta}` ,即每一个旋转角度生成一个 `test` 文件.对于汽车的情况,结果文件为 `build/out/mask` 文件夹以及一个 `build/out/test.plt` 文件,操作同上.

4. 项目使用 `cmake` 和 `vcpkg` 构建,这里使用 `cmake-presets` 来构建
```shell
cd build
cmake -S .. --preset=release
ninja
cd bin
./rotate_rc
```

5. 通过调整 `src/rotate_rc.cpp` 文件中第 8 和 9 行
```cpp
constexpr ReadMode kReadMode = ReadMode::kText;
constexpr ReadMode kReadMode = ReadMode::kBinary;
```
可以修改读入的 `X_ii` 文件格式, `ReadMode::kText` 对应文本文件, `ReadMode::kBinary` 对应二进制文件.

5. 通过调整 `src/rotate_rc.cpp` 文件中第 11 和 12 行
```cpp
constexpr OutputMode kOutputMode = OutputMode::kText;
constexpr OutputMode kOutputMode = OutputMode::kBinary;
```
可以修改输出的网格文件格式, `OutputMode::kText` 对应文本文件, `OutputMode::kBinary` 对应二进制文件,注意这个选项只控制 `sliceGrid(kOutputMode, file)` 函数中的输出.

6. 通过调整 `src/rotate_rc.cpp` 文件中第 14 和 15 行
```cpp
rayCasting(kReadMode, MaskMode::kBool, file);
rotateRC(kReadMode, MaskMode::kIndex, file);
```
可以控制程序是计算螺旋桨(旋转)还是汽车(不旋转)的射线相交.

7. 程序第 17 行
```cpp
scaleGrid(1e-3, file);
```
表示将 `grid_0` 文件夹中的网格文件按照第一个参数的尺度进行缩放,输出文件在 `grid_handle` 文件夹中.

8. 程序第 18 行
```cpp
sliceGrid(kOutputMode, file);
```
表示将 `grid_0` 文件夹中网格文件的 Index 信息剔除,上面说了这里可以调整输出为文本文件或者二进制文件,输出文件同样在 `grid_handle` 文件夹中.

9. 对于 mask 输出,这里可以通过 `MaskMode::kBool` 和 `MaskMode::kIndex` 来调整,前者代表无论对于多少套网格,均输出网格外为 0 ,网格内为 1 ,后者则会根据网格的命名排序来输出 1, 2, 3 ... .

10. `src/rotate_rc.cpp` 文件中的第 32-24 行
```cpp
file.grid_copy_file_vec_.emplace_back(file.root_ / "dat/grid/grid_0/body.dat");
file.grid_rotate_file_vec_.emplace_back(file.root_ / "dat/grid/grid_0/lxj1.dat");
file.grid_rotate_file_vec_.emplace_back(file.root_ / "dat/grid/grid_0/lxj2.dat");
```
分别表示了 `body.dat` 是不需要旋转的,而 `lxj1.dat` 和 `lxj2.dat` 是需要旋转的.

11. 通过调整 `src/rotate_rc.cpp` 文件中第 15 行函数中第一个参数
```cpp
rotateRC(1, kReadMode, MaskMode::kIndex, file);
```
可以调整旋转的角度间隔,这里设置为 1 度.

12. `src/rotate_rc.cpp` 文件中的第 39-43 行
```cpp
{
std::unique_ptr<Eigen::Vector3d> point1 = std::make_unique<Eigen::Vector3d>(
    std::initializer_list<std::initializer_list<double>>{{2.6, -3.5475194, 0.97825646}});
std::unique_ptr<Eigen::Vector3d> point2 = std::make_unique<Eigen::Vector3d>(
    std::initializer_list<std::initializer_list<double>>{{2.6, -7.3689222, 0.64392703}});
rotateGrid(kThetaDifference, file.grid_rotate_file_vec_[0], file, std::move(point1), std::move(point2));
}
```
表示了对于 `lxj1.dat` 文件的旋转,其中用两个旋转点来确定旋转轴,旋转的方向可以由点 1 指向点 2 的右手定则来确定.
