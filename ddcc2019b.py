
def main():
    buf = input()
    N = int(buf)
    black_count = 0
    point_list = []
    for i in range(N+1):
        point_list.append([])
        for j in range(N+1):
            point_list[i].append(False)
    center_x = N / 2
    center_y = N / 2
    for x in range(N+1):
        for y in range(N+1):
            if calc_distance((x, y), (center_x, center_y)) <= N / 2:
                point_list[x][y] = True
    for x in range(N):
        for y in range(N):
            if point_list[x][y] and point_list[x+1][y] and point_list[x][y+1] and point_list[x+1][y+1]:
                black_count += 1
    print(black_count)


def calc_distance(p1, p2):
    return abs(p1[0] - p2[0]) + abs(p1[1] - p2[1])


if __name__ == '__main__':
    main()
