def main():
    abcd = input()

    a = int(abcd[0])
    b = int(abcd[1])
    c = int(abcd[2])
    d = int(abcd[3])
    if (a + b + c + d == 7):
        print(str(a)+'+'+str(b)+'+'+str(c)+'+'+str(d)+'=7')
    elif (a + b + c - d == 7):
        print(str(a)+'+'+str(b)+'+'+str(c)+'-'+str(d)+'=7')
    elif (a + b - c + d == 7):
        print(str(a)+'+'+str(b)+'-'+str(c)+'+'+str(d)+'=7')
    elif (a + b - c - d == 7):
        print(str(a)+'+'+str(b)+'-'+str(c)+'-'+str(d)+'=7')
    elif (a - b + c + d == 7):
        print(str(a)+'-'+str(b)+'+'+str(c)+'+'+str(d)+'=7')
    elif (a - b + c - d == 7):
        print(str(a)+'-'+str(b)+'+'+str(c)+'-'+str(d)+'=7')
    elif (a - b - c + d == 7):
        print(str(a)+'-'+str(b)+'-'+str(c)+'+'+str(d)+'=7')
    elif (a - b - c - d == 7):
        print(str(a)+'-'+str(b)+'-'+str(c)+'-'+str(d)+'=7')

if __name__ == '__main__':
    main()
