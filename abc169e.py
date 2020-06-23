def main():
    n = int(input())
    ab = [None for _ in range(n)]
    for i in range(n):
        ab[i] = list(map(int, input().split()))

    ab.sort(key=lambda x: x[0])
    if n % 2 == 0:
        median_min1 = ab[n//2-1][0]
        median_min2 = ab[n//2][0]
    else:
        median_min1 = ab[n//2][0]
        median_min2 = ab[n//2][0]

    ab.sort(key=lambda x: x[1])
    if n % 2 == 0:
        median_max1 = ab[n//2-1][1]
        median_max2 = ab[n//2][1]
    else:
        median_max1 = ab[n//2][1]
        median_max2 = ab[n//2][1]

    if n % 2 == 0:
        ans = (median_max1 + median_max2) - (median_min1 + median_min2) + 1
    else:
        ans = median_max1 - median_min1 + 1

    print(ans)

if __name__ == '__main__':
    main()
