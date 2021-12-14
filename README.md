# isr-validator
This is a validator for Core Challenge 2022.

This validator only checks whether the reconfiguration sequence in the output file is correct for the input.
Note that this does not strictly check the format of the input or output files is correct,
nor whether the input graph satisfies the conditions.

For more information about the Core Challenge 2022, please see [here](https://core-challenge.github.io/2022/).

## Requirement
* Python 3.x, >= 3.6

## Usage
```bash
python3 main.py <col file> <dat file> <output file>
```

* col file : describes an input graph structure in DIMACS format.
* dat file : describes start/target states.
* output file : describes an answer for the ISR problem.

For more information about the file format, please see the following links．
* [Input file format](https://core-challenge.github.io/2022/#input-file-format)
* [Output file format](https://core-challenge.github.io/2022/#output-file-format)

## License
"isr-validator" is under [MIT license](https://en.wikipedia.org/wiki/MIT_License).
 