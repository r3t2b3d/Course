## 介绍

本仓库为创新创业实践课程作业

小组构成：欧阳仁鼎(202100460133)

## 项目完成情况

1. 精简 SM3 生日攻击

2. 精简 SM3 rho 算法攻击
3. 针对 SM3、SHA256 的哈希长度拓展攻击
4. SM3 优化
5. RFC6962标准 中 Merkle Tree 实现
6. 在网络交互中实现带哈希函数的范围证明
7. 实现推广哈希链体系   &#10008; 
8. 利用 ARM 指令实现 AES   &#10008; 
9. AES/SM4 软件实现
10. ECDSA推断技术在以太坊中应用的报告
11. 按照 RFC6979 标准实现 SM2
12. ECDSA，Schnorr，SM2-sig的相关攻击PoC
13. 实现上述ECMH方案
14. SM2 实现 PGP 方案
15. 实现 SM2 2P 签名方案的网络交互
16. 实现 SM2 2P 解密方案的网络交互
17. 比较Firefox和谷歌的记住密码插件的实现区别
18. 在比特币测试网发送一笔交易，并解析数据到每一比特  &#10008; 
19. 伪造签名来假装成 Satoshi
20. ECMH PoC
21. Schnorr 多签方案
22. MPT研究报告

上述项目中 &#10008; 代表未完成项目

额外的，

在project21的基础上拓展了 `Schnorr哈希算法误用实例分析`，项目为 `Project21+`

在project12的基础上拓展了 `ECDSA的随机数生成器误用实例分析`，项目为 `Project12+`

## 项目详情

### Project1

项目名：精简 SM3 生日攻击			运行时间：40bits/330 s

+ 利用Python实现了对SM3的生日攻击

### Project2

项目名：精简 SM3 rho 算法攻击	运行时间：40bits/294 s

+ 利用Python实现了对SM3对rho算法攻击

### Project3

项目名：针对 SM3、SHA256 的哈希长度拓展攻击

+ 对标准gmssl库进行修改，分析哈希的分块逻辑，构造合适的Padding进行长度扩展

技巧：由于大部分MD结构的哈希算法具有相同的Padding方式和结构，为了更便于拓展，我们可以统一一个Patch函数，只需将原算法的流程截取中间部分进行替代即可

### Project4

项目名：SM3 优化		运行时间：优化前测试用例0.092ms/优化后测试用例0.023ms

+ 采用unrolling和simd以及gcc自带的o2程度优化，有几倍的提速

### Project5

项目名：RFC6962标准 中 Merkle Tree 实现		运行时间 0.00380s

+ 对照标准文档进行简化后的实现

### Project6

项目名：在网络交互中实现带哈希函数的范围证明

+ 利用socket进行网络交互模拟

### Project9

项目名：AES/SM4 软件实现	运行时间：AES/0.0010 s ，SM4/ 0.118 ms

+ AES采用Python进行实现有较好的数据结构，SM4采用C进行实现

### Project10

项目名：ECDSA推断技术在以太坊中应用的报告	运行时间：0.06050 s

+ 利用数学关系式压缩上链体积，几乎是减半的压缩量

### Project11

项目名：按照 RFC6979 标准实现 SM2		运行时间：0.00396 s

+ 查阅标准文档进行实现

### Project12

项目名：ECDSA，Schnorr，SM2-sig的相关攻击PoC

+ 三种方案对所列出的攻击是类似的，利用开源数学软件实现了所有的PoC

创新：在项目中对曲线的选取进行了分析，实现了 Smart Attack 以及 MOV Attack，并且对这三种算法的随机数生成器进行分析，以ECDSA为例，结合近期论文进行算法实现，对具有脆弱性的代数随机数生成器进行了攻击

### Project13

项目名：实现上述ECMH方案		运行时间：0.00829 s

+ 参照ppt及所查资料对ECMH进行实现

### Project14

项目名：SM2 实现 PGP 方案		运行时间：0.10992 s

+ 参考ppt对PGP方案进行了实现

### Project15

项目名：实现 SM2 2P 签名方案的网络交互

+ 利用socket进行网络模拟，并实现了上述交互过程

### Project16

项目名：实现 SM2 2P 解密方案的网络交互

+ 利用socket进行网络模拟，并实现了上述交互过程

### Project17

项目名：比较Firefox和谷歌的记住密码插件的实现区别

+ 参照了一部分Chrome的开源代码，并结合了官方文档及wiki上的信息进行整理

### Project19

项目名：伪造签名来假装成 Satoshi		运行时间：0.01283 s

+ 参照ppt对伪造过程进行实现

### Porject20

项目名：ECMH PoC		运行时间：0.00829 s

+ 参照ppt及所查资料对ECMH进行实现

### Project21

项目名：Schnorr 多签方案		运行时间：0.07050 s

+ 查阅相关论文实现了多签时的密钥聚合和签名聚合

创新点：针对Schnorr方案中可能存在的不规范操作进行了分析，并对其中需要随机数生成时出现比特泄漏或随机比特较少时利用LLL算法进行攻击，并完成了代码实现

### Project22

项目名：MPT研究报告

+ 查阅标准文档和有关论文对ppt上的问题进行思考，在C上简化实现了相关功能
