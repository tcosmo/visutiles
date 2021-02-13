

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


class ParityVector(object):
    def __init__(self, pv_string):
        self.pv_string = pv_string

    def span(self):
        return self.pv_string.count("1")

    def norm(self):
        return len(self.pv_string)


def parity_vector_input_and_north_span_translate(pv_string):
    pv = ParityVector(pv_string)
    json_dict = init_json_dict()
    json_dict["input"]["edges"] = get_pv_edges(pv_string, CENTER)
    json_dict["input"]["edges"] += get_pv_edges(
        pv_string, CENTER+pv.span()*NORTH, full_of_zeros=True)
    return json.dumps(json_dict)


def parity_vector_input(pv_string):
    json_dict = init_json_dict()
    json_dict["input"]["edges"] = get_pv_edges(pv_string, CENTER)

    return json.dumps(json_dict)


if __name__ == "__main__":
    print(locals()[sys.argv[1]](*sys.argv[2:]))
