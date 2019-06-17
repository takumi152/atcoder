# WIP (gives WA and TLE)

def main():
    buf = input()
    N = int(buf)
    buf = input()
    buflist = buf.split()
    A = list(reversed(list(map(int, buflist))))
    A_inv = list(map(lambda x: x * (-2), A))
    dp_pos = []
    dp_neg = []
    for i in range(N):
        dp_pos.append([])
        dp_neg.append([])
        for j in range(16): # 10**9 < 4**15
            dp_pos[i].append(0)
            dp_neg[i].append(0)
    for j in range(16):
        dp_pos[0][j] = j * 2
        dp_neg[0][j] = j * 2 + 1
    for i in range(1, N):
        # update dp_pos
        count = 0
        factor = 1
        while A[i-1] * factor < A[i]:
            count += 1
            factor *= 4
        dp_pos[i][0] = dp_pos[i-1][count]
        for j in range(1, 16):
            if A[i] * int(4 ** j) <= A[i-1]:
                dp_pos[i][j] = dp_pos[i][j-1] + 2
            else:
                dp_pos[i][j] = dp_pos[i][j-1] + 2 + 2 * i
        # update dp_neg
        count = 0
        factor = 1
        while A_inv[i-1] * factor > A_inv[i]:
            count += 1
            factor *= 4
        dp_neg[i][0] = dp_neg[i-1][count] + 1
        for j in range(1, 16):
            if A_inv[i] * int(4 ** j) >= A_inv[i-1]:
                dp_neg[i][j] = dp_neg[i][j-1] + 2
            else:
                dp_neg[i][j] = dp_neg[i][j-1] + 2 + 2 * i
    #print("dp_pos:")
    #for i in dp_pos:
    #    print(i)
    #print("dp_neg:")
    #for i in dp_neg:
    #    print(i)
    minimum = min(dp_pos[N-1][0], dp_neg[N-1][0])
    for i in range(N-1):
        #print(i, dp_pos[i][0] + dp_neg[N-2-i][0])
        if dp_pos[i][0] + dp_neg[N-2-i][0] < minimum:
            minimum = dp_pos[i][0] + dp_neg[N-2-i][0]
    print(minimum)

if __name__ == '__main__':
    main()
