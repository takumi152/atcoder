
def main():
    buf = input()
    N = int(buf)
    buf = input()
    buflist = buf.split()
    x = list(map(int, buflist))
    x = list(sorted(x))
    diff_list = []
    diff_sum = []
    for i in range(1, len(x)):
        diff_list.append(x[i] - x[i-1])
        if i == 1:
            diff_sum.append(x[i] - x[i-1])
        else:
            diff_sum.append(x[i] - x[i-1] + diff_sum[-1])
    diff_total = sum(diff_sum)
    total = 0
    penalty = 0
    for i in range(len(diff_sum)):
        total += diff_total - penalty
        penalty += diff_list[i] * (len(diff_sum) - i)
    print(total)

if __name__ == '__main__':
    main()
