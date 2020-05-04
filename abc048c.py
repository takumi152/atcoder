def main():
    n, x = map(int, input().split())
    a = list(map(int, input().split()))

    ans = 0
    for i in range(n-1):
        if a[i] + a[i+1] > x:
            num_to_eat = min(a[i+1], a[i] + a[i+1] - x)
            ans += num_to_eat
            a[i+1] -= num_to_eat
            if a[i] + a[i+1] > x:
                num_to_eat = min(a[i], a[i] + a[i+1] - x)
                ans += num_to_eat
                a[i] -= num_to_eat

    print(ans)

if __name__ == '__main__':
    main()
