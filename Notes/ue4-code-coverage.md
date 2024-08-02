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

## 参考

- [Code Coverage of Unreal Engine projects](https://pgaleone.eu/2022/02/23/code-coverage-with-unreal-engine/)
- [Code Coverage of Unreal Engine projects - patch gist](https://gist.github.com/galeone/f8bdf0fb4fafc517a4f65537b2ae2634)
- [llvm-cov - emit coverage information](https://llvm.org/docs/CommandGuide/llvm-cov.html)
- [C/C++测试覆盖率分析工具的分层 ](https://www.cnblogs.com/math/p/tool-chains-layer.html)
- [使用 Gcov 和 LCOV 度量 C/C++ 项目的代码覆盖率](https://cloud.tencent.com/developer/article/1868149)
- [gcov-example](https://github.com/shenxianpeng/gcov-example)
