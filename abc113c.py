
def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    M = int(buflist[1])
    P = []
    Y = []
    for i in range(M):
        buf = input()
        buflist = buf.split()
        P.append(int(buflist[0]))
        Y.append(int(buflist[1]))
    citylist = []
    for i in range(len(P)):
        citylist.append([i,P[i],Y[i]])
    citylist = list(sorted(citylist, key=lambda x:x[2]))
    citycount = []
    for i in range(100000):
        citycount.append(1)
    for i in range(len(citylist)):
        citylist[i].append(citycount[citylist[i][1]-1])
        citycount[citylist[i][1]-1] += 1
    citylist = list(sorted(citylist, key=lambda x:x[0]))
    for i in range(len(citylist)):
        print("{:06}".format(citylist[i][1])+"{:06}".format(citylist[i][3]))


if __name__ == '__main__':
    main()
