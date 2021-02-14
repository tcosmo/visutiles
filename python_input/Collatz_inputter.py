

import sys
import json
from python_input.geometry import np, WEST, SOUTH, EAST, NORTH, CENTER

COLLATZ_TILESET = "assets/tilesets/CollatzTileset/CollatzTileset.json"


def init_json_dict():
    return {'tileset': COLLATZ_TILESET, "input": {"edges": [], "tiles": []}}


def pos_to_serlist(pos):
    return [int(pos[0]), int(pos[1])]


def get_edge(pos_a, pos_b, color):
    return [[pos_to_serlist(pos_a), pos_to_serlist(pos_b)],
            list(color)]


def get_pv_edges(pv_string, starting=CENTER, full_of_zeros=False):
    last_pos = None
    curr_pos = np.array(starting)

    edges = []

    for c in pv_string:
        if c == "1":
            last_pos = np.array(curr_pos)
            curr_pos += SOUTH
            edges.append(get_edge(last_pos, curr_pos,
                                  ("ter", 1 if not full_of_zeros else 0)))

        last_pos = np.array(curr_pos)
        curr_pos += WEST
        edges.append(get_edge(last_pos, curr_pos, ("bin", 0)))
    return edges


def get_weird_pv_edges(weird_pv_string, starting=CENTER):
    # I believe that this gives parity vectors for {x/2,(3x-1)/2}
    last_pos = None
    curr_pos = np.array(starting)

    edges = []

    for c in weird_pv_string:
        if c == "1":
            last_pos = np.array(curr_pos)
            curr_pos += SOUTH
            edges.append(get_edge(last_pos, curr_pos,
                                  ("ter", 0)))

        last_pos = np.array(curr_pos)
        curr_pos += WEST
        edges.append(get_edge(last_pos, curr_pos, ("bin", int(c))))
    return edges


class ParityVector(object):
    def __init__(self, pv_string):
        self.pv_string = pv_string

    def span(self):
        return self.pv_string.count("1")

    def norm(self):
        return len(self.pv_string)


def parity_vector_and_north_span_translate(pv_string):
    pv = ParityVector(pv_string)
    json_dict = init_json_dict()
    json_dict["input"]["edges"] = get_pv_edges(pv_string, CENTER)
    json_dict["input"]["edges"] += get_pv_edges(
        pv_string, CENTER+pv.span()*NORTH, full_of_zeros=True)
    return json_dict


def parity_vector(pv_string):
    json_dict = init_json_dict()
    json_dict["input"]["edges"] = get_pv_edges(pv_string, CENTER)

    return json_dict


def weird_parity_vector(pv_string):
    json_dict = init_json_dict()
    json_dict["input"]["edges"] = get_weird_pv_edges(pv_string, CENTER)

    return json_dict


def get_edges_write_word_then_move(color_word, move, starting=CENTER):
    last_pos = None
    curr_pos = starting.copy()
    edges = []
    for b in color_word:
        last_pos = curr_pos.copy()
        curr_pos += move
        edges.append(get_edge(last_pos, curr_pos, b))
    return edges


def string_to_colors(binary_string, binary=True):
    return list(map(lambda x: ("bin" if binary else "ter", int(x)), binary_string))


def CRT(binary_string, ternary_string, padding=None):
    json_dict = init_json_dict()
    edges = []

    if padding is not None:
        padding = int(padding)

    edges += get_edges_write_word_then_move(
        string_to_colors(binary_string[::-1]), WEST)

    if padding is not None and len(binary_string) < padding:
        edges += get_edges_write_word_then_move(
            string_to_colors("0"*(padding-len(binary_string))), WEST, WEST*len(binary_string))

    edges += get_edges_write_word_then_move(
        string_to_colors(ternary_string[::-1], False), NORTH)

    if padding is not None and len(ternary_string) < padding:
        edges += get_edges_write_word_then_move(
            string_to_colors("0"*(padding-len(ternary_string)), False), NORTH, NORTH*len(ternary_string))

    json_dict["input"]["edges"] = edges
    return json_dict


def trivial_xor(binary_string):
    json_dict = init_json_dict()
    edges = []

    edges = get_edges_write_word_then_move(
        string_to_colors(binary_string[::-1], False), NORTH)

    edges.append(get_edge((0, 0), (-1, 0), ("bin", 0)))

    json_dict["input"]["edges"] = edges
    return json_dict


def mod3_parity(binary_string):
    json_dict = init_json_dict()
    edges = []

    edges = get_edges_write_word_then_move(
        string_to_colors(binary_string), WEST)

    edges.append(get_edge((0, 0), (0, 1), ("ter", 0)))

    json_dict["input"]["edges"] = edges
    return json_dict


def not_gate(one_bit):
    x = one_bit
    json_dict = init_json_dict()
    edges = []

    # Not x
    edges.append(get_edge((0, 0), (-1, 0), ("bin", int(x))))
    edges.append(get_edge((0, 0), (0, 1), ("ter", 1)))

    json_dict["input"]["edges"] = edges
    return json_dict


