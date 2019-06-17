# WIP

MOD = int(1e6) + 3

def main():
    buf = input()
    Q = int(buf)
    xdn = []
    for i in range(Q):
        buf = input()
        buflist = buf.split()
        xdn.append((int(buflist[0]), int(buflist[1]), int(buflist[2])))
    for i in Q:
        x = xdn[i][0]
        d = xdn[i][1]
        n = xdn[i][2]


def modinv(x, m = MOD):
    b = m
    u = 1
    v = 0
    while b:
        t = x // b
        x -= t * b
        x, b = b, x
        u -= t * v
        u, v = v, u
    u %= m
    if u < 0:
        u += m
    return u

if __name__ == '__main__':
    main()
