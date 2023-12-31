# 哈希长度拓展攻击

哈希长度拓展攻击对所有采用Merkle-Damgård结构的哈希算法皆适用

## 实验环境

+ Python 3.11.4

+ Apple M2 24G

## 攻击思路

总的来说我们并不需要过于详细地了解算法细节，我们只需要能划分出单轮哈希过程，就能很轻松地完成哈希长度拓展攻击

![截屏2023-07-13 14.33.54](https://oyrd-1313391192.cos.ap-nanjing.myqcloud.com/images/%E6%88%AA%E5%B1%8F2023-07-13%2014.33.54.png)

大体思路就是在最后的输出的基础上再做一次f的过程，其中f的一个参数为输出O1, 另一参数为附加信息M，这样我们就可得到原消息经过填充后加上附加信息做哈希后所得值

## 实验结果

代码都是用的`assert`，这里我们导出一组输出即可

![截屏2023-07-13 14.49.23](https://oyrd-1313391192.cos.ap-nanjing.myqcloud.com/images/%E6%88%AA%E5%B1%8F2023-07-13%2014.49.23.png)

![截屏2023-07-13 18.46.23](https://oyrd-1313391192.cos.ap-nanjing.myqcloud.com/images/%E6%88%AA%E5%B1%8F2023-07-13%2018.46.23.png)

