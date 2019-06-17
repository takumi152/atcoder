import string

def main():
    buf = input()
    buflist = buf.split()
    buflist = list(map(int, buflist))
    itemstr = string.ascii_uppercase[:buflist[0]]
    if len(itemstr) > 5:
        ansstr = mergesort(itemstr[:int(len(itemstr)/2)], itemstr[int(len(itemstr)/2):])
    else:
        ansstr = tablesort(itemstr)
    print("!", ansstr)

def mergesort(left, right):
    if len(left) == 0:
        return right
    elif len(left) == 1 and len(right) == 1:
        comp = query(left, right)
        if comp == '<':
            return left + right
        else:
            return right + left
    else:
        leftans = mergesort(left[:int(len(left)/2)], left[int(len(left)/2):])
        rightans = mergesort(right[:int(len(right)/2)], right[int(len(right)/2):])
        ans = ""
        while len(leftans) > 0 and len(rightans) > 0:
            comp = query(leftans[0], rightans[0])
            if comp == '<':
                ans += leftans[0]
                leftans = leftans[1:]
            else:
                ans += rightans[0]
                rightans = rightans[1:]
        return ans + leftans + rightans

def tablesort(itemstr):
    pass

def query(alpha, beta):
    print("?", alpha, beta, flush=True)
    comp = input()
    return comp

if __name__ == '__main__':
    main()
