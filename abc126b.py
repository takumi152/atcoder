
def main():
    buf = input()
    S = buf
    first = int(S[0:2])
    second = int(S[2:4])
    if first == 0 and second == 0:
        print('NA')
        return
    elif 1 <= first and first <= 12:
        if 1 <= second and second <= 12:
            print('AMBIGUOUS')
            return
        else:
            print('MMYY')
            return
    elif 1 <= second and second <= 12:
        print('YYMM')
        return
    else:
        print('NA')
        return

if __name__ == '__main__':
    main()
