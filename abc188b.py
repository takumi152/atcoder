def main():
    n = int(input())
    a = list(map(int, input().split()))
    b = list(map(int, input().split()))

    dot = 0
    for i in range(n):
        dot += a[i] * b[i]

    if dot == 0:
        print('Yes')
    else:
        print('No')

if __name__ == '__main__':
    main()
