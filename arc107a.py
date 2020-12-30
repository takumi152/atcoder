def main():
    a, b, c = map(int, input().split())

    aa = a * (a + 1) // 2
    bb = b * (b + 1) // 2
    cc = c * (c + 1) // 2

    ans = (aa * bb * cc) % 998244353

    print(ans)

if __name__ == '__main__':
    main()
