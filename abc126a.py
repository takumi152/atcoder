
def main():
    buf = input()
    buflist = list(buf.split())
    N = int(buflist[0])
    K = int(buflist[1])
    buf = input()
    S = list(buf)
    S[K-1] = S[K-1].lower()
    print(''.join(S))


if __name__ == '__main__':
    main()
