def count_divisor(x):
    count = 0
    i = 1
    while i * i <= x:
        if x % i == 0:
            count += 1
            if i * i < x:
                count += 1
        i += 1
    return count

def main():
    k = int(input())

    ans = 0
    for i in range(1, k+1):
        ans += count_divisor(i) * (k // i)

    print(ans)

if __name__ == '__main__':
    main()
