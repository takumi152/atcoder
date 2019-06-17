import math

def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    M = int(buflist[1])
    max = 1
    for i in get_divisor_list(M):
        # Mがiで割り切れる、かつ割ったときの値がN以上なら可能なはず
        if M % i == 0 and M / i >= N:
            max = i
    print(max)

# 約数のリストを取得
def get_divisor_list(num):
    div_list = [1, num]
    limit = int(math.sqrt(num))
    for i in range(2, limit+1):
        if num % i == 0:
            div_list.append(i)
            div_list.append(int(num / i))
    return sorted(div_list)

if __name__ == '__main__':
    main()