def and_gate(two_bits):
    x, y = two_bits
    json_dict = init_json_dict()
    edges = []

    # And
    edges.append(get_edge((0, 0), (0, 1), ("ter", int(x))))
    edges.append(get_edge((0, 1), (-1, 1), ("bin", int(y))))
    edges.append(get_edge((-1, 1), (-2, 1), ("bin", 0)))

    json_dict["input"]["edges"] = edges
    return json_dict


def xor_gate(two_bits):
    x, y = two_bits
    json_dict = init_json_dict()
    edges = []

    # Xor
    edges.append(get_edge((0, 0), (0, 1), ("ter", int(y))))
    edges.append(get_edge((0, 1), (-1, 1), ("bin", int(x))))

    json_dict["input"]["edges"] = edges
    return json_dict


def or_gate(two_bits):
    x, y = two_bits
    json_dict = init_json_dict()
    edges = []

    # or
    edges.append(get_edge((0, 0), (0, 1), ("ter", int(y))))
    edges.append(get_edge((0, 1), (-1, 1), ("bin", int(x))))

    edges.append(get_edge((0, 0), (0, -1), ("ter", 1)))
    edges.append(get_edge((-1, 0), (-2, 0), ("bin", 0)))
    edges.append(get_edge((-3, 1), (-3, 2), ("ter", int(y))))
    edges.append(get_edge((-3, 1), (-2, 1), ("bin", 1)))

    edges.append(get_edge((-3, 2), (-4, 2), ("bin", 0)))
    edges.append(get_edge((-4, 2), (-5, 2), ("bin", 0)))
    edges.append(get_edge((-5, 2), (-6, 2), ("bin", 0)))
    edges.append(get_edge((-6, 2), (-7, 2), ("bin", 0)))
    # edges.append(get_edge((-2, 2), (-1, 2), ("bin", int(x))))
    # edges.append(get_edge((0, 0), (0, -1), ("ter", 1))

    json_dict["input"]["edges"] = edges
    return json_dict


def rule_110(three_bits):
    # y' = OR(AND(NOT(x),y),XOR(y,z))
    x, y, z = three_bits
    json_dict = init_json_dict()
    edges = []

    # Not x
    edges.append(get_edge((0, 0), (-1, 0), ("bin", int(x))))
    edges.append(get_edge((0, 0), (0, 1), ("ter", 1)))

    # And y
    edges.append(get_edge((-1, 1), (-2, 1), ("bin", 0)))
    edges.append(get_edge((-2, 1), (-3, 1), ("bin", int(y))))
    edges.append(get_edge((-3, 1), (-4, 1), ("bin", 0)))

    # XOR(y,z)
    edges.append(get_edge((-4, 1), (-4, 2), ("ter", int(z))))
    edges.append(get_edge((-4, 2), (-5, 2), ("bin", int(y))))

    # OR
    edges.append(get_edge((-4, -1), (-4, 0), ("ter", 1)))
    edges.append(get_edge((-5, 0), (-6, 0), ("bin", 0)))
    edges.append(get_edge((-6, 2), (-7, 2), ("bin", int(y))))
    # edges.append(get_edge((-7, 2), (-6, 2), ("bin", 1)))

    edges.append(get_edge((-7, 3), (-8, 3), ("bin", 0)))
    edges.append(get_edge((-8, 3), (-9, 3), ("bin", 0)))
    edges.append(get_edge((-9, 3), (-10, 3), ("bin", 0)))
    edges.append(get_edge((-10, 3), (-11, 3), ("bin", 0)))

    json_dict["input"]["edges"] = edges
    return json_dict


def bin_ter_input(bin_ter_string):
    # just messing around
    json_dict = init_json_dict()
    edges = []

    last_pos = None
    curr_pos = CENTER.copy()
    for i, c in enumerate(bin_ter_string):
        last_pos = curr_pos.copy()
        if i % 2 == 0:
            curr_pos += WEST
            edges.append(get_edge(last_pos, curr_pos, ("bin", int(c))))
        else:
            curr_pos += SOUTH
            edges.append(get_edge(last_pos, curr_pos, ("ter", int(c))))

    json_dict["input"]["edges"] = edges
    return json_dict


def ter_bin_input(ter_bin_string):
    # just messing around
    json_dict = init_json_dict()
    edges = []

    last_pos = None
    curr_pos = CENTER.copy()
    for i, c in enumerate(ter_bin_string):
        last_pos = curr_pos.copy()
        if i % 2 == 1:
            curr_pos += WEST
            edges.append(get_edge(last_pos, curr_pos, ("bin", int(c))))
        else:
            curr_pos += SOUTH
            edges.append(get_edge(last_pos, curr_pos, ("ter", int(c))))

    json_dict["input"]["edges"] = edges
    return json_dict


if __name__ == "__main__":
    print(json.dumps(locals()[sys.argv[1]](*sys.argv[2:])))
