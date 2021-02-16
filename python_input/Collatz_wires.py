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


def simple_bridge(two_bits, length):
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
