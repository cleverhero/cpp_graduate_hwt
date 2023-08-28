import os
import sys
import random


def generate_rank_and_select_test(insert_c, select_c, rank_c, shuffle=False):
    input, output = '', ''
    arr = [i for i in range(1, insert_c + 1)]

    if shuffle:
        random.shuffle(arr)
    input = ' '.join(f'k {i}' for i in arr)

    random.shuffle(arr)
    to_select = arr[:select_c]
    input = input + ' ' + ' '.join(f'm {i}' for i in to_select)
    output = ' '.join(f'{i}' for i in to_select)

    random.shuffle(arr)
    to_rank = arr[:rank_c]
    input = input + ' ' + ' '.join(f'n {i}' for i in to_rank)
    output = output + ' ' + ' '.join(f'{i - 1}' for i in to_rank)

    return input, output


def make_rank_and_select_test(test_name, insert_c, select_c, rank_c, shuffle):
    test_path = f'./{test_name}'
    fi, fo = open(test_path + '.in', 'w+'), open(test_path + '.out', 'w+')
    input, output = generate_rank_and_select_test(insert_c, select_c, rank_c, shuffle)
    print(input, file=fi)
    print(output, file=fo)


def random_range(minv, maxv):
    l = random.randrange(minv, maxv)
    r = random.randrange(l, maxv)

    return (l, r)


def ans_for_query(query, minv, maxv):
    query = list(query)

    if query[0] >= maxv: return 0
    if query[1] <= minv: return 0
    if query[1] - query[0] < 0: return 0

    if query[0] < minv: query[0] = minv
    if maxv < query[1]: query[1] = maxv

    if query[0] % 2 == 0: query[0] += 1
    if query[1] % 2 == 0: query[1] += 1

    return (query[1] - query[0]) // 2


def generange_range_find_test(insert_c, query_c, shuffle=False):
    input, output = '', ''
    arr = [2*i + 1 for i in range(insert_c)]

    if shuffle:
        random.shuffle(arr)
    input = str(insert_c) + ' '
    input += ' '.join(map(str, arr)) + ' '

    queries = [
        random_range(-insert_c, 3 * insert_c) 
        for i in range(query_c)
    ]
    input += str(query_c) + ' '
    input += ' '.join(f'{q[0]} {q[1]}' for q in queries)
    output = ' '.join(
        f'{ans_for_query(q, 0, 2*insert_c)}' 
        for q in queries
    )

    return input, output


def make_range_find_test(test_name, insert_c, query_c, shuffle):
    test_path = f'./{test_name}'
    fi, fo = open(test_path + '.in', 'w+'), open(test_path + '.out', 'w+')
    input, output = generange_range_find_test(insert_c, query_c, shuffle)
    print(input, file=fi)
    print(output, file=fo)


if __name__ == '__main__':
    make_rank_and_select_test('rank_select_test', 500_000, 2_000, 300, True)
    make_range_find_test('range_find_test', 500_000, 200, True)
