# AES/SM4 软件实现

## 实验环境

+ Python 3.11.4

+ Apple clang version 14.0.3 (clang-1403.0.22.14.1)

+ Apple M2 24G

AES

+ 编程语言：Python

+ 不依赖AES密码学封装库，单独实现了基础的AES加解密，并验证了加解密结果正确性

SM4

+ 编程语言：C
+ 不依赖任何密码学封装库，在C下实现了SM4加解密，并利用GMSSL库进行正确性的验证

## 实验结果

![截屏2023-07-13 15.43.25](https://oyrd-1313391192.cos.ap-nanjing.myqcloud.com/images/%E6%88%AA%E5%B1%8F2023-07-13%2015.43.25.png)

![截屏2023-07-13 15.44.11](https://oyrd-1313391192.cos.ap-nanjing.myqcloud.com/images/%E6%88%AA%E5%B1%8F2023-07-13%2015.44.11.png)

```python
from gmssl import sm4

m = b"it's a test text"
key = b"it's a test key!"
s = sm4.CryptSM4(mode=sm4.SM4_ENCRYPT, padding_mode=1)
s.set_key(key, sm4.SM4_ENCRYPT)
c = s.crypt_ecb(m)
print(c.hex()[:32])
```

![截屏2023-07-13 15.44.41](https://oyrd-1313391192.cos.ap-nanjing.myqcloud.com/images/%E6%88%AA%E5%B1%8F2023-07-13%2015.44.41.png)