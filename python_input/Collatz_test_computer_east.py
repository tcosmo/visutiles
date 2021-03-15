from subprocess import Popen, PIPE
import sys
import json
from python_input.geometry import np, WEST, SOUTH, EAST, NORTH, CENTER
from python_input.Collatz_inputter import get_edge, get_edge_v, init_json_dict, \
    get_edges_write_word_then_move, string_to_colors
from python_input.Collatz_computer_east import rule_124_computer

import tqdm
import numpy as np
import itertools
import json


def rule124(x, y, z):
    return (~x & y) | (y ^ z)


def get_rule_124_matrix(input_string):
    matrix = -1 * \
        np.ones((len(input_string), int(
            np.ceil(len(input_string)/2))), dtype=np.int64)

    for i, c in enumerate(input_string):
        matrix[i, -1] = int(c)

        for j in range(2, len(matrix[0])+1):
            for i in range(len(input_string)):
                if i < j-1 or i > len(input_string)-j:
                    continue

                z = matrix[i-1, -1*j+1]
                y = matrix[i, -1*j+1]
                x = matrix[i+1, -1*j+1]
                matrix[i, -1*j] = rule124(x, y, z)

    return matrix


def getstatusoutput(cmd, input):
    """Return (status, output) of executing cmd in a shell."""
    import os
    pipe = os.popen('{ ' + cmd + '; }', 'rw')
    pipe.write(input)
    text = pipe.read()
    sts = pipe.close()
    if sts is None:
        sts = 0
    if text[-1:] == '\n':
        text = text[:-1]
    return sts, text


def getoutput(cmd, input):
    """Return output (stdout or stderr) of executing cmd in a shell."""
    return getstatusoutput(cmd, input)[1]


def create_sh_all_nbit_strings(n=10):
    # create bash script
    import os
    string_pool = list(map(lambda x: "".join(
        x), itertools.product(["0", "1"], repeat=n)))

    for s in string_pool:
        the_mat = get_rule_124_matrix(s)
        nb_layers = len(the_mat[0])
        nb_iters = 51 + 153*(nb_layers-1)
        print(f"echo '{s}'")
        print("python3 -m python_input.Collatz_computer_east rule_124_computer "
              f"{s} {nb_layers} | "
              f"./visutiles -i -n {nb_iters} --no-gui -p >> rule_124_tests.txt")
    return


def test_validity(json_dict, the_mat):

    top_right = np.array((0, -17), dtype=np.int64)
    move_left = np.array((-46, 0), dtype=np.int64)
    move_down = np.array((0, -27), dtype=np.int64)

    edge_list = json_dict["input"]["edges_to_check"]

    new_dict = {}
    for edge in edge_list:
        new_dict[tuple(edge[0][1])] = edge[1]

    the_computed_mat = np.ones(the_mat.shape, dtype=np.int64)
    for i in range(0, len(the_mat)):
        for j in range(-1, -1*len(the_mat[0])-1, -1):
            true_color = the_mat[i, j]
            if true_color == -1:
                the_computed_mat[i, j] = -1
                continue

            edge_pos = top_right + i*move_down - (j+1)*move_left

            the_computed_mat[i, j] = new_dict[tuple(edge_pos)][1]

    print(the_mat)
    print(the_computed_mat)

    if not (the_mat == the_computed_mat).all():
        print("Test failed")
    else:
        print("Test OK")
    return True


def test_specific_input(s):
    the_mat = get_rule_124_matrix(s)
    nb_layers = len(the_mat[0])
    nb_iters = 51 + 153*(nb_layers-1)
    print(nb_iters)
    p = Popen(
        f"./visutiles -i -n {nb_iters} --no-gui -p".split(), stdin=PIPE, stdout=PIPE, bufsize=int(10*1e6))
    out, err = p.communicate(str.encode(
        json.dumps(rule_124_computer(s, str(nb_layers)))))

    print("Testing", s)
    json_dict = json.loads(out)
    test_validity(json_dict, the_mat)


def rule_124_mat(s):
    the_mat = get_rule_124_matrix(s)
    print(the_mat)


if __name__ == "__main__":
    locals()[sys.argv[1]](*sys.argv[2:])
# create_sh_all_nbit_strings(8)
