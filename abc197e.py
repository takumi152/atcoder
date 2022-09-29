def main():
    n = int(input())
    x = [0 for _ in range(n)]
    c = [0 for _ in range(n)]
    for i in range(n):
        x[i], c[i] = map(int, input().split())

    c_group = [[] for _ in range(n)]
    for i in range(n):
        c_group[c[i]-1].append(x[i])

    for i in range(n):
        c_group[i].sort()

    # time, position
    left = (0, 0)
    right = (0, 0)
    for i in range(n):
        if len(c_group[i]) == 0:
            continue
        elif len(c_group[i]) == 1:
            next = (min(left[0] + abs(left[1] - c_group[i][0]), right[0] + abs(right[1] - c_group[i][0])), c_group[i][0])
            left, right = next, next
        else:
            next_left = (min(left[0] + max(0, left[1] - c_group[i][0]) + max(0, c_group[i][-1] - left[1]) * 2, right[0] + max(0, right[1] - c_group[i][0]) + max(0, c_group[i][-1] - right[1]) * 2), c_group[i][0])
            next_right = (min(left[0] + max(0, left[1] - c_group[i][0]) * 2 + max(0, c_group[i][-1] - left[1]), right[0] + max(0, right[1] - c_group[i][0]) * 2 + max(0, c_group[i][-1] - right[1])), c_group[i][-1])
            left, right = next_left, next_right

    ans = min(left[0] + abs(left[1]), right[0] + abs(right[1]))

    print(ans)

if __name__ == '__main__':
    main()
