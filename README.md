# isr-validator
This is a validator for Core Challenge 2022.

This validator only checks whether the reconfiguration sequence in the output file is correct for the input.
Note that this does not strictly check the format of the input or output files is correct,
nor whether the input graph satisfies the conditions.

For more information about the Core Challenge 2022, please see [here](https://core-challenge.github.io/2022/).

> :warning:
> The validator written in python was not fast enough for some files, so we are currently working on the validator written in C++.

## Requirement
### For Python
* Python 3.x, >= 3.6

## Usage
### For Python
```
python3 main.py [col file] [dat file] [output file]
```

### For C++
```
make
```
```
./main [col file] [dat file] [output file]
```

###
* col file : describes an input graph structure in DIMACS format.
* dat file : describes start/target states.
* output file : describes an answer for the ISR problem.

For more information about the file format, please see the following links．
* [Input file format](https://core-challenge.github.io/2022/#input-file-format)
* [Output file format](https://core-challenge.github.io/2022/#output-file-format)

## Status Code
| Code | Description |
| ---- | ---- |
| 00 | Output file says that there is no correct reconfiguration sequence, so it passes all the checks. However, the validator cannot ensure that whether a correct reconfiguration sequence does not exist for the input. |
| 01 | Output file says that there is a correct reconfiguration sequence and passes all the checks without any warning. |
| 02 | Output file says that there is a correct reconfiguration sequence and passes all the checks, but there is some warning. |
| 10 | The initial state does not match the start state. |
| 11 | The last state does not match the target state. |
| 12 | Some state is not an independent set. |
| 13 | The difference between some successive states is not equal to one. |

## License
"isr-validator" is under [MIT license](https://en.wikipedia.org/wiki/MIT_License).
 