def main():
    v, t, s, d = map(int, input().split())

    vanish_start = v * t
    vanish_end = v * s

    if d < vanish_start or vanish_end < d:
        print('Yes')
    else:
        print('No')

if __name__ == '__main__':
    main()
