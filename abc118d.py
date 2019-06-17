# WA Edition

match_table = [0, 2, 5, 5, 4, 5, 6, 3, 7, 6]
search_table = [1, 7, 4, 5, 3, 2, 9, 6, 8]

def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    M = int(buflist[1])
    buf = input()
    buflist = buf.split()
    A = list(sorted(list(map(int, buflist))))
    # remove unnecessary number
    if 1 in A:
        if 4 in A:
            A.remove(4)
        if 6 in A:
            A.remove(6)
        if 9 in A:
            A.remove(9)
    if 7 in A:
        if 6 in A:
            A.remove(6)
        if 9 in A:
            A.remove(9)
    if 2 in A and (3 in A or 5 in A):
        A.remove(2)
    if 3 in A and 5 in A:
        A.remove(3)
    if 6 in A and 9 in A:
        A.remove(6)
    # main part
    for i in search_table:
        if i in A:
            extra_number, penalty = generate_extra_number(A, i, N % match_table[i], N)
            num_to_print = ""
            for j in range(N // match_table[i] - penalty):
                num_to_print += str(i)
            if int(extra_number[0]) > i:
                num_to_print = extra_number + num_to_print
            else:
                num_to_print = num_to_print + extra_number
            print(num_to_print)
            print(len(num_to_print))
            return

def generate_extra_number(A, main_num, remainder, N):
    if remainder == 0:
        return str(main_num), 1
    d = match_table[main_num]
    best = 0
    penalty = 99
    digit = 0
    while digit < 7:
        digit += 1
        for i in num_generator(A, digit, ""):
            extra_sum = sum([match_table[int(x)] for x in i])
            if (remainder - extra_sum) % d == 0 and extra_sum <= N:
                cand_best = int(i)
                cand_penalty = -(remainder - extra_sum) // d
                print(best, cand_best, cand_penalty, cand_penalty - len(i), penalty - len(str(best)))
                if cand_penalty - len(i) < penalty - len(str(best)):
                    best = cand_best
                    penalty = cand_penalty
                elif cand_penalty - len(i) == penalty - len(str(best)):
                    if cand_best / exp(len(i)) > best / exp(len(str(best))) :
                        best = cand_best
                        penalty = cand_penalty
    return str(best), penalty

def num_generator(A, digit, num):
    if digit == 0:
        yield num
    else:
        for i in reversed(A):
            yield from num_generator(A, digit-1, num + str(i))

def exp(x):
    n = 1
    for i in range(x):
        n *= 10
    return n

if __name__ == '__main__':
    main()
