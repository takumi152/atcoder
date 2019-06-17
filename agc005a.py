
def main():
    buf = input()
    X = buf
    length = len(X)
    S_count = 0
    for i in X:
        if i == 'S':
            S_count += 1
        else: # i == 'T'
            if S_count > 0:
                S_count -= 1
                length -= 2
    print(length)

if __name__ == '__main__':
    main()
