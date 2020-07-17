def main():
    x, n = map(int, input().split())
    p = list(map(int, input().split()))

    best_num = 0
    best_diff = 999
    for i in range(102):
        if i not in p and abs(x - i) < best_diff:
            best_num = i
            best_diff = abs(x - i)

    print(best_num)

if __name__ == '__main__':
    main()
