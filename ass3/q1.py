def xor(f2, f1): 
    return bytearray(a^b for a, b in zip(f2, f1))


with open("c1", "rb") as f1:
    d1 = bytearray(f1.read())

with open("c2", "rb") as f2:
    d2 = bytearray(f2.read())

xored = xor(d1, d2)

with open ("p", 'wb') as file:
    file.write(xored)


