def solve(n, x):
    dp = [[0 for _ in range(2)] for _ in range(64)]
    for i in range(62, -1, -1):
        if 2 ** i > x:
            continue
        elif 2 ** (i + 1) > x:
            if n & 2 ** i:
                dp[i][0] = 1
        else:
            if n & 2 ** i:
                if x & 2 ** i:
                    dp[i][0] = dp[i+1][0] * 2 + 1

                else:


def main():
    n, l, r = map(int, input().split())


if __name__ == '__main__':
    main()
