from numpy import base_repr

def main():
    n = int(input())

    ans = base_repr(n, 36)

    print(ans)

if __name__ == '__main__':
    main()
