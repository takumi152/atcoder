def main():
    n = int(input())
    a = list(map(int, input().split()))

    ans = []
    for i in range(n):
        ans.append(0)
    for x in a:
        ans[x-1] += 1

    for x in ans:
        print(x)

if __name__ == '__main__':
    main()
