import socket
from hashlib import shake_128, sha256
from Crypto.Util.strxor import strxor
from Crypto.Util.number import *

buffer_size = 4028

p = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F
a = 0x0000000000000000000000000000000000000000000000000000000000000000
b = 0x0000000000000000000000000000000000000000000000000000000000000007
Gx = 0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798
Gy = 0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8
ord_ = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141

F = GF(p)
E = EllipticCurve(F, [a, b])
G = E(Gx, Gy)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('127.0.0.1', 10004))

d1 = 8711767019441335608202321807407936183884549150317943106278606559169717416483
pk = E(6496291364447780596933144322518283314055871112838673593310732284106666741710, 73279099192725378952392893099556444238854765124013803972089552657329614616791)


def KDF(data, klen):
    return shake_128(data).digest(klen//8)

def pad(msg, pl):
    re = msg
    while len(re)<pl:
        re = b'\x00'+re
    return re
        

def enc(msg, P):
    k = randint(1, ord_-1)
    C1 = k*G
    x2, y2 = (k*P).xy()
    t = KDF((str(x2)+str(y2)).encode(), 256)
    C2 = strxor(pad(msg, 32), t).hex()
    C3 = sha256(str(x2).encode()+msg+str(y2).encode()).hexdigest()
    return str(C1.xy())+'&'+C2+'&'+C3

C = enc(b'test_msg', pk).split('&')

C1 = E(eval(C[0]))
C2 = int(C[1], 16)
C3 = C[2]
print(C3)
assert C1 != E(0)
T1 = inverse_mod(d1, ord_)*C1

s.send(str(T1.xy()).encode())

T2 = E(eval(s.recv(buffer_size)))
x2, y2 = (T2-C1).xy()
t = KDF(str(x2).encode()+str(y2).encode(), 256)
M = long_to_bytes(C2^^int(t.hex(), 16))
u = sha256(str(x2).encode()+M+str(y2).encode()).hexdigest()

if u == C3:
    print("PlainText :", M)
    
s.close()
