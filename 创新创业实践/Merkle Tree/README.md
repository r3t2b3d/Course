# RFC6962 Merkle Tree

### 介绍

RFC 6962即证书透明度标准文档，于2013年发布旨在增加公信力和安全性，以保护互联网用户免受恶意的SSL/TLS证书的攻击。在证书透明度的框架中，日志服务器使用Merkle Tree来存储和组织提交的证书，该项目实现了该文档所描述的简化版merkle tree，功能包括merkle tree构造，快速错误查找，存在性证明

### 实验结果

![截屏2023-07-13 15.18.03](https://oyrd-1313391192.cos.ap-nanjing.myqcloud.com/images/%E6%88%AA%E5%B1%8F2023-07-13%2015.18.03.png)