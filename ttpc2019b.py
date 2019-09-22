import re

def main():
    buf = input()
    N = int(buf)
    S = []
    for i in range(N):
        buf = input()
        S.append(buf)
    ans = []
    for x in S:
        found = re.match('[a-z]*okyo[a-z]*ech[a-z]*', x)
        if found:
            ans.append('Yes')
        else:
            ans.append('No')
    for x in ans:
        print(x)

if __name__ == '__main__':
    main()
