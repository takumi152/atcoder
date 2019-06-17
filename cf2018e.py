# WIP

def main():
    buf = input()
    N = int(buf)
    Q = 0
    op = []
    denominator = 1
    for i in range(N):
        denominator = lcm(denominator, i+1)
    numerator_list = []
    for i in range(N):
        numerator_list.append(int(denominator / (i+1)))
    numerator = 0
    op_count = []
    for i in range(N):
        op_count.append(0)
    i = 0
    j = N-1
    while i != j:



# 最大公約数 & 最小公倍数
def gcd(a, b):
    while b > 0:
        a, b = b, a % b
    return int(a)

def lcm(a, b):
    return int(a * b / gcd(a, b))

if __name__ == '__main__':
    main()
