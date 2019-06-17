# WIP

MOD = int(1e9)+7

def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    M = int(buflist[1])
    K = int(buflist[2])
    NM = N * M
    expect_vertical = 0
    for i in range(N):
        expect_vertical += i * (i + 1) * M // 2
        expect_vertical += (N - i - 1) * (N - i) * M // 2
    combination = 1
    combination_exclf = 1
    for i in range(K):
        combination *= NM - i
        combination %= MOD
        combination_exclf *= i + 1
        combination_exclf %= MOD


def modinv(x, m = MOD):
    b = m
    u = 1
    v = 0
    while b:
        t = x // b
        x -= t * b
        a, b = b, a
        u -= t * v
        u, v = v, u
    u %= m
    if u < 0:
        u += m
    return u

if __name__ == '__main__':
    main()
