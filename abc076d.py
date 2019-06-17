
def main():
    buf = input()
    N = int(buf)
    buf = input()
    buflist = buf.split()
    t = list(map(int, buflist))
    buf = input()
    buflist = buf.split()
    v = list(map(int, buflist))
    t.append(0)
    v.append(0)
    for i, _ in enumerate(t): # double time and speed to make calculation easier
        t[i] *= 2
        v[i] *= 2
    time = 0
    last_speed = 0
    speed = 0
    distance = 0.0
    point = 0
    spd_time_list = []
    for i, _ in enumerate(t):
        spd_time_list.append((v[i], (0 if not spd_time_list else t[i-1] + spd_time_list[-1][1])))
    while time < spd_time_list[-1][1]:
        last_speed = speed
        brake = False
        for i in range(point+1, len(spd_time_list)):
            if speed >= spd_time_list[i][0] + (spd_time_list[i][1] - time):
                brake = True
                break
        if brake:
            speed -= 1
        elif speed < spd_time_list[point][0]:
            speed += 1
        distance += (last_speed + speed) / 2
        time += 1
        if time >= spd_time_list[point+1][1]:
            point += 1
    distance /= 4 # roll back time and speed (and so the distance)
    print(distance)

if __name__ == '__main__':
    main()
