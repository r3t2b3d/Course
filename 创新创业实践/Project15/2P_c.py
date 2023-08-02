import socket

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
s.connect(('127.0.0.1', 7777))

data = s.recv(buffer_size)
s.send(b"y")

P1 = E(eval(s.recv(buffer_size)))
d2 = randint(1, ord_-1)
P = inverse_mod(d2, ord_)*P1-G
print("Public Key :", str(P.xy()))

e = int(s.recv(buffer_size))
Q1 = E(eval(s.recv(buffer_size)))
k2 = randint(1, ord_-1)
Q2 = k2*G
k3 = randint(1, ord_-1)
x1 = ZZ((k3*Q1+Q2).xy()[0])
r = (x1+e)%ord_
s2 = d2*k3%ord_
s3 = d2*(r+k2)%ord_

s.send(str(r).encode())
s.send(str(s2).encode())
s.send(str(s3).encode())

s.close()
