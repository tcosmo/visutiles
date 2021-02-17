import sys
import json
from python_input.geometry import np, WEST, SOUTH, EAST, NORTH, CENTER
from python_input.Collatz_inputter import get_edge, init_json_dict, \
    get_edges_write_word_then_move, string_to_colors


def get_horizontal_wire_edges(one_bit, length, input_west=True, starting=CENTER, mask=[False]):
    edges = []
    length = int(length)
    move = EAST if input_west else WEST
    edges += get_edges_write_word_then_move(
        string_to_colors("0" * 2 * length), move, starting)
    if not mask[0]:
        edges += [get_edge(starting, starting + NORTH, ("ter", int(one_bit)))]
    return edges


def horizontal_wire(one_bit, length, input_west="True"):
    bool_input_west = (input_west == "True" or input_west == "")
    json_dict = init_json_dict()
    edges = get_horizontal_wire_edges(one_bit, length, bool_input_west)
    json_dict["input"]["edges"] = edges
    return json_dict


def simple_ne_bridge(two_bits, length):
    json_dict = init_json_dict()
    length = int(length)
    edges = []
    edges += get_horizontal_wire_edges(two_bits[0], length,
                                       input_west=True, starting=CENTER)

    # edges += get_edges_write_word_then_move(
    # string_to_colors(two_bits[1] * 2), EAST, starting=EAST * 2 * length)

    edges += get_horizontal_wire_edges("", length,
                                       input_west=True,
                                       starting=EAST * 2 * length + 2 * EAST, mask=[True])

    edges += get_edges_write_word_then_move(
        string_to_colors("0", binary=False), NORTH, starting=EAST * 2 * length + 2*EAST + NORTH)

    edges += get_edges_write_word_then_move(
        string_to_colors("0" * 2 * length, binary=False), NORTH, starting=EAST * 2 * length + 2 * EAST + NORTH+5*WEST+NORTH)

    edges += get_edges_write_word_then_move(
        string_to_colors("0" * 2 * length, binary=False), SOUTH, starting=EAST * 2 * length+2*EAST)

    edges += [get_edge(EAST * 2 * length+1*EAST+2*length*SOUTH + WEST+NORTH,
                       EAST * 2 * length + 1 * EAST + 2 * length * SOUTH + WEST, ("ter", 0))]

    edges += [get_edge(EAST * 2 * length+1*EAST+2*length*SOUTH,
                       EAST * 2 * length+1*EAST+2*length*SOUTH+EAST, ("bin", int(two_bits[1])))]
    # edges += [get_edge(EAST * 2 * length+1*EAST+2*length*SOUTH+WEST,
    #                    EAST * 2 * length+1*EAST+2*length*SOUTH+EAST+WEST, ("bin", int(two_bits[1])))]

    json_dict["input"]["edges"] = edges
    return json_dict


def simple_sw_bridge(two_bits, length):
    two_bits = two_bits[::-1]
    json_dict = init_json_dict()
    length = int(length)
    edges = []
    k = 2
    edges += get_edges_write_word_then_move(
        string_to_colors("0" * (6 * length + k)), EAST, CENTER + (5 - k + 2) * WEST)

    edges += get_edges_write_word_then_move(
        string_to_colors("0" * (6 * length)), EAST, CENTER + (5 - k + 2) * WEST + 2*EAST+(6 * length + k-1)*EAST)

    edges += get_edges_write_word_then_move(
        string_to_colors("0" * (2)), WEST, CENTER + 2 * NORTH + 5 * WEST)

    edges += get_edges_write_word_then_move(
        string_to_colors("0" * (6)), WEST, CENTER + 0*NORTH+7*WEST)

    # edges += get_edges_write_word_then_move(
    # string_to_colors(two_bits[1] * 2), EAST, starting=EAST * 2 * length)

    # edges += get_edges_write_word_then_move(
    #    string_to_colors("0" * (2 * length)), WEST, EAST * 4 * length + 2 * EAST)
    l = 7

    edges += [get_edge(CENTER + (5 - k + 2) * WEST + EAST+(6 * length + k-1)*EAST+NORTH+l*EAST+EAST+NORTH,
                       CENTER + (5 - k + 2) * WEST + EAST + (6 * length + k - 1) * EAST + 2 * NORTH + l * EAST+EAST+NORTH, ("ter", int(two_bits[0])))]

    edges += [get_edge(CENTER + (5 - k + 2) * WEST + EAST+(6 * length + k-1)*EAST+NORTH+l*EAST+EAST+2*NORTH+WEST,
                       CENTER + (5 - k + 2) * WEST + EAST + (6 * length + k - 1) * EAST + 2 * NORTH + l * EAST+EAST+NORTH, ("bin", int(0)))]

    edges += [get_edge(CENTER + (5 - k + 2) * WEST + EAST+(6 * length + k-1)*EAST+l*EAST+EAST,
                       CENTER + (5 - k + 2) * WEST + EAST+(6 * length + k-1)*EAST + NORTH+l*EAST+EAST, ("ter", int(0)))]

    edges += [get_edge(CENTER + (5 - k + 2) * WEST + EAST+(6 * length + k-1)*EAST+NORTH+l*EAST+EAST,
                       CENTER + (5 - k + 2) * WEST + EAST+(6 * length + k-1)*EAST + 2*NORTH+l*EAST+EAST, ("ter", int(0)))]

    # edges += get_edges_write_word_then_move(
    #     string_to_colors("0", binary=False), NORTH, starting=EAST * 2 * length + 2*EAST + NORTH)

    edges += get_edges_write_word_then_move(
        string_to_colors("0" * (4 * length), binary=False), NORTH, starting=EAST * 2 * length + 2 * EAST + NORTH+NORTH)

    edges += [get_edge(EAST * 2 * length + 2 * EAST + WEST,
                       EAST * 2 * length + 2 * EAST + 2*WEST, ("bin", int(0)))]

    edges += get_edges_write_word_then_move(
        string_to_colors("0" * 6 * length, binary=False), SOUTH, starting=EAST * 2 * length+2*EAST)

    edges += [get_edge(EAST * 2 * length + 2 * EAST + NORTH+2*length*NORTH+NORTH+2*NORTH,
                       EAST * 2 * length + 2 * EAST + NORTH + 2 * length * NORTH + WEST + NORTH+2*NORTH, ("bin", int(two_bits[1])))]

    qq = 40
    edges += get_edges_write_word_then_move(
        string_to_colors("0" * (10 * length), binary=False), NORTH, starting=qq*WEST+EAST * 2 * length + 2 * EAST + NORTH+NORTH)

    edges += [get_edge(qq*WEST+EAST * 2 * length + 2 * EAST + NORTH+2*length*NORTH+NORTH+2*NORTH,
                       qq*WEST+EAST * 2 * length + 2 * EAST + NORTH + 2 * length * NORTH + WEST + NORTH+2*NORTH, ("bin", int(two_bits[1])))]

    # edges += [get_edge(EAST * 2 * length+1*EAST+2*length*SOUTH+WEST,
    #                    EAST * 2 * length+1*EAST+2*length*SOUTH+EAST+WEST, ("bin", int(two_bits[1])))]

    edges += get_horizontal_wire_edges("1", 10,
                                       input_west=False, starting=SOUTH*7+EAST*24)

    json_dict["input"]["edges"] = edges
    return json_dict


