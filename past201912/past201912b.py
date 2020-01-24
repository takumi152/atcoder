
def main():
    buf = input()
    n = int(buf)
    a = []
    for i in range(n):
        buf = input()
        a.append(int(buf))

    ans = []
    for i in range(n-1):
        if a[i] < a[i+1]:
            ans.append('up ' + str(a[i+1] - a[i]))
        elif a[i] > a[i+1]:
            ans.append('down ' + str(a[i] - a[i+1]))
        else:
            ans.append('stay')

    for i in range(n-1):
        print(ans[i])

if __name__ == '__main__':
    main()
