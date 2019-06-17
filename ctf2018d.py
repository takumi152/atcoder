
def main():
    buf = input()
    S = buf
    count = 0
    earliest = S[0]
    for i, c in enumerate(S):
        if c <= earliest:
            count += 1
            earliest = c
    print(count)

if __name__ == '__main__':
    main()
