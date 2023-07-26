import os
import sys
import random


def generate(insert_c, select_c, rank_c, shuffle=False):
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


def make_test(test_name, insert_c, select_c, rank_c, shuffle):
    test_path = f'./{test_name}'
    fi, fo = open(test_path + '.in', 'w+'), open(test_path + '.out', 'w+')
    input, output = generate(insert_c, select_c, rank_c, shuffle)
    print(input, file=fi)
    print(output, file=fo)


if __name__ == '__main__':
    make_test('rank_select_test', 500_000, 2_000, 300, True)
