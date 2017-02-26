def find_next(greedy_list, dyn_list, t, skill):
    if t == 0:
        result = findListMax(dyn_list, skill)
        if result:
            dyn_list.remove(result)
            return result
    else:
        result = findListMax(greedy_list, skill)
        if result:
            greedy_list.remove(result)
            return result
    return None


# We have as a precondition that li contains tuples of size 2. As soon as we find a first value (== skill requirement)
# less than value (== skill level) we return the associated tuple
def findListMax(li, value):
    for i in iter(li):
        first, second = i
        if (first <= value):
            return i
    return None


def iterative_find(greedy_list, dyn_list, t, skill):
    challenges = 0
    next = find_next(greedy_list, dyn_list, t, skill)
    while next:
        challenges += 1
        t = (1 - t)
        skill += next[1]
        next = find_next(greedy_list, dyn_list, t, skill)
    return challenges


n, h = map(int, input().split())

dyn_list = []
greedy_list = []
total_list = []
for _ in range(n):
    (t, d, s) = map(int, input().split())
    total_list.append((t, d, s))
    if t == 0:
        dyn_list.append((d, s))
    else:
        greedy_list.append((d, s))


greedy_copy = list(greedy_list)
dyn_copy = list(dyn_list)

result_one = iterative_find(greedy_list, dyn_list, 1, h)
result_zero = iterative_find(greedy_copy, dyn_copy, 0, h)

print(result_one if result_one > result_zero else result_zero)
