def main():
    a, b = map(int, input().split())

    print(max(0, a - b * 2))

if __name__ == '__main__':
    main()
