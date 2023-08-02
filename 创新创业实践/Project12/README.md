# Pitfalls with proof-of-concept code

## 实验环境

+ Python 3.11.4

+ Apple M2 24G

+ SageMath version 10.0

## 实验内容

![image-20230714142257548](https://oyrd-1313391192.cos.ap-nanjing.myqcloud.com/images/image-20230714142257548.png)

本项目所有代码均在开源数学软件SageMath上实现，特别的，本项目在原基础上拓展了曲线安全性的选取工作，并对相关攻击算法做了一定的分析与实现

**算法分析**

### ECDSA

+ Key Gen：

选择曲线E_p(a, b)，选择点G，G的阶为n。随机选择d，公钥 $P=dG$

+ Sign

$e = h(m)$

随机选择k，$(x,\,y)=k*G$

$r=x(mod\,n)$

$s=k^{-1}(e+rd)$

输出 $(r,s)$

+ Verify

$e=h(m)$

$u_1=es^{-1}(mod~n)$

$u_2=rs^{-1}(mod~n)$

验证 $[u_1G+u_2P]_x(mod~n)=r$，满足则验签成功

### Schnorr

+ Key Gen

选择素数q，素数 $p=2q+1$，选择原根g。随机选择d，公钥 $y=g^d$

+ Sign

随机选择k, $r=g^k$

$e=h(r||m)$

$s=k-xe$

输出 $(s, e)$

+ Verify

$r_v=g^sy^e$

$e_v=h(r_v||m)$

验证 $e_v=e$，满足则验签成功

### SM2-sig

+ Key Gen

选择曲线E_p(a, b)，选择点G，G的阶为n。随机选择d，公钥 $P=dG$

+ Sign

$e=h(m)$

随机选取k，$P_1=k\cdot G$

$x=(P_1)_x$

$R=e+x(mod~n)$

$S=d^{-1}(k+R)-R(mod~n)$

输出 $(R, S)$

+ Verify

$e=h(m)$

$t=R+S(mod~n)$

$P_1=R*G$

$P_2=t*P$

$x=(P_2-P_1)_x$

判断 $R=e+x$

### 攻击思路

三种算法在这几种情景下思路类似，故只列出dsa的具体数学表达式，其他情况以此类推

1⃣️ Leaking k leads to leaking of private key

$x=(s\cdot k-H(m))\cdot r^{-1}\,mod\,q$ 

2⃣️ Reusing k leads to leaking of private key

$k=s_1^{-1}\cdot(H(m_1)+x\cdot r_1)\,mod\,q$

$k=s_2^{-1}\cdot(H(m_2)+x\cdot r_2)\,mod\,q$

两式相减，消去k

$(s_2\cdot r_1-s_1\cdot r_2)\cdot x=H(m_2)-H(m_1)\,mod\,q$

$x=(H(m_2)-H(m_1))\cdot (s_2\cdot r_1-s_1\cdot r_2)^{-1}\,mod\,q$

3⃣️ Same sk and k with dsa, leads to leaking of private key

$(s-r)\cdot x=H(m)\,mod\,q$

$x=H(m)\cdot (s-r)^{-1}\,mod\,q$

4⃣️ Forge signature without check m

在只需提交哈希值和相应签名时，伪造ecdsa的签名是容易的

ECDSA的签名生成

+ $R = kG$

+ $P = dG$

+ $s = k^{-1}(e+d*r)$

验证

+ $sR = eG+rP$

我们需要提交`(r, s, e)`进行验签

不妨设  $R = uG+vP$

我们只需构造 $suG+svP = eG+rP$

只要R确定，`v, r`均确定，取 $s=r*v^{-1}$

故 $e=su=rv^{-1}u$

在不给出e对应的明文的情况下，我们可以通过验证

## 拓展

上述表格中 $(r,s);(r,-s)$ 的验证是简单的不做过多赘述，由于基于数域DLP的密码方案都可以移植到椭圆曲线上，椭圆曲线的选取一直是值得关注的，这里来简要说明两类椭圆曲线的脆弱性，在依赖ECDLP困难性的椭圆曲线选取中需规避这些情况

**Smart Attack**

对于Frobenius trace为1的椭圆曲线$E_p$上的点$P,Q=dP$，求解d的流程如下

+ 利用Hensel lift将点P, Q 提升到$E(Q_p)$上点 P', Q'
+ 定义 $E_1(Q_p)$ 为$E(Q_p)$到$E(F_p)$同态的核，$E_2(Q_p)$ 为$E_1(Q_p)$到$pZ_p$同态的核
+ $Q=dP\Rightarrow Q'-dP'\in E_1(Q_p)$
+ $pQ'-d(pP')\in E_2(Q_p)$
+ 利用同构映射 $\psi(S)=-\frac{S_x}{S_y}$将$E_1(Q_p)$映射到$pZ_p$
+ $\psi(pQ')-d\psi(pP')=0\,mod\,p^2$
+ $d=\frac{\psi(pQ')}{\psi(pP')}$

