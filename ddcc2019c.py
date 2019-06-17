import math

MOD = 1000000007

def main():
    buf = input()
    N = int(buf)
    pattern_count = 0
    count_history = [0]
    for i in range(1, N+1):
        count = (int(i ** 10) * int((N // i) ** 10) - count_history[-1]) % MOD
        pattern_count = (pattern_count + count) % MOD
        count_history.append(int(i ** 10) * int((N // (i + 1)) ** 10) % MOD)
    print(pattern_count)

if __name__ == '__main__':
    main()
