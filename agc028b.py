# WIP

def main():
    buf = input()
    N = int(buf)
    buf = input()
    buflist = buf.split()
    A = list(map(int, buflist))
    # initialize cost table
    cost_table = []
    for i in range(N):
        cost_table.append([])
    # create cost table
    for i in range(N):
        for j in range(N):
            if i <= j:
                cost_table[i].append(sum(A[i:j+1])) # calculate cost of a slice
            else:
                cost_table[i].append(0) # impossible
    # initialize appearance table
    appearance_table = []
    for i in range(N):
        appearance_table.append([])
        for j in range(N):
            appearance_table[i].append(0)
    # create appearance table
    # start from 1 element with (N-1)!(=N!/N) appearance
    # N : N!
    # N-1 : (N-1)! * 2
    # N-2 : (N-2)! * ((2 * 2) + 2 * 2 ^ 1)
    # N-3 : (N-3)! * ((4 * 2) + (2 * 2) + 2)
    for i in range(N):
        appearance_table[i][j] = factorial(N-1)

    # calc total cost
    result = 0
    for i in range(N):
        for j in range(N):
            result += cost_table[i][j] * appearance_table[i][j]
    result = result % int(10e9 + 7)
    print(cost_table)
    print(appearance_table)
    print(result)

def factorial(n):
    if n == 0:
        return 1
    else:
        return n * factorial(n - 1)

def power_sum(n):
    if n == 0:
        return 1
    elif n == 1:
        return 2
    else:
        return 2 ** n + power_sum(n - 1)

if __name__ == '__main__':
    main()
