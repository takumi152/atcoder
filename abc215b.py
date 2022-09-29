def main():
    n = int(input())

    for k in range(0, 100):
        if 2 ** k > n:
            print(k - 1)
            break

if __name__ == '__main__':
    main()
