# SM3 attack

## 实验环境

+ Python 3.11.4

+ Apple M2 24G

## 效果测试

SM3压缩参数

```python
reduce_factor = 8 #压缩因子，即精简SM2为原始SM2的前 4*8=32 bits
```

### Birthday Attack

碰撞32比特

![截屏2023-07-13 14.13.57](https://oyrd-1313391192.cos.ap-nanjing.myqcloud.com/images/%E6%88%AA%E5%B1%8F2023-07-13%2014.13.57.png)

碰撞40比特

![截屏2023-07-13 14.11.58](https://oyrd-1313391192.cos.ap-nanjing.myqcloud.com/images/%E6%88%AA%E5%B1%8F2023-07-13%2014.11.58.png)

### Rho Attack

碰撞32bite

![截屏2023-07-13 14.18.49](https://oyrd-1313391192.cos.ap-nanjing.myqcloud.com/images/%E6%88%AA%E5%B1%8F2023-07-13%2014.18.49.png)

碰撞40比特

![截屏2023-07-13 14.25.35](https://oyrd-1313391192.cos.ap-nanjing.myqcloud.com/images/%E6%88%AA%E5%B1%8F2023-07-13%2014.25.35.png)