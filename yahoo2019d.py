
def main():
    buf = input()
    L = int(buf)
    A = []
    for i in range(L):
        buf = input()
        A.append(int(buf))
    print(min(calc_move_count(A), calc_move_count(list(reversed(A)))))

def calc_move_count(A):
    stone_diff = []
    for i in A:
        if i == 0:
            stone_diff.append(1)
        elif i % 2 == 0:
            stone_diff.append(-1)
        else:
            stone_diff.append(0)
    no_left_sum = []
    for i in reversed(stone_diff):
        if not no_left_sum:
            no_left_sum.append(abs(i))
        else:
            no_left_sum.append(no_left_sum[-1] + abs(i))
    no_left_sum = list(reversed(no_left_sum))
    no_left_sum.append(0)
    move_left = [0]
    for i in A:
        if not move_left:
            move_left.append(i)
        else:
            move_left.append(move_left[-1] + i)
    move_right = []
    for i in reversed(A):
        if not move_right:
            move_right.append(i)
        else:
            move_right.append(move_right[-1] + i)
    move_right = list(reversed(move_right))
    move_right.append(0)
    dp = [[0, no_left_sum[0], 0, move_right[0]]]
    for i in range(len(A)):
        l = []
        l.append(min(move_left[i+1], dp[-1][0]+abs(stone_diff[i]+1)))
        l.append(min(move_left[i+1], l[0], dp[-1][1]+abs(stone_diff[i])-no_left_sum[i]) + no_left_sum[i+1])
        l.append(min(move_left[i+1], l[1]-no_left_sum[i+1], dp[-1][2]+abs(stone_diff[i]+1)))
        l.append(min(dp[-1][3], l[2] + move_right[i+1]))
        dp.append(l)
    return dp[-1][3]


if __name__ == '__main__':
    main()
