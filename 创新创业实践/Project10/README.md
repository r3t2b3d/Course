# Application of this deduce technique in Ethereum with ECDSA

## 实验环境

+ Python 3.11.4

+ Apple M2 24G

+ SageMath version 10.0

## 恢复流程

ECDSA算法如下

+ Key generate：

选择曲线E_p(a, b)，选择点G，G的阶为n。随机选择d，公钥 $P=dG$

+ Sign

$e = h(m)$

随机选择k，$(x,~y)=k*G$

$r=x(mod~n)$

$s=k^{-1}(e+rd)$

输出 $(r,s)$

+ Ver

$e=h(m)$

$u_1=es^{-1}(mod~n)$

$u_2=rs^{-1}(mod~n)$

验证 $[u_1G+u_2P]_x(mod~n)=r$，满足则验签成功

+ Recover

在已知 $(m, r, s)$ 的情况下恢复P

这确实是一个有效的恢复方式，可是如果有人恶意构造r, s也会导致一些伪造发送

在r, s正确的前提下，我们进行如下公钥恢复算法

通过r恢复x，由于  $r=x(mod~n)$，故 $x=r+kn$

计算 $e=h(m)$

$u_1=er^{-1}(mod~n)$

$u_2=sr^{-1}(mod~n)$

公钥 $P=-u_1G+u_2R$

总的来说，此种方式确实能大大减轻实际区块链交易上的带宽，节点仅从签名中恢复对应的用户公钥来确定用户位置

## 算法实现

本项目在开源数学软件SageMath上进行实现

```python
from Crypto.Util.number import *
from hashlib import sha256


def Keygen():
    p = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F
    a = 0x0000000000000000000000000000000000000000000000000000000000000000
    b = 0x0000000000000000000000000000000000000000000000000000000000000007
    Gx = 0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798
    Gy = 0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8
    n = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141
    F = GF(p)
    E = EllipticCurve(F, [a, b])
    G = E(Gx, Gy)    
    d = randint(1, n-1)
    return E, a, b, p, G, n, d
    

def sign(m):
    e = bytes_to_long(sha256(m).digest())
    while True:
        try:
            k = randint(1, n-1)
            x, y = (k*G).xy()
            assert x < n
            r = ZZ(x)%n
            s = inverse_mod(k, n)*(e+r*d)%n
            return (r, s)
        except:
            continue
    

def ver(m, r, s):
    e = bytes_to_long(sha256(m).digest())
    u1 = e*inverse_mod(s, n)%n
    u2 = r*inverse_mod(s, n)%n
    if ZZ((u1*G+u2*P).xy()[0])%n == r:
        return True
    return False

def recover(m, r, s):
    x = r
    R = E.lift_x(x)
    R2 = -R
    e = bytes_to_long(sha256(m).digest())
    u1 = e*inverse_mod(r, n)%n
    u2 = s*inverse_mod(r, n)%n
    P1 = -u1*G+u2*R
    P2 = -u1*G+u2*R2
    return P1, P2

        
E, a, b, p, G, n, d = Keygen()
P = d*G
m = b'hash_hash'
r, s = sign(m)
rec = recover(m, r, s)
print(ver(m, r, s))
assert rec[0] == P or rec[1] == P
```

## 实验结果

![image-20230714130542843](https://oyrd-1313391192.cos.ap-nanjing.myqcloud.com/images/image-20230714130542843.png)