
def main():
    a, b, c = map(int, input().split())

    print(max(0, (b + c) - a))

if __name__ == '__main__':
    main()
