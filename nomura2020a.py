def main():
    h1, m1, h2, m2, k = map(int, input().split())

    t1 = h1 * 60 + m1
    t2 = h2 * 60 + m2
    ans = (t2 - t1) - k

    print(ans)

if __name__ == '__main__':
    main()
