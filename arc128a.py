def main():
    n = int(input())
    a = list(map(int, input().split()))

    gold = True
    ans = [0 for _ in range(n)]
    for i in range(n-1):
        if gold and a[i] > a[i+1]:
            ans[i] = 1
            gold = False
        elif not gold and a[i] < a[i+1]:
            ans[i] = 1
            gold = True
    if not gold:
        ans[-1] = 1

    print(' '.join([str(x) for x in ans]))

if __name__ == '__main__':
    main()
