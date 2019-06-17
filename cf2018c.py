
def main():
    buf = input()
    N = int(buf)
    cake = []
    for i in range(N):
        cake.append([])
        for j in range(N):
            cake[i].append(".")
    start = 2
    for i in range(N):
        for j in range(start, N, 5):
            cake[i][j] = "X"
        start -= 2
        if start < 0:
            start += 5
    for i in range(N):
        for j in range(N):
            if (i == 0 or cake[i-1][j]==".") and (j == 0 or cake[i][j-1]==".") and (i == N-1 or cake[i+1][j]==".") and (j == N-1 or cake[i][j+1]=="."):
                cake[i][j] = "X"
    for i in range(N):
        str_to_print = ""
        for j in range(N):
            str_to_print += cake[i][j]
        print(str_to_print)

if __name__ == '__main__':
    main()
