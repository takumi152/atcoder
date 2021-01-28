def main():
    n = int(input())
    a = list(map(int, input().split()))

    a_next = []
    for i in range(len(a)):
        a_next.append((a[i], i+1))
    a = a_next

    for i in range(n-1):
        a_next = []
        for j in range(0, len(a), 2):
            a_next.append(max(a[j], a[j+1]))
        a = a_next

    print(min(a)[1])

if __name__ == '__main__':
    main()