攻击代码实现

```python
q = 0xd3ceec4c84af8fa5f3e9af91e00cabacaaaecec3da619400e29a25abececfdc9bd678e2708a58acb1bd15370acc39c596807dab6229dca11fd3a217510258d1b
a = 0x95fc77eb3119991a0022168c83eee7178e6c3eeaf75e0fdf1853b8ef4cb97a9058c271ee193b8b27938a07052f918c35eccb027b0b168b4e2566b247b91dc07
b = 0x926b0e42376d112ca971569a8d3b3eda12172dfb4929aea13da7f10fb81f3b96bf1e28b4a396a1fcf38d80b463582e45d06a548e0dc0d567fc668bd119c346b2
E=EllipticCurve(GF(q),[a, b])
assert ZZ(E.order()) == ZZ(q)


def SSAS(P, Q, p):
    def phi(S):
        x, y = S.xy()
        return -x/y
    E = P.curve()
    Q_p = Qp(p, 2)
    E2 = EllipticCurve(Qp(p, 2), [ZZ(a) + randint(0,p)*p for a in E.a_invariants() ])
    
    for item in E2.lift_x(ZZ(P.xy()[0]), all=True):
        if GF(p)(item.xy()[1]) == P.xy()[1]:
            P_ = item
            
    for item in E2.lift_x(ZZ(Q.xy()[0]), all=True):
        if GF(p)(item.xy()[1]) == Q.xy()[1]:
            Q_ = item
    
    P_ = p*P_
    Q_ = p*Q_
    print(p*P_)
    return ZZ(phi(Q_)/phi(P_))


P = E.random_element()
d = randint(1, q-1)
Q = d*P

assert d == SSAS(P, Q, q)
```



**MOV Attack**

对于嵌入度为k的椭圆曲线 $E(F_p)$ 上的点 $P,Q=dP$ ，求解d的流程如下

+ 由于pairing是定义在代数闭域的椭圆曲线上，我们先利用同态将P，Q提升到 $E(F_p^k)$
+ 随机生成点R，满足和P, Q同阶，且与P, Q非线性
+ 计算 $(P, R), (Q, R)$的pairing结果 $s_1,s_2$
+ 在 $F_p^k$ 下求解 d 满足 $s_2=s_1^d$

攻击代码实现

```python
from Crypto.Util.number import *


p = getPrime(100)
assert p%3 == 2
F1 = GF(p)
F2 = GF(p^2)
E1 = EllipticCurve(F1, [0, 3])
E2 = EllipticCurve(F2, [0, 3])
P = E1.random_point()
d = 1234567891
Q = d*P


phi = Hom(F1, F2)(F1.modulus().roots(F2)[0][0])
P_ = E2(phi(P.xy()[0]), phi(P.xy()[1]))
Q_ = E2(phi(Q.xy()[0]), phi(Q.xy()[1]))
n = E2.order()
n1 = P_.order()

while True:
    R = E2.random_point()
    if R.order()%n1 == 0:
        R = R.order()//n1*R
        break


s1 = P_.weil_pairing(R, n1)
s2 = Q_.weil_pairing(R, n1)

assert s1 != 0
s = s2.log(s1)
print(s)
```

## 总结

本项目对现阶段区块链上的可能应用的基础算法进行了一定程度分析，实际上随着现阶段的研究和新兴技术对出现，对该类算法的分析越来越强力，后续将介绍利用格基规约技术对上述算法做进一步的分析