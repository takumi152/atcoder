
def main():
    buf = input()
    N = int(buf)
    A = []
    B = []
    for i in range(N):
        buf = input()
        buflist = buf.split()
        A.append(int(buflist[0]))
        B.append(int(buflist[1]))
    distance_sum = 0
    for i in range(N):
        distance_sum += B[i] - A[i]
    candidates = list(sorted(A + B))
    shortest = int(2 ** 64)
    for entrance in candidates:
        for exit in candidates[i:]:
            time_taken = distance_sum
            for customer in range(N):
                time_taken += abs(A[customer] - entrance) + abs(B[customer] - exit)
            if time_taken < shortest:
                shortest = time_taken
    print(shortest)

if __name__ == '__main__':
    main()
