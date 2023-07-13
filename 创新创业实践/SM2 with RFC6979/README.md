# 按照 RFC6979 标准实现 SM2

该协议需要构造一个依赖待签名数据和私钥的确定性随机数生成算法来替换SM2过程中随机数k的生成，这边hmac并未标准化，本项目选择sha256的后缀加盐作为hmac

```python
def hmac(data, key):
    return sha256(data+key).digest()

def RFC6962_k(data, sk):
    h = sha256(data).digest()
    V = b'\x01'*32
    K = b'\x00'*32
    K = hmac(V+b'\x00'+long_to_bytes(sk)+h, K)
    V = hmac(V, K)
    K = hmac(V+b'\x01'+long_to_bytes(sk)+h, K)
    V = hmac(V, K)

    T = b''
    V = hmac(V, K)
    T = T+V
    k = bytes_to_long(T)
    if 1<= k <= int(default_ecc_table['n'], 16)-1:
        return k
    else:
        while True:
            K = hmac(V+b'\x00', K)
            V = hmac(V, K)
            T = T+V
            k = bytes_to_long(T)
            if 1<= k <= int(default_ecc_table['n'], 16)-1:
                return k
```



### 实验结果

![截屏2023-07-13 16.43.45](https://oyrd-1313391192.cos.ap-nanjing.myqcloud.com/images/%E6%88%AA%E5%B1%8F2023-07-13%2016.43.45.png)

