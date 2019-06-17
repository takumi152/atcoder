import math

def main():
    buf = input()
    N = int(buf)
    AB = []
    for i in range(N):
        buf = input()
        buflist = buf.split()
        AB.append((int(buflist[0]), int(buflist[1])))
    strongest = 0
    for i, _ in enumerate(AB):
        if i == strongest:
            continue
        battle = math.ceil(AB[strongest][0] / AB[i][1]) - math.ceil(AB[i][0] / AB[strongest][1])
        if battle > 0:
            pass
        elif battle < 0:
            strongest = i
        else:
            strongest = i+1
    if strongest >= N:
        print(-1)
        return
    for i, _ in enumerate(AB):
        if i == strongest:
            continue
        battle = math.ceil(AB[strongest][0] / AB[i][1]) - math.ceil(AB[i][0] / AB[strongest][1])
        if battle > 0:
            pass
        else:
            print(-1) # defeat
            return
    print(strongest+1)

if __name__ == '__main__':
    main()
