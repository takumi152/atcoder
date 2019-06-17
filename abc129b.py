
def main():
    buf = input()
    N = int(buf)
    buf = input()
    buflist = buf.split()
    W = list(map(int, buflist))
    minimum = int(1e9)+7
    for i in range(1, N):
        diff = abs(sum(W[0:i]) - sum(W[i:N]))
        if diff < minimum:
            minimum = diff
    print(minimum)

if __name__ == '__main__':
    main()
