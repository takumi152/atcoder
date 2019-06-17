
def main():
    buf = input()
    S = buf
    days = len(S)
    wins = 0
    for i in S:
        if i == 'o':
            wins += 1
    if wins >= days - 7:
        print('YES')
    else:
        print('NO')

if __name__ == '__main__':
    main()
