def main():
    n = int(input())
    a = list(map(int, input().split()))

    modulo_list = [None for _ in range(200)]
    for i in range(1, 2 ** n):
        now_list = []
        p = 0
        x = i
        while x > 0:
            if x & 1:
                now_list.append(p)
            p += 1
            x //= 2
        total = 0
        for x in now_list:
            total += a[x]
        if modulo_list[total % 200] == None:
            modulo_list[total % 200] = now_list
        else:
            print('Yes')
            print(len(modulo_list[total % 200]), ' '.join([str(x+1) for x in modulo_list[total % 200]]))
            print(len(now_list), ' '.join([str(x+1) for x in now_list]))
            return

    print('No')

if __name__ == '__main__':
    main()
