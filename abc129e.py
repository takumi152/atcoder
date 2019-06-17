
MOD = int(1e9) + 7

def main():
    buf = input()
    L = list(map(int, list(buf)))
    L.reverse()
    # 片側が0なら自明
    # 繰り上がりが発生しない場合は条件を満たす
    # 1つの数値に対して2^(1の数)通り
    bit1 = 2
    bit_list = []
    pow2 = [1]
    for i in range(len(L)):
        bit_list.append(bit1)
        bit1 = (bit1 * 3) % MOD
        pow2.append((pow2[-1] * 2) % MOD)
    pattern = 1
    print(L)
    print(bit_list)
    for i, x in enumerate(L):
        if x == 0:
            pattern = (pattern - bit_list[i]) % MOD
        else: # x == 1
            pattern = (pattern + bit_list[i]) % MOD
    print(pattern)

#2
#2 4
#2 4 4 8
#2 4 4 8 4 8 8 16

if __name__ == '__main__':
    main()