if __name__ == "__main__":
    print(json.dumps(locals()[sys.argv[1]](*sys.argv[2:])))
# def get_north_west_wire_edges(one_bit, starting, mask=[False], input_south=True):

#     edges = []

#     edges += [get_edge(starting, starting + NORTH, ("ter", 0))]
#     if not mask[0]:
#         if input_south:
#             edges += [get_edge(starting, starting + WEST,
#                                ("bin", int(one_bit)))]
#         else:
#             edges += [get_edge(starting+NORTH+WEST, starting + NORTH+2*WEST,
#                                ("bin", int(one_bit)))]
#             edges += [get_edge(starting + NORTH+2*WEST + SOUTH, starting + NORTH+2*WEST,
#                                ("ter", 2*int(one_bit)))]

#     edges += [get_edge(starting + WEST, starting + 2 * WEST, ("bin", 0))]

#     return edges

# def north_west_wire(one_bit, length):
#     length = int(length)
#     json_dict = init_json_dict()
#     edges = []
#     module_input_displacement = WEST + NORTH
#     for i in range(length):
#         edges += get_north_west_wire_edges(one_bit,
#                                            starting=i * module_input_displacement,
#                                            mask=[i != 0])
#     json_dict["input"]["edges"] = edges
#     return json_dict

# def south_east_wire(one_bit, length):
#     # Harder than I tought for x=0
#     length = int(length)
#     json_dict = init_json_dict()
#     edges = []
#     module_input_displacement = EAST + SOUTH
#     for i in range(length):
#         edges += get_north_west_wire_edges(one_bit,
#                                            starting=i * module_input_displacement,
#                                            mask=[i != 0], input_south=False)
#     json_dict["input"]["edges"] = edges
#     return json_dict

# def get_south_west_wire_edges(one_bit, starting=CENTER, mask=[False]):
#     # Not working
#     x = one_bit
#     edges = []

#     edges += [get_edge(starting, starting + SOUTH, ("ter", 2))]
#     # edges += [get_edge(starting+WEST+SOUTH, starting +
#     #                   WEST+2*SOUTH, ("ter", 1))]
#     edges += [get_edge(starting + WEST, starting + 2*WEST, ("bin", 0))]
#     if not mask[0]:
#         edges += [get_edge(starting, starting + WEST,
#                            ("bin", int(one_bit)))]
#     return edges

# def south_west_wire(one_bit, length):
#     length = int(length)
#     json_dict = init_json_dict()
#     edges = []
#     module_input_displacement = WEST + SOUTH
#     for i in range(length):
#         edges += get_south_west_wire_edges(one_bit,
#                                            starting=i * module_input_displacement,
#                                            mask=[i != 0])
#     json_dict["input"]["edges"] = edges
#     return json_dict

# def get_north_east_wire_edges(one_bit, starting=CENTER, mask=[False]):
#     x = one_bit
#     edges = []

#     edges += [get_edge(starting, starting + NORTH, ("ter", 2))]
#     edges += [get_edge(starting+EAST, starting + 2*EAST, ("bin", 1))]
#     if not mask[0]:
#         edges += [get_edge(starting, starting + EAST,
#                            ("bin", int(one_bit)))]
#     return edges

# def north_east_wire(one_bit, length):
#     length = int(length)
#     json_dict = init_json_dict()
#     edges = []
#     module_input_displacement = NORTH + EAST
#     for i in range(length):
#         edges += get_north_east_wire_edges(one_bit,
#                                            starting=i * module_input_displacement,
#                                            mask=[i != 0])
#     json_dict["input"]["edges"] = edges
#     return json_dict
