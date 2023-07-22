# ECMH scheme

## 实验环境

+ Python 3.11.4
+ Apple M2 24G
+ SageMath version 10.0

## 实验内容

本项目实现了基于椭圆曲线的多重集合散列

满足 $h(a)+h(b)=h(\{a,b\})$

该方案的想法是将元素映射到椭圆曲线上的点，而椭圆曲线上的点满足加法性质，可利用此来构造满足要求的密码函数，下面对函数正确性进行验证

打印`h({a, b}),h({b, c}),h(b),h({a, b, c})`

验证 `h({a, b})+h({b, c})-h(b) = h({a, b, c})`

## 实验结果

![image-20230713165843138](https://oyrd-1313391192.cos.ap-nanjing.myqcloud.com/images/image-20230713165843138.png)