import math

def main():
    buf = input()
    buflist = buf.split()
    H = int(buflist[0])
    W = int(buflist[1])
    K = int(buflist[2]) - 1 # zero indexing

    pattern_list = []
    create_pattern(pattern_list, [], W-1, 0)
    prob = [1]
    for i in range(W-1):
        prob.append(0)
    for i in range(H):
        new_prob = []
        for j in range(W):
            new_prob.append(0)
        for j in range(W):
            for k in pattern_list:
                if j in k:
                    new_prob[j+1] += prob[j]
                elif j-1 in k:
                    new_prob[j-1] += prob[j]
                else:
                    new_prob[j] += prob[j]
        prob = list(new_prob)
    print(prob[K] % 1000000007)


def create_pattern(pattern_list, pattern, W, C):
    pattern_list.append(pattern)
    if len(pattern) == 0:
        for i in range(W):
            new_pattern = list(pattern)
            new_pattern.append(i)
            create_pattern(pattern_list, new_pattern, W, i)
    else:
        for i in range(C+2, W):
            new_pattern = list(pattern)
            new_pattern.append(i)
            create_pattern(pattern_list, new_pattern, W, i)


if __name__ == '__main__':
    main()
