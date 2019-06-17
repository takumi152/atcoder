
MOD = int(1e9) + 7

def main():
    buf = input()
    buflist = buf.split()
    H = int(buflist[0])
    W = int(buflist[1])
    S = []
    for i in range(H):
        buf = input()
        S.append(buf)
    lightmap_h = []
    for i in range(H):
        lightmap_h.append([])
        for j in range(W):
            lightmap_h[i].append(0)
    lightmap_v = []
    for j in range(H):
        lightmap_v.append([])
        for i in range(W):
            lightmap_v[j].append(0)
    for i in range(H):
        light_count = 0
        last_pointer = 0
        for j in range(W):
            if S[i][j] == '.':
                light_count += 1
            else:
                for k in range(last_pointer, j):
                    lightmap_h[i][k] = light_count
                light_count = 0
                last_pointer = j+1
        for k in range(last_pointer, W):
            lightmap_h[i][k] = light_count
    for j in range(W):
        light_count = 0
        last_pointer = 0
        for i in range(H):
            if S[i][j] == '.':
                light_count += 1
            else:
                for k in range(last_pointer, i):
                    lightmap_v[k][j] = light_count
                light_count = 0
                last_pointer = i+1
        for k in range(last_pointer, H):
            lightmap_v[k][j] = light_count
    maximum = 0
    for i in range(H):
        for j in range(W):
            hor = lightmap_h[i][j]
            vert = lightmap_v[i][j]
            if hor == 0 or vert == 0:
                continue
            light_distance = hor + vert - 1
            if light_distance > maximum:
                maximum = light_distance
    print(maximum)

if __name__ == '__main__':
    main()
