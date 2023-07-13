# Forge a signature to pretend that you are Satoshi

在只需提交哈希值和相应签名时，伪造ecdsa的签名是容易的

ECDSA的签名生成

+ $R = kG$

+ $P = dG$

+ $s = k^{-1}(e+d*r)$

验证

+ $sR = eG+rP$

我们需要提交`(r, s, e)`进行验签

不妨设  $R = u*G+v*P$

我们只需构造 $s*u*G+s*v*P = e*G+r*P$

只要R确定，`v, r`均确定，取 $s=r*v^{-1}$

故 $e=s*u=r*v^{-1}*u$

在不给出e对应的明文的情况下，我们可以通过验证

### 实验结果

![截屏2023-07-13 18.09.57](https://oyrd-1313391192.cos.ap-nanjing.myqcloud.com/images/%E6%88%AA%E5%B1%8F2023-07-13%2018.09.57.png)