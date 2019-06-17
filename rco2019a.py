import time
import random
import math

def main():
    t_start = time.time()
    buf = input()
    N = int(buf) # = 200
    town_list = []
    for i in range(N):
        buf = input()
        buflist = buf.split()
        town_list.append((int(buflist[0]), int(buflist[1])))
    visit_order = []
    for i in range(N):
        visit_order.append(i)
    dist_list = []
    for i in range(N):
        if i != N - 1:
            dist_list.append(calc_distance(town_list[i], town_list[i+1]))
        else:
            dist_list.append(calc_distance(town_list[i], town_list[0]))
    dist_sum = sum(dist_list)
    variance, dist_sum = calc_variance(town_list, visit_order, dist_list, dist_sum, 0, 0, N)
    limit = 1.9
    while time.time() - t_start < limit:
        eps = (1 - (time.time() - t_start) / limit) ** 2
        # swap
        a = random.randrange(N)
        b = random.randrange(N)
        visit_order[a], visit_order[b] = visit_order[b], visit_order[a]
        current_variance, current_sum = calc_variance(town_list, visit_order, dist_list, dist_sum, a, b, N)
        if current_variance < variance:
            variance = current_variance
            dist_sum = current_sum
        else:
            if random.random() > math.exp((variance - current_variance) / eps / 100):
                # rollback
                visit_order[a], visit_order[b] = visit_order[b], visit_order[a]
                calc_variance(town_list, visit_order, dist_list, current_sum, b, a, N)
            else:
                variance = current_variance
                dist_sum = current_sum
        #print(variance, current_variance)
    for i in range(N):
        print(visit_order[i])

def calc_distance(x, y):
    d0 = x[0] - y[0]
    d1 = x[1] - y[1]
    return math.sqrt(d0 * d0 + d1 * d1)

def calc_variance(town_list, visit_order, dist_list, dist_sum, a, b, N):
    a_before_to = a-1 if a > 0 else N-1
    b_before_to = b-1 if b > 0 else N-1
    # recalc a
    i_a_before = visit_order[a-1] if a > 0 else visit_order[-1]
    i_a_after = visit_order[a+1] if a < N-1 else visit_order[0]
    dist_a_before = calc_distance(town_list[visit_order[a]], town_list[i_a_before])
    dist_a_after = calc_distance(town_list[visit_order[a]], town_list[i_a_after])
    dist_sum += dist_a_before + dist_a_after - dist_list[a_before_to] - dist_list[a]
    # recalc b
    i_b_before = visit_order[b-1] if b > 0 else visit_order[-1]
    i_b_after = visit_order[b+1] if b < N-1 else visit_order[0]
    dist_b_before = calc_distance(town_list[visit_order[b]], town_list[i_b_before])
    dist_b_after = calc_distance(town_list[visit_order[b]], town_list[i_b_after])
    dist_sum += dist_b_before + dist_b_after - dist_list[b_before_to] - dist_list[b]
    # update dist_list
    dist_list[a_before_to] = dist_a_before
    dist_list[a] = dist_a_after
    dist_list[b_before_to] = dist_b_before
    dist_list[b] = dist_b_after
    # calc variance
    average = dist_sum / N
    var_sum = 0.0
    for i in visit_order:
        v = dist_list[i] - average
        var_sum += v * v
    return var_sum / N, dist_sum

def test():
    t_start = time.time()
    buf = input()
    N = int(buf) # = 200
    town_list = []
    for i in range(N):
        buf = input()
        buflist = buf.split()
        town_list.append((int(buflist[0]), int(buflist[1])))
    visit_order = []
    for i in range(N):
        visit_order.append(i)
    dist_list = []
    for i in range(N):
        if i != N - 1:
            dist_list.append(calc_distance(town_list[i], town_list[i+1]))
        else:
            dist_list.append(calc_distance(town_list[i], town_list[0]))
    dist_sum = sum(dist_list)
    for i in range(N):
        print(calc_variance(town_list, visit_order, dist_list, dist_sum, i, i, N))

if __name__ == '__main__':
    main()
