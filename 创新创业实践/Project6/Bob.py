import socket
from Crypto.Util.number import *
from hashlib import sha256

buffer_size = 4096

soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
soc.bind(('127.0.0.1', 12345))
soc.listen(1)

conn, addr = soc.accept()
conn.send(b"Begin Range Proof With Hash Function? y/n")

data = conn.recv(buffer_size)
if data == b'n':
    conn.send(b'bye')
    conn.close()
    soc.close()


def issuer_sign(msg):
    p = 172434534038649216681165139995315054530860918939223536114664415929643163810660757929816612984249433285617777400861070271290999313291908214297236896361603728225518440672594254125075181372482173095091004860898677545431019777744189261626836380367332186006200809474401669581093184942866500522623787834474773771497
    q = 146471619046198012834627531501874513969734146218345091814900343205984153390178437199119800563435298441568593344741116358787095734697188300192214202753664437755969066680437530012101859266470648236069102571646240878612096240321433856608309944141755328585909586554898784978107912930383332903403090955922815776489
    n = p*q
    e = 0x10001
    phi = (p-1)*(q-1)
    d = inverse_mod(e, phi)
    msg = bytes_to_long(msg)
    s = pow(msg, d, n)
    return s
    
def issuer(age, valid):
    s = long_to_bytes(randint(0, 2^128))
    born = 2021-age
    k = valid-born
    c = s
    for _ in range(k):
        c = sha256(c).digest()
    return s, issuer_sign(c)

year = 2021
age = 43
s, sig = issuer(age, 2100)
low = 21
least = year-low
d0 = least-(year-age)
for _ in range(d0):
    s = sha256(s).digest()

conn.send(s.hex().encode())
conn.send(str(sig).encode())    
    

conn.close()
soc.close()