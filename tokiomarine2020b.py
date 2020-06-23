def main():
    a, v = map(int, input().split())
    b, w = map(int, input().split())
    t = int(input())

    dist = abs(a - b)
    if dist - (v - w) * t <= 0:
        print('YES')
    else:
        print('NO')    

if __name__ == '__main__':
    main()
