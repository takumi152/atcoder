
def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    K = int(buflist[1])
    buf = input()
    buflist = buf.split()
    V = list(map(int, buflist))
    max_value = 0
    for i in range(min(N+1, K+1)):
        push_limit = K - i
        for j in range(i+1):
            jewel = []
            for k in range(-i+j, j):
                jewel.append(V[k])
            jewel = list(sorted(jewel))
            value = sum(jewel)
            for k in range(push_limit):
                if k >= len(jewel):
                    break
                if jewel[k] < 0:
                    value += -jewel[k]
                else:
                    break
            if value > max_value:
                max_value = value
    print(max_value)


if __name__ == '__main__':
    main()
