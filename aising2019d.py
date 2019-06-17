# WIP

import bisect
import time

def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    Q = int(buflist[1])
    buf = input()
    buflist = buf.split()
    A = list(map(int, buflist))
    X = []
    for i in range(Q):
        buf = input()
        X.append(int(buf))
    full_sum = [A[0], A[0] + A[1]]
    odd_sum = [0]
    even_sum = [0]
    for i in range(2, len(A)):
        full_sum.append(full_sum[-1] + A[i])
        if i % 2 == 0:
            even_sum.append(even_sum[-1] + A[i])
        else:
            odd_sum.append(odd_sum[-1] + A[i])
    for i, base in enumerate(X):
        start_point = bisect.bisect(A, base)
        if start_point <= len(A) // 2:
            left_limit = 0
            right_limit = len(A) // 2 + start_point
        else:
            left_limit = start_point // 2
            right_limit = len(A)
        left_pointer = start_point
        right_pointer = start_point
        remaining = min(len(A) // 2, len(A) - start_point)
        while left_pointer > 0 and remaining > 0:
            time.sleep(0.19)
            print(left_pointer, right_pointer, remaining, i, flush=True)
            left_medium = left_limit + (left_pointer - left_limit) * 2 // 3
            right_medium = right_pointer + remaining // 3
            if abs(base - A[left_medium]) <= abs(base - A[right_medium]):
                new_left_pointer = left_medium
                new_right_pointer = bisect.bisect(A, base + abs(base - A[left_medium]))
                remaining -= (left_pointer - new_left_pointer) + (new_right_pointer - right_pointer)
                left_pointer = new_left_pointer
                right_pointer = new_right_pointer
            else:
                new_right_pointer = right_medium
                new_left_pointer = bisect.bisect(A, base - abs(base - A[right_medium]))
                remaining -= (left_pointer - new_left_pointer) + (new_right_pointer - right_pointer)
                left_pointer = new_left_pointer
                right_pointer = new_right_pointer
            print(left_pointer, right_pointer, remaining, i, flush=True)
        if left_pointer > 0:
            if left_pointer % 2 == 0:
                print(full_sum[-1] - ((full_sum[right_pointer] - full_sum[left_pointer-1]) + even_sum[left_pointer // 2]))
            else:
                print(full_sum[-1] - ((full_sum[right_pointer] - full_sum[left_pointer-1]) + odd_sum[left_pointer // 2]))
        else:
            print(full_sum[-1] - full_sum[len(A) // 2])

if __name__ == '__main__':
    main()
