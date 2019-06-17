
MOD = int(1e9) + 7

def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    M = int(buflist[1])
    if M == 1:
        print(1)
        return
    prime_factor = prime_factorization(M)
    factor_count = []
    for k, v in prime_factor.items():
        factor_count.append(v)
    max_factor = max(factor_count)
    nPr = [1]
    nCr = [1]
    factorial = [1]
    rHn = [1]
    for i in range(N, 0, -1):
        nPr.append((nPr[-1] * i) % MOD)
        nCr.append((nCr[-1] * i * modinv(N - i + 1)) % MOD)
    for i in range(1, N + max_factor + 1):
        factorial.append((factorial[-1] * i) % MOD)
    for i in range(1, max_factor+1):
        rHn.append((factorial[N+i-1] * modinv(factorial[N-1]) * modinv(factorial[i])) % MOD)
    combination = 1
    for x in factor_count:
        # n個のボールをr個の箱に入れる場合の組み合わせ総数は？
        # = rHn
        # = n+r-1Cn
        combination = (combination * rHn[x]) % MOD
    print(combination)


def prime_factorization(x):
    n = x
    i = 2
    factor = {}
    while i * i <= n:
        while n % i == 0:
            if i in factor:
                factor[i] += 1
            else:
                factor.update({i : 1})
            n //= i
        i += 1
    if n > 1 or not factor:
        if n in factor:
            factor[n] += 1
        else:
            factor.update({n : 1})
    return factor

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
