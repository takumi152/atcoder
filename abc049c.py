import sys

def main():
    buf = input()
    S = buf
    result = erase(S)
    if result:
        print("YES")
    else:
        print("NO")

def erase(item):
    S = item
    if len(S) == 0:
        return True
    strlist = ["dream", "dreamer", "erase", "eraser"]
    while any(list(map(lambda i: S[-len(i):] == i, strlist))):
        for i in strlist:
            if S[-len(i):] == i:
                S = S[:-len(i)]
                if len(S) == 0:
                    return True
                break
    return False

if __name__ == '__main__':
    main()
