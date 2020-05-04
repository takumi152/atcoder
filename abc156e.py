mod = int(10 ** 9) + 7

def modinv(x, m = mod):
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

def main():
    n, k = map(int, input().split())

    if k > n-1:
        k = n-1

    factorial = [None for _ in range(200001)]
    factorial[0] = 1
    factorial[1] = 1
    for i in range(2, 200001):
        factorial[i] = (factorial[i-1] * i) % mod

    posCount = [None for _ in range(n+1)]
    for i in range(n+1):
        posCount[i] = (factorial[n] * modinv((factorial[i] * factorial[n-i]) % mod)) % mod

    placeCount = [None for _ in range(n+1)]
    placeCount[0] = 0
    for i in range(1, n+1):
        placeCount[i] = (factorial[n-1] * modinv((factorial[i-1] * factorial[(n-1)-(i-1)]) % mod)) % mod

    ans = 0
    for i in range(-1, -k-2, -1):
        ans = (ans + (posCount[i] * placeCount[i])) % mod
    if k == 1:
        ans -= 1
        if ans < 0:
            ans += mod

    print(ans)

if __name__ == '__main__':
    main()
