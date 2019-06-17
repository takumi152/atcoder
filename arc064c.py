
def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    x = int(buflist[1])
    buf = input()
    buflist = buf.split()
    a = list(map(int, buflist))
    eaten = 0
    for i in range(1,len(a)):
        need_to_eat = a[i-1] + a[i] - x
        if need_to_eat <= 0:
            continue
        else:
            eaten += need_to_eat
            if need_to_eat > a[i]:
                a[i-1] -= need_to_eat - a[i]
                need_to_eat = a[i]
            a[i] -= need_to_eat
    print(eaten)

if __name__ == '__main__':
    main()
