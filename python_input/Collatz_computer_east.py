import sys
import json
from python_input.geometry import np, WEST, SOUTH, EAST, NORTH, CENTER
from python_input.Collatz_inputter import get_edge, get_edge_v, init_json_dict, \
    get_edges_write_word_then_move, string_to_colors


def get_rule_110_router_module_edges_north_pole(bit, starting, length_params):
    edges = []
    now = starting.copy()

    edges.append(get_edge_v(now, NORTH, ("ter", int(bit))))

    edges += get_edges_write_word_then_move(
        string_to_colors("0" * (length_params[1]+2)), WEST, now)

    now += (length_params[1]+2) * WEST

    edges += get_edges_write_word_then_move(
        string_to_colors("1" * (length_params[0]-1), binary=False), NORTH, now+NORTH+2*EAST)

    now += NORTH + 2 * EAST + NORTH * (length_params[0] - 1)
    edges += get_edges_write_word_then_move(
        string_to_colors("1" * (length_params[0] - 2), binary=False), NORTH, now + 2 * NORTH + WEST)
    now += 2 * NORTH + WEST+(length_params[0] - 2)*NORTH
    edges += get_edges_write_word_then_move(
        string_to_colors("1" * (1), binary=False), SOUTH, now+EAST)
    return edges


def get_rule_110_router_module_edges_middle_pole(bit, starting, length_params):
    edges = []
    now = starting.copy()

    edges.append(get_edge_v(now+WEST, NORTH, ("ter", int(bit))))
    edges.append(get_edge_v(
        now+NORTH*length_params[2], NORTH, ("ter", int(bit))))

    now += WEST
    edges += get_edges_write_word_then_move(
        string_to_colors("0" * (length_params[1])), WEST, now)

    # And(Not(x),y)
    now += (length_params[1]) * WEST

    edges.append(get_edge_v(
        now + WEST, WEST, ("bin", int(1))))

    # edges.append(get_edge_v(
    #     now+4*WEST+NORTH, NORTH, ("ter", int(1))))

    now += WEST
    edges += get_edges_write_word_then_move(
        string_to_colors("0" * (1)), WEST, now + NORTH + WEST)
    edges += get_edges_write_word_then_move(
        string_to_colors("0" * (2)), WEST, now+2*WEST)

    # Xor(y,z)
    now = starting.copy() + NORTH * length_params[2]
    edges += get_edges_write_word_then_move(
        string_to_colors("0" * (length_params[1] + 5)), WEST, now)

    # Relever le signal à gauche
    now += (length_params[1] + 5)*WEST
    edges += get_edges_write_word_then_move(
        string_to_colors("0", binary=False), SOUTH, now)
    # # Redescendre au sud
    # edges += get_edges_write_word_then_move(
    #     string_to_colors("0", binary=True), WEST, now + WEST)

    # edges += get_edges_write_word_then_move(
    #     string_to_colors("1", binary=False), SOUTH, now + SOUTH + WEST)
    # edges += get_edges_write_word_then_move(
    #     string_to_colors("1", binary=False), SOUTH, now + SOUTH)
    # edges += get_edges_write_word_then_move(
    #     string_to_colors("1", binary=False), SOUTH, now + SOUTH+EAST)

    # # finir le mur
    # edges += get_edges_write_word_then_move(
    #     string_to_colors("2", binary=False), SOUTH, now+SOUTH)

    # # Or

    # edges += get_edges_write_word_then_move(
    #     string_to_colors("1", binary=False), SOUTH, now+2*SOUTH+WEST)

    return edges


def get_rule_110_router_module_edges_south_pole(bit, starting, length_params):
    edges = []
    now = starting.copy()

    edges.append(get_edge_v(now, NORTH, ("ter", int(bit))))
    edges.append(get_edge_v(now+NORTH, NORTH, ("ter", int(bit))))
    edges += get_edges_write_word_then_move(
        string_to_colors("1" * length_params[1]), WEST, now)

    # to stop downward signal
    edges.append(get_edge_v(
        now+(length_params[1]-1)*WEST, SOUTH, ("ter", int(1))))

    now += (length_params[1] + 2) * WEST

    # to retrieve rightward signal
    edges.append(get_edge_v(now + 2 * NORTH, WEST, ("bin", int(0))))

    # kill signal on top of ^ edge above
    edges.append(get_edge_v(now + 3 * NORTH + WEST, NORTH, ("ter", int(1))))
    # todo debug: will not scale below
    #edges.append(get_edge_v(now + 6 * NORTH+WEST, NORTH, ("ter", int(1))))

    edges.append(get_edge_v(now + WEST, WEST, ("bin", int(1))))
    edges.append(get_edge_v(now + 2 * WEST, WEST, ("bin", int(0))))

    # route signal south west
    edges.append(get_edge_v(now + 2 * WEST + WEST, WEST, ("bin", int(0))))
    edges.append(get_edge_v(now + 2 * WEST+WEST+WEST, SOUTH, ("ter", int(0))))
    now += 2 * WEST + WEST + SOUTH
    edges += get_edges_write_word_then_move(
        string_to_colors("11"+"1" * (length_params[0]-3+(length_params[0]-length_params[2]-1)), binary=False), SOUTH, now)
    now += (length_params[0] - 3 +
            (length_params[0] - length_params[2] - 1)) * SOUTH

    edges.append(get_edge_v(now + EAST, SOUTH, ("ter", int(1))))

    return edges


def get_rule_110_router_module_edges(bit, starting, length_params):
    edges = []

    now = starting.copy()
    edges += get_rule_110_router_module_edges_north_pole(
        bit, now, length_params)

    now += length_params[0]*SOUTH
    edges += get_rule_110_router_module_edges_middle_pole(
        bit, now, length_params)

    now += length_params[0]*SOUTH
    edges += get_rule_110_router_module_edges_south_pole(
        bit, now, length_params)

    # middle pole

    return edges


def rule_110_computer(bits):
    json_dict = init_json_dict()
    edges = []

    # must be odd and > 1
    distance_inter_input = 2 * 4 + 1

    # must be 6k + 3 and odd
    distance_between_input_and_first_exchanger = 6*0 + 3

    # must be < distance_inter_input
    # must be even?
    distance_between_the_two_middle_pole_input = 4

    length_params = [distance_inter_input,
                     distance_between_input_and_first_exchanger, distance_between_the_two_middle_pole_input]
    now = CENTER

    for i, b in enumerate(bits):
        edges += get_rule_110_router_module_edges(
            b, now+(3*i*length_params[0])*SOUTH, length_params)

    json_dict["input"]["edges"] = edges
    return json_dict


if __name__ == "__main__":
    print(json.dumps(locals()[sys.argv[1]](*sys.argv[2:])))
