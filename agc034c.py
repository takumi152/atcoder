import math

def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    X = int(buflist[1])
    iblu = []
    for i in range(N):
        buf = input()
        buflist = buf.split()
        iblu.append((i, int(buflist[0]), int(buflist[1]), int(buflist[2])))
    aoki_score = 0
    takahashi_score = 0
    for x in iblu:
        aoki_score += x[1] * x[2]
    score_gain = []
    for x in iblu:
        score_gain.append((x[0], X * x[3] - x[1] * (x[3] - x[2])))
    score_gain.sort(reverse=True, key=lambda x: x[1])
    study_hour = 0
    for i, x in enumerate(score_gain):
        new_takahashi_score = takahashi_score + x[1]
        if new_takahashi_score < aoki_score:
            study_hour += X
            takahashi_score = new_takahashi_score
        else: # new_takahashi_score >= aoki_score
            score_needed = aoki_score - takahashi_score
            study_minimum = X
            for j in range(i, N):
                penalty = iblu[score_gain[j][0]][1] * (iblu[score_gain[j][0]][3] - iblu[score_gain[j][0]][2])
                st = min(math.ceil(score_needed / iblu[score_gain[j][0]][2]), math.ceil((score_needed + penalty) / iblu[score_gain[j][0]][3]))
                if st < study_minimum:
                    study_minimum = st
            for j in range(i):
                score_needed = aoki_score - new_takahashi_score + score_gain[j][1]
                penalty = iblu[score_gain[j][0]][1] * (iblu[score_gain[j][0]][3] - iblu[score_gain[j][0]][2])
                st = min(math.ceil(score_needed / iblu[score_gain[j][0]][2]), math.ceil((score_needed + penalty) / iblu[score_gain[j][0]][3]))
                if st < study_minimum:
                    study_minimum = st
            #penalty = iblu[x[0]][1] * (iblu[x[0]][3] - iblu[x[0]][2])
            #study_hour += min(math.ceil(score_needed / iblu[x[0]][2]), math.ceil((score_needed + penalty) / iblu[x[0]][3]))
            #takahashi_score += iblu[x[0]][2] * math.ceil(score_needed / iblu[x[0]][2]) if math.ceil(score_needed / iblu[x[0]][2]) < math.ceil((score_needed + penalty) / iblu[x[0]][3]) else iblu[x[0]][3] * math.ceil((score_needed + penalty) / iblu[x[0]][3]) - penalty
            study_hour += study_minimum
            takahashi_score = new_takahashi_score
        if takahashi_score >= aoki_score:
            break
    print(study_hour)


if __name__ == '__main__':
    main()
