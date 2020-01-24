
def main():
    buf = input()
    s = buf
    for i in range(3):
        if s[i] not in {'0','1','2','3','4','5','6','7','8','9'}:
            print('error')
            return
    print(int(s) * 2)

if __name__ == '__main__':
    main()
