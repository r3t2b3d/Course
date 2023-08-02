# Schnorr哈希算法误用实例分析

主要涉及的就是一个对应比特的dlp下用合适的哈希函数，否则容易引发各种问题

我们来看下面一种Schnorr实现

```python
def keygen():
    p = getStrongPrime(512)
    g = 2
    x = randint(1, p-1)
    y = pow(g, x, p)
    return (p, g, y), x


def H(m):
    return bytes_to_long(2*sha256(m).digest())


def schnorr(m, pk, sk):
    p, g, y = pk
    k = H(os.urandom(16)+m)
    r = pow(g, k, p)
    e = H(long_to_bytes(r)+m)
    s = (k-sk*e)%(p-1)
    return (r, s)
```



这里我们关注Schnorr的签名表达式

$s=k-x\cdot e$

$k = h(m||x)\cdot (2^{256}+1)=k_1\cdot (2^{256}+1)$

可以取两次签名结果，得如下方程

$s_1=k_1\cdot (2^{256}+1)-x\cdot e_1\,mod\,q$

$s_2=k_2\cdot (2^{256}+1)-x\cdot e_2\,mod\,q$

$\Rightarrow e_2s_1-e_1s_2=(k_1e_2-k_2e_1)\cdot (2^{256}+1)\,mod\,q$

$\Rightarrow e_2^{-1}e_1\cdot k_2+e_2^{-1}(e_2s_1-e_1s_2)(2^{256}+1)^{-1}=k_1\,mod\,q$

构造 lattice
$$
\left[\begin{matrix} 1&&e_1e_2^{-1}\\
&2^{256}&e_2^{-1}(e_2s_1-e_1s_2)(2^{256}+1)^{-1}\\
&&q
\end{matrix}\right]
$$
利用LLL算法可以还原出 $k_2$，即可求出私钥

我们很容易看到这主要问题还是k值的随机比特不够多，在这里只有256比特，这个问题其实类似于HNP对k出现比特泄漏DSA的攻击

总的来说在进行密码算法实现的时候需要多关注随机数多质量才能避免一些可能的密码分析手段