# PGP scheme with SM2

协议包括三部分

+ Generate session key：SM2 key exchange

+ Encrypt session key：SM2 encryption

+ Encrypt data：Symmetric encryption

密钥交换部分参考 https://onlinelibrary.wiley.com/doi/epdf/10.1002/sec.987

会话密钥传递采用SM2公钥加密算法

数据加密采用AES-ECB

### 实验结果

![image-20230713171526675](https://oyrd-1313391192.cos.ap-nanjing.myqcloud.com/images/image-20230713171526675.png)