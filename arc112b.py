def main():
    b, c = map(int, input().split())

    possible_range = []
    if b > 0:
        possible_range.append((b - c // 2, b))
        if c >= 1:
            possible_range.append((-b - (c - 1) // 2, -b))
            possible_range.append((-b, -b + (c - 1) // 2))
        if c >= 2:
            possible_range.append((b, b + (c - 2) // 2))
    else:
        possible_range.append((b - c // 2, b))
        if c >= 1:
            possible_range.append((-b - (c - 1) // 2, -b))
            possible_range.append((-b, -b + (c - 1) // 2))

    event = []
    for x in possible_range:
        event.append((x[0], 1))
        event.append((x[1]+1, -1))

    event.sort()

    ans = 0
    d = 0
    for i in range(len(event)-1):
        d += event[i][1]
        if d > 0:
            ans += event[i+1][0] - event[i][0]

    print(ans)

if __name__ == '__main__':
    main()
