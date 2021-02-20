import sys
import json
from python_input.geometry import np, WEST, SOUTH, EAST, NORTH, CENTER
from python_input.Collatz_inputter import get_edge, get_edge_v, init_json_dict, \
    get_edges_write_word_then_move, string_to_colors


def get_rule_110_router_module_edges_left_pole(bit, starting, length_params):
    edges = []
    now = starting.copy()
    edges.append(get_edge_v(now, WEST, ("bin", int(bit))))
    edges += get_edges_write_word_then_move(string_to_colors(
        "0" * length_params[0], binary=False), SOUTH, now)
    now += length_params[0] * SOUTH
    edges += get_edges_write_word_then_move(string_to_colors(
        "0" * (length_params[1] - 1), binary=True), WEST, now + WEST)

    now += (length_params[1] + 1) * WEST
    edges += get_edges_write_word_then_move(string_to_colors(
        "0"+"1" * (2*length_params[0]-1), binary=False), SOUTH, now + WEST)
    edges += get_edges_write_word_then_move(string_to_colors(
        "0" * (2 * (length_params[0] // 4) + 1), binary=True), WEST, now)
    now += (2 * (length_params[0] // 4) + 1)*WEST
    edges += get_edges_write_word_then_move(string_to_colors(
        "0" * (1), binary=True), WEST, now + 2 * NORTH)

    edges += get_edges_write_word_then_move(string_to_colors(
        "1" * (2 * length_params[0]), binary=False), SOUTH, now)
    now += (2 * length_params[0])*SOUTH
    # edges += get_edges_write_word_then_move(string_to_colors(
    #    "1", binary=False), NORTH, now+2*NORTH+EAST)
    return edges


def get_rule_110_router_module_edges_middle_pole(bit, starting, length_params):
    edges = []
    now = starting.copy()
    edges.append(get_edge_v(now, WEST, ("bin", int(bit))))
    edges += get_edges_write_word_then_move(string_to_colors(
        "1"*3*length_params[0], binary=False), SOUTH, now)

    return edges


def get_rule_110_router_module_edges_right_pole(bit, starting, length_params):
    edges = []
    now = starting.copy()
    edges.append(get_edge_v(now, WEST, ("bin", int(bit))))
    edges += get_edges_write_word_then_move(string_to_colors(
        "1" * (length_params[0] - 2), binary=False), SOUTH, now)
    now += length_params[0] * SOUTH
    edges += get_edges_write_word_then_move(string_to_colors(
        "1" * (1), binary=False), SOUTH, now+EAST)
    edges += get_edges_write_word_then_move(string_to_colors(
        "1" * (2 * length_params[0]), binary=False), SOUTH, now)
    now += (2 * length_params[0])*SOUTH
    edges += get_edges_write_word_then_move(string_to_colors(
        "1" * (1), binary=False), SOUTH, now + WEST)
    edges += get_edges_write_word_then_move(string_to_colors(
        "1" * (1), binary=False), SOUTH, now + EAST + NORTH)
    edges += get_edges_write_word_then_move(string_to_colors(
        "1" * (1), binary=True), WEST, now + 3*EAST+NORTH)
    now = now + WEST + SOUTH + EAST

    edges += get_edges_write_word_then_move(string_to_colors(
        "1" * (length_params[1] + length_params[0] - 1), binary=True), EAST, now)
    now += (length_params[1] + length_params[0] - 1)*EAST
    edges += get_edges_write_word_then_move(string_to_colors(
        "1", binary=False), NORTH, now+EAST)

    return edges


def get_rule_110_router_module_edges(bit, starting, length_params):
    edges = []

    now = starting.copy()
    edges += get_rule_110_router_module_edges_right_pole(
        bit, now, length_params)

    now += length_params[1]*WEST
    edges += get_rule_110_router_module_edges_middle_pole(
        bit, now, length_params)

    now += length_params[1]*WEST
    edges += get_rule_110_router_module_edges_left_pole(
        bit, now, length_params)

    # middle pole

    return edges


def rule_110_computer(bits):
    bits = bits[::-1]
    json_dict = init_json_dict()
    edges = []

    # must be even
    base_distance = 10

    # interinput distance
    # must be 6k + 1
    inter_input_distance = 12 + 1

    length_params = [base_distance, inter_input_distance]
    now = CENTER

    for i, b in enumerate(bits):
        edges += get_rule_110_router_module_edges(
            b, now+(3*i*length_params[1])*WEST, length_params)

    json_dict["input"]["edges"] = edges
    return json_dict


if __name__ == "__main__":
    print(json.dumps(locals()[sys.argv[1]](*sys.argv[2:])))
