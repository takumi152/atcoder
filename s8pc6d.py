
def main():
    buf = input()
    N = int(buf)
    snowball = []
    for i in range(N):
        buf = input()
        buflist = buf.split()
        snowball.append([int(buflist[0]), int(buflist[1])]) # [X[i], R[i]]
    snowball = list(sorted(snowball, key=lambda x:x[0]))
    largest = snowball[0][1]
    left_ball_list = []
    right_ball_list = []
    left_ball_size = snowball[0][1]
    left_ball_list.append([snowball[0][0], left_ball_size])
    for i in range(N-1):
        left_ball_size -= snowball[i+1][0] - snowball[i][0]
        if left_ball_size < 0:
            left_ball_size = 0
        left_ball_size = (left_ball_size ** 3 + snowball[i+1][1] ** 3) ** (1/3)
        if left_ball_size > largest:
            largest = left_ball_size
        left_ball_list.append([snowball[i+1][0], left_ball_size])
    snowball = list(reversed(snowball))
    right_ball_size = snowball[0][1]
    right_ball_list.append([snowball[0][0], right_ball_size])
    for i in range(N-1):
        right_ball_size -= snowball[i][0] - snowball[i+1][0]
        if right_ball_size < 0:
            right_ball_size = 0
        right_ball_size = (right_ball_size ** 3 + snowball[i+1][1] ** 3) ** (1/3)
        if right_ball_size > largest:
            largest = right_ball_size
        right_ball_list.append([snowball[i+1][0], right_ball_size])
    right_ball_list = list(reversed(right_ball_list))
    for i in range(N-1):
        left_ball = left_ball_list[i]
        right_ball = right_ball_list[i+1]
        distance = right_ball[0] - left_ball[0]
        if left_ball[1] > right_ball[1]:
            rb = right_ball[1] - distance
            if rb < 0:
                rb = 0
            ball_size = (left_ball[1] ** 3 + rb ** 3) ** (1/3)
        else:
            lb = left_ball[1] - distance
            if lb < 0:
                lb = 0
            ball_size = (lb ** 3 + right_ball[1] ** 3) ** (1/3)
        if ball_size > largest:
            largest = ball_size
    print(largest)

if __name__ == '__main__':
    main()
