def main():
    a = list(map(int, input().split()))

    for i in range(5):
        if a[i] == 0:
            print(i+1)
            return

if __name__ == '__main__':
    main()
