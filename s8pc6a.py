import sys
import math

def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    T = int(buflist[1])
    buf = input()
    buflist = buf.split()
    A = list(map(int, buflist))
    arrival = 0
    for i in A:
        arrival += i
    timeleap = math.ceil(arrival / T)
    print(timeleap)

if __name__ == '__main__':
    main()
