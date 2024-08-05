# UE4 代码覆盖率

## 原理

GCC 其实有内置的代码覆盖率的工具的，叫 Gcov.
 
### Gcov 主要分三步：

- 在 GCC 编译的时加入特殊的编译选项，生成可执行文件，和 *.gcno；
- 运行（测试）生成的可执行文件，生成了 *.gcda 数据文件；
- 有了 *.gcno 和 *.gcda，通过源码生成 gcov 文件，最后生成代码覆盖率报告。

### 可视化报告工具

- lcov, Linux 的覆盖率报告就是用这个生成的，有1.x版本和2.0版本。这个都是用perl 写的，但是它 2.0版本只有预编译了centos的rpm包，没有ubuntu的dep包，安装非常慢。而且，这个工具只输出html 格式，不利于结构化处理。
- gcovr，使用Python写的，内部也是对 gcov 处理结果的二次处理。这个工具可以输出 html、cvs、json等丰富的格式。Python项目也更有利于按需修改。缺点是官方对于bug的修复不是很积极。

gcovr 统计四种不同的覆盖率，分别是 line, function, branch, decision。

- Line 行覆盖率
分母：编译到可执行程序里的没有被优化的代码行总数，不包括声明和定义代码行，例如函数定义，类定义代码行。
分子：实际被执行到的代码行。

- Function 函数覆盖率
分母：编译到可执行程序里没有被内联(inline) 的函数总数。
分子：被执行到的函数的总数。

- Branch 分支覆盖率
- Decision 决策覆盖率

## UE4 测量代码覆盖步骤

UE4 构建工具是一个（不容易）可扩展的跨平台构建工具，该工具默认不带代码覆盖支持，需要修改每个工具链以使其正常运行。

因为Linux生态系统为我们提供了一套易于使用的测量覆盖率的工具， 如Gcov， 所以下文描述步骤仅适用于 DS。 由于 UE4项目的特殊性，注意到如下：

- UE4 采用 clang 编译ds， 因此，编译及链接参数与 gcc 略有不同。
- DS 不能执行强制退出命令，否则无法正确生成覆盖文件。
- UE4 DS可能会采用交叉编译。如在 Windows 上编译Linux版本 DS。 运行则在Linux， 而生成的gcda文件目录则是按照windows路径来的。可以使用 GCOV_PREFIX 改变 .gcda 文件存放的根目录，使用 GCOV_PREFIX_STRIP 消除 .gcda 存放目录（从顶级目录开始消除）。

UE4的代码覆盖完整流程由四步构成：

### Step1 编译

需要修改 UBT， 这里有 基于 UE4.27 的patch文件： https://gist.github.com/galeone/f8bdf0fb4fafc517a4f65537b2ae2634

### Step2 链接

需要修改 UBT， 参考基于 UE4.27 的patch文件： https://gist.github.com/galeone/f8bdf0fb4fafc517a4f65537b2ae2634

最后通过以下命令进行编译链接。 注意 -CodeCoverage 为新增选项。

```
mono Engine/Binaries/DotNET/UnrealBuildTool.exe \ # the UBT
         TargetNameEditor \ # Our compilation target, in Editor
         Linux \ # Our target platfrom
         Development \
         -project=Project.uproject \ # The unreal project we are building
         -CodeCoverage # The custom flag we are going to add
```

### Step3 运行

启动 UE DS， 运行测试case。

可参考：
```
Engine/Binaries/Linux/UE4Editor Project.uproject \
        -ExecCmds="automation RunTests Now MODULE+TO+TEST+PLUS+SEPARATED; quit" \
        -buildmachine -forcelogflush -unattended -nopause -nosplash -log -nullrhi -stdout -FullStdOutLogOutput
```
注意，quit 不能强制退出。

### Step4 输出代码覆盖报告

由于 clang 生成的 cov 文件无法直接被 gcov 工具识别。 需要使用 clang 自动的工具 llvm-cov 来读取 cov 文件生成报告。
但 UE4 依赖的 clang 工具版本比较低， 例如： UE4.23 的 clang 工具版本为：8.0.1 ， 因此，这里需要下载 llvm 工具链源码自行编译

前置依赖：

- gcc >=5.1
- cmake > 3.4.3

需要先下载安装 cmake （>3.4.3）:
```
wget https://github.com/Kitware/CMake/releases/download/v3.30.2/cmake-3.30.2-linux-x86_64.sh
chmod +x cmake-3.30.2-linux-x86_64.sh
./cmake-3.30.2-linux-x86_64.sh
```

```
wget https://github.com/llvm/llvm-project/archive/refs/tags/llvmorg-11.0.1.tar.gz
tar xf llvmorg-11.0.1.tar.gz
cd llvm-project-*
mkdir build
cd build
cmake -DLLVM_ENABLE_PROJECTS=clang-tools-extra -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" ../llvm
make -j$(nproc)
```

由于不能直接调用 lcov 工具生成报告， 这里需要先包装一个 脚本来作为参数传递给 lcov:

```
# Create llvm-gcov.sh with the following content
#!/bin/bash
exec llvm-cov gcov "$@"

# Make it executable
chmod +x llvm-gcov.sh
```

接着调用 lcov(注意调用前，先将 llvm-gcov.sh 拷贝到 PATH 路径下):

```
# $engine contains the path of the engine

lcov -q --gcov-tool llvm-gcov.sh --directory . --capture --no-external --base-directory $engine/Engine/Source/ -o cov.info
lcov --remove cov.info '/usr/*' \
     --remove cov.info "$engine"'/Engine/Source/*' \
     --remove cov.info $(pwd)'/Plugins/*' \
     --remove cov.info $(pwd)'/Intermediate/*' \
     --remove cov.info $(pwd)'/Source/*/ThirdParty/*' \
     --output-file cov.info

# Now cov.info has all the information aboout the prevous execution, without any reference to:
# - engine files
# - system files
# - Plugins
# - generated files
# - ThirdParty libraries included in some of our modules
```

生成可视化的网页报告：

```
genhtml cov.info -o coverage
```


## 参考

- [Code Coverage of Unreal Engine projects](https://pgaleone.eu/2022/02/23/code-coverage-with-unreal-engine/)
- [Code Coverage of Unreal Engine projects - patch gist](https://gist.github.com/galeone/f8bdf0fb4fafc517a4f65537b2ae2634)
- [llvm-cov - emit coverage information](https://llvm.org/docs/CommandGuide/llvm-cov.html)
- [C/C++测试覆盖率分析工具的分层 ](https://www.cnblogs.com/math/p/tool-chains-layer.html)
- [使用 Gcov 和 LCOV 度量 C/C++ 项目的代码覆盖率](https://cloud.tencent.com/developer/article/1868149)
- [gcov-example](https://github.com/shenxianpeng/gcov-example)
- [Data File Relocation to Support Cross-Profiling](https://gcc.gnu.org/onlinedocs/gcc/Cross-profiling.html)
