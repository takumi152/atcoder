import math

MOD = int(10e9) + 7

def main():
    buf = input()
    T = int(buf)
    buf = input()
    buflist = buf.split()
    a = list(map(int, buflist))
    pattern_total = 0
    for i in range(T):
        pattern_count = 0
        pattern_list = [0]
        for j in range(600):
            pattern_list.append(0)
        for j in range(1, a[i]+1):
            if j % 2 == 0:
                pattern_list[j] = 1
        pattern_count += a[i].bit_length()
        for j in range(i+1, T):
            new_pattern_list = [0]
            for k in range(600):
                new_pattern_list.append(0)
            #for k in range(0, a[j]+1):
            #    new_pattern_list[k] = 1
            for k in range(1, 300+1):
                if pattern_list[k*2] > 0:
                    for l in range(k+1, a[j]+1+k):
                        new_pattern_list[l] += pattern_list[k*2]
            factor = 1
            while factor < 600:
                pattern_count += new_pattern_list[factor]
                factor *= 2
            pattern_count = pattern_count % MOD
            pattern_list = new_pattern_list
        pattern_total = (pattern_total + pattern_count) % MOD
    print(pattern_total)



if __name__ == '__main__':
    main()
