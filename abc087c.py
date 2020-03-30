def main():
    n = int(input());
    a1 = list(map(int, input().split()))
    a2 = list(map(int, input().split()))

    maximum = 0
    for i in range(n):
        score = sum(a1[0:i+1]) + sum(a2[i:n])
        if score > maximum:
            maximum = score

    print(maximum)
    return


if __name__ == '__main__':
    main()
