MOD = int(10e9) + 7

def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    K = int(buflist[1])
    buf = input()
    buflist = buf.split()
    a = list(map(int, buflist))
    buf = input()
    buflist = buf.split()
    b = list(map(int, buflist))
    card_list = list(sorted(list(zip(a,b)), key=lambda x:x[0]))
    kind_list = [0]
    for i, _ in enumerate(card_list):
        kind_list.append(1)
    flag = True
    kind = N
    card_list_s = list(card_list)
    kind_list_s = list(kind_list)
    while flag:
        flag = False
        diff_list = []
        for i, card in enumerate(card_list):
            diff_list.append((card[1] - card[0], i))
        diff_list = list(reversed(list(sorted(diff_list, key=lambda x:x[0]))))
        for i, diff in enumerate(diff_list):
            if diff[0] > 0:
                if kind_list[card_list[diff[1]][0]] == 1:
                    kind -= 1
                if kind_list[card_list[diff[1]][1]] == 0:
                    kind += 1
                kind_list[card_list[diff[1]][0]] -= 1
                kind_list[card_list[diff[1]][1]] += 1
                card_list[diff[1]] = swap(card_list[diff[1]])
                flag = True
                if kind == K:
                    card_list_s = list(card_list)
                    kind_list_s = list(kind_list_s)
        if kind < K:
            card_list = list(card_list_s)
            kind_list_s = list(kind_list_s)
            kind = K
    total = 0
    for i, card in enumerate(card_list):
        total += card[0]
    print(total)



def swap(a):
    return (a[1], a[0])


if __name__ == '__main__':
    main()
