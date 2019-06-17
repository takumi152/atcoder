import sys

def main():
    sys.setrecursionlimit(1000000) # recursion limit hack
    buf = input()
    N = int(buf)
    buf = input()
    buflist = buf.split()
    h = list(map(int, buflist))
    done = []
    height = []
    for i in range(N):
        if h[i] == 0:
            done.append(True)
        else:
            done.append(False)
        height.append(0)
    print(recursion_water(h, done, height, 0))

def recursion_water(h, done, height, count):
    for i in range(len(h)):
        if done[i]:
            continue
        else:
            count += 1
            for j in range(i, len(h)):
                if not done[j]:
                    height[j] += 1
                    if height[j] == h[j]:
                        done[j] = True
                else:
                    break
            return recursion_water(h, done, height, count)
    return count

if __name__ == '__main__':
    main()
