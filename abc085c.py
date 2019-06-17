
def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    Y = int(buflist[1])
    for u10000 in range(N + 1):
        if u10000 * 10000 > Y:
            break
        elif u10000 * 10000 + (N - u10000) * 5000 < Y:
            continue
        else:
            for u5000 in range(N - u10000 + 1):
                if u10000 * 10000 + u5000 * 5000 > Y:
                    break
                else:
                    u1000 = N - u10000 - u5000
                    if u10000 * 10000 + u5000 * 5000 + u1000 * 1000 == Y:
                        print(u10000, u5000, u1000)
                        return
    print(-1, -1, -1)                   

if __name__ == '__main__':
    main()
