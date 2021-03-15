import sys
import json
from python_input.geometry import np, WEST, SOUTH, EAST, NORTH, CENTER
from python_input.Collatz_inputter import get_edge, get_edge_v, init_json_dict, \
    get_edges_write_word_then_move, string_to_colors, pos_to_serlist


def get_rule_110_router_module_edges_north_pole(bit, starting, length_params, mask_input=False):
    edges = []
    now = starting.copy()

    if not mask_input:
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


def two_zero_corner(starting):
    edges = []
    now = starting.copy()
    edges.append(get_edge_v(now, EAST, ("bin", 0)))
    edges.append(get_edge_v(now, SOUTH, ("ter", 2)))
    return edges


def get_rule_110_router_module_edges_middle_pole(bit, starting, length_params, mask_input=False):
    edges = []
    now = starting.copy()
    if not mask_input:
        edges.append(get_edge_v(now, NORTH, ("ter", int(bit))))
        edges.append(get_edge_v(
            now+NORTH*length_params[2], NORTH, ("ter", int(bit))))

    edges += get_edges_write_word_then_move(
        string_to_colors("1"), WEST, now+NORTH)
    edges += get_edges_write_word_then_move(
        string_to_colors("111"), WEST, now+WEST)

    # And(Not(x),y)
    now += (length_params[1]+1) * WEST

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

    edges += get_edges_write_word_then_move(
        string_to_colors("11", binary=False), SOUTH, now+SOUTH+WEST)

    edges += get_edges_write_word_then_move(
        string_to_colors("1", binary=False), SOUTH, now + SOUTH)
    edges += get_edges_write_word_then_move(
        string_to_colors("1", binary=False), SOUTH, now + 2 * SOUTH)

    # OR
    edges += get_edges_write_word_then_move(
        string_to_colors("111"[::-1], binary=True), WEST, now+1*WEST)

    # repiping output
    now += 1 * WEST + 4 * SOUTH + 3 * WEST
    edges += get_edges_write_word_then_move(
        string_to_colors("1", binary=False), SOUTH, now + EAST)

    # param impair
    repipe = 9

    edges += get_edges_write_word_then_move(
        string_to_colors("0"*repipe, binary=True), WEST, now + SOUTH)

    for i in range(repipe-2):
        edges += get_edges_write_word_then_move(
            string_to_colors("1", binary=False), NORTH, now + i * WEST+2*WEST)
        edges += get_edges_write_word_then_move(
            string_to_colors("1", binary=False), NORTH, now + i*WEST+3*SOUTH+2*WEST)

    now += repipe * WEST
    #edges += two_zero_corner(now + NORTH)

    edges.append(get_edge_v(now + WEST, SOUTH, ("ter", 0)))
    edges.append(get_edge_v(now + WEST, EAST, ("bin", 0)))

    edges += get_edges_write_word_then_move(
        string_to_colors("1" * 6, binary=False), SOUTH, now + SOUTH)

    edges.append(get_edge_v(now + 7*SOUTH+NORTH+EAST, SOUTH, ("ter", 1)))
    edges.append(get_edge_v(now + 7*SOUTH, SOUTH, ("ter", 0)))
    edges.append(get_edge_v(now + 8 * SOUTH, SOUTH, ("ter", 0)))
    now += 9 * SOUTH+WEST

    repipe2 = 6*2
    edges += get_edges_write_word_then_move(
        string_to_colors("0" * repipe2, binary=True), WEST, now)

    edges += get_edges_write_word_then_move(
        string_to_colors("1" * 15+"10", binary=False), NORTH, now+repipe2*WEST+2*EAST+2*NORTH)

    now += repipe2*WEST
    repipe3 = 6*2
    edges += get_edges_write_word_then_move(
        string_to_colors("0" * repipe3, binary=True), WEST, now)

    # second input
    edges += get_edges_write_word_then_move(
        string_to_colors("0" * repipe3, binary=True), WEST, now+9*NORTH)

    # third input
    edges += get_edges_write_word_then_move(
        string_to_colors("0" * repipe3, binary=True), WEST, now+13*NORTH)

    # fourth input

    edges += get_edges_write_word_then_move(
        string_to_colors("1" * (repipe3 - 3), binary=True), WEST, now + 18 * NORTH)
    edges += get_edges_write_word_then_move(
        string_to_colors("0", binary=True), WEST, now + 18 * NORTH + NORTH + (repipe3 - 3) * WEST)
    edges += get_edges_write_word_then_move(
        string_to_colors("11", binary=True), WEST, now+18*NORTH+(repipe3 - 2)*WEST)

    for i in range(repipe3-2):
        edges += get_edges_write_word_then_move(
            string_to_colors("1", binary=False), NORTH, now + i * WEST+2*WEST+2*NORTH)
        edges += get_edges_write_word_then_move(
            string_to_colors("1", binary=False), NORTH, now + i * WEST + 3 * SOUTH + 2 * WEST + NORTH)

        edges += get_edges_write_word_then_move(
            string_to_colors("1", binary=False), NORTH, now + i * WEST+2*WEST+7*NORTH)
        edges += get_edges_write_word_then_move(
            string_to_colors("1", binary=False), NORTH, now + i*WEST+3*SOUTH+2*WEST+13*NORTH)

        edges += get_edges_write_word_then_move(
            string_to_colors("1", binary=False), NORTH, now + i * WEST+2*WEST+11*NORTH)
        edges += get_edges_write_word_then_move(
            string_to_colors("1", binary=False), NORTH, now + i * WEST + 3 * SOUTH + 2 * WEST + 17 * NORTH)

        edges += get_edges_write_word_then_move(
            string_to_colors("1", binary=False), NORTH, now + i * WEST+2*WEST+16*NORTH)

    return edges


def get_rule_110_router_module_edges_south_pole(bit, starting, length_params, mask_input=False, edges_to_check=[]):
    edges = []
    now = starting.copy()

    if not mask_input:
        edges.append(get_edge_v(now, NORTH, ("ter", int(bit))))
        edges.append(get_edge_v(now+NORTH, NORTH, ("ter", int(bit))))

    edges_to_check.append([pos_to_serlist(now), pos_to_serlist(now+NORTH)])
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


def get_rule_110_router_module_edges(bit, starting, length_params,  mask_input=False, edges_to_check=[]):
    edges = []

    now = starting.copy()
    edges += get_rule_110_router_module_edges_north_pole(
        bit, now, length_params, mask_input)

    now += length_params[0]*SOUTH
    edges += get_rule_110_router_module_edges_middle_pole(
        bit, now, length_params, mask_input)

    now += length_params[0]*SOUTH
    edges += get_rule_110_router_module_edges_south_pole(
        bit, now, length_params, mask_input, edges_to_check)

    # middle pole

    return edges


def rule_124_computer(bits, nb_iter):
    # Rule 124 is the mirror of rule 110
    # y' = OR(AND(NOT(x),y),XOR(y,z))

    nb_iter = int(nb_iter)

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

    edges_to_check = []
    for i, b in enumerate(bits):
        for it in range(nb_iter):
            edges += get_rule_110_router_module_edges(
                b, now+(3*i*length_params[0])*SOUTH+it*46*WEST, length_params, mask_input=(it != 0), edges_to_check=edges_to_check)

    json_dict["input"]["edges"] = edges
    json_dict["input"]["edges_to_check"] = edges_to_check
    return json_dict


if __name__ == "__main__":
    print(json.dumps(locals()[sys.argv[1]](*sys.argv[2:])))
