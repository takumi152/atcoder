# WIP

def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    M = int(buflist[1])
    q = int(buflist[2])
    x = []
    p = []
    for i in range(M):
        buf = input()
        buflist = buf.split()
        x.append(int(buflist[0]))
        p.append(int(buflist[1]))
    probability_list = []
    for i in range(M):
        probability_list.append(p[i]/q)
    print(probability_list)
    total = 0
    remaining = N
    while remaining > 0:
        existence_chance = []
        for j in range(M):
            existence_chance.append(1-(1-probability_list[j])**remaining)
        scorelist = []
        for j in range(M):
            hungry_list = list(x)
            sushi_count = hungry_list[j]
            nonexistent_chance = 1 - existence_chance[j]
            existence_chance.pop(j)
            existence_chance_sorted = [z for _, z in sorted(zip(hungry_list,existence_chance), key=lambda pair: abs(sushi_count-pair[0]))][::-1]
            hungry_list_sorted = [z for z, _ in sorted(zip(hungry_list,existence_chance), key=lambda pair: abs(sushi_count-pair[0]))][::-1]




if __name__ == '__main__':
    main()
