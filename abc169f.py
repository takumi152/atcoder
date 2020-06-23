
mod = 998244353

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
    n, s = map(int, input().split())
    a = list(map(int, input().split()))

    pow2 = [0 for _ in range(n+1)]
    pow2[0] = 1
    for i in range(n):
        pow2[i+1] = (pow2[i] * 2) % mod

    ans = 0
    dp = [0 for _ in range(s+1)]
    dp[0] = pow2[n]
    for i in range(n):
        dp_next = [0 for _ in range(s+1)]
        for j in range(s+1):
            dp_next[j] = (dp_next[j] + dp[j]) % mod
            if j + a[i] <= s:
                dp_next[j+a[i]] = (dp_next[j+a[i]] + (dp[j] * modinv(2))) % mod
        dp = dp_next

    print(dp[s])

if __name__ == '__main__':
    main()
