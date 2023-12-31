# SM3优化

## 实验环境

+ Python 3.11.4

+ Apple clang version 14.0.3 (clang-1403.0.22.14.1)

+ Apple M2 24G

## 实验内容

本项目旨在进行SM3哈希算法的优化工作，以提高其在软件实现中的性能和效率。通过使用适当的数据结构、算法优化、编译器选项以及平台特定的优化策略，我们成功地改进了SM3哈希算法的性能。

在实现SM3时我们采用了如下策略进行优化

1. 使用位运算：我们利用位运算替代常规运算，并选择适当的数据结构，如向量以加速数据处理和减少内存占用。
2. 循环展开：我们对主要循环进行展开，以减少循环开销，提高并行性，并减少指令分支
3. 编译器优化选项：这里更简单的我们不需要进行实际的代码修改，通过gcc的编译优化选项 `-o1,-o2,-o3` 可以完成诸如unrolling等一系列优化操作

4. 这里我试图加入多线程对哈希过程进行并行，但是我发现大多数过程都具有强依赖性，局部的可操作空间进行并行优化并不能带来太多的提升
5. SIMD指令优化：通过使用SIMD指令，我们实现了对向量数据的并行处理，从而显著加速了哈希计算。这个确实是有效的，相比原始SM3实现带来了近乎四倍的提升效果，但是架构上一直没处理好，arm架构上比较麻烦