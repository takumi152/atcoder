def main():
    n = int(input())
    a = list(map(int, input().split()))

    mod = 998244353

    a.sort()

    ans = 0
    left_pow = n-1
    right_sum = 0
    for i in range(1, n):
        right_sum = (right_sum + a[i] * pow(2, i-1, mod)) % mod
    for i in range(n):
        ans = (ans + a[i] * ((right_sum + a[i]) % mod)) % mod
        left_pow -= 1
        if i < n-1:
            right_sum -= a[i+1]
            if right_sum < 0:
                right_sum += mod
        right_sum = (right_sum * pow(2, mod-2, mod)) % mod

    print(ans)

if __name__ == '__main__':
    main()
