
def main():
    a = input()
    bc = input()
    s = input()
    bc_split = bc.split()
    bc_split = list(map(int, bc_split))
    sum = int(a)
    for i in bc_split:
        sum += i
    print(sum, s)

if __name__ == '__main__':
    main()
