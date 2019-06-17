import math

def main():
    buf = input()
    buflist = list(buf.split())
    N = int(buflist[0])
    K = int(buflist[1])
    divisor = N
    dividend = 0
    for i in range(N):
        dividend += 2 ** min(0, math.floor(math.log2(i+1) - math.log2(K)))
    print(dividend / divisor)

if __name__ == '__main__':
    main()
