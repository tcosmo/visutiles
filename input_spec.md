# Input format to `visutiles`

Inputs to visutiles are given in a way of a `json` file containing
information about:

1. The tileset that is used
2. Edges of the input configuration
3. Dually, tiles of the input configuration

Example:

```json
assets/example_input.json
{
  "tileset": "assets/tilesets/CollatzTileset/CollatzTileset.json",
  "input": {
    "edges": [
      [
        [
          [0, 0],
          [0, 1]
        ],
        ["ter", 2]
      ],
      [
        [
          [0, 0],
          [1, 0]
        ],
        ["bin", 0]
      ]
    ],
    "tiles": [[[-1, 0], "3"]]
  }
}
```

Note that integers are used for referring to values (positions or index of
alphabet letter) while strings are used for referring to keys (tiles names).

Tilesets are given in the following way:

```json
assets/tilesets/CollatzTileset/CollatzTileset.json
{
  "alphabets": {
    "bin": ["0", "1"],
    "ter": ["0", "1", "2"]
  },
  "alphabet_on_edge": ["ter", "bin", "ter", "bin"],
  "tiles": {
    "0": [0, 0, 0, 0],
    "1": [0, 1, 1, 0],
    "2": [1, 0, 2, 0],
    "3": [1, 1, 0, 1],
    "4": [2, 0, 1, 1],
    "5": [2, 1, 2, 1]
  }
}
```

Inputting to `visutiles` is then simply done like that:

`cat assets/example_input.json | ./visutiles -i`
