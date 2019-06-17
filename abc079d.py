
c = []

def main():
    buf = input()
    buflist = buf.split()
    H = int(buflist[0])
    W = int(buflist[1])
    global c
    for i in range(10):
        buf = input()
        buflist = buf.split()
        c.append(list(map(int, buflist)))
    A = []
    for i in range(H):
        buf = input()
        buflist = buf.split()
        A.append(list(map(int, buflist)))
    cost_list = []
    for i in range(10):
        nlist = []
        for j in range(10):
            if j != 1:
                nlist.append(j)
        cost_list.append(min_cost_of(i, 1, 1, nlist, 0))
    number_count = []
    for i in range(10):
        i_count = 0
        for j in range(H):
            for k in range(W):
                if A[j][k] == i:
                    i_count += 1
        number_count.append(i_count)
    mp_needed = 0
    for i in range(10):
        mp_needed += cost_list[i] * number_count[i]
    print(mp_needed)

def min_cost_of(s, d, t, nlist, cost):
    if s == d or s == t:
        return cost
    else:
        minimum = c[s][d]
        for i in nlist:
            rlist = list(nlist)
            rlist.remove(i)
            result = min_cost_of(s, d, i, rlist, cost + c[i][t])
            if result < minimum:
                minimum = result
        return minimum

if __name__ == '__main__':
    main()
