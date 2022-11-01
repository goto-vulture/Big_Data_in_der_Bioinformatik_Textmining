# Bioinformatics Textmining

## Introduction

In our current time, more information will be collected than ever before. This trend extends to almost every area in our life. In the private area and also in the professional environment. Especially in science, an above average increase can be observed.

In order to find more knowledge in this massive amount of data, the data will be placed and connected with other data, that was created in a comparable topic. Here is a simple example to show the general idea:

For example, when looking at Wikipedia articles that contains meaning-related topics. Without going in the content, new knowledge can be found by looking at which words occur in all articles. This makes it possible to find topics that play a role in each article.

With small amounts of data, this sounds easy and trivial. But with larger amounts of data, a search of this type through the data is only possible with the help of computers.



## General idea

For a efficient search process, all words will be encoded in integers. With this encoded data, simple integer comparisons are possible in shortest time.

With the help of some elementary set operations (the main important one: intersection), the set of words will be created, that appear in every source set.



## Structure of the evaluation
<img src="https://github.com/goto-vulture/Bioinformatics_Textmining/blob/Misc/Written_elaboration/Structure_of_the_evaluation_process_EN.png" width="50%" height="50%">



## Usage

### Building

make is the provided tool to build the program. To build the program with the default settings a simple `make` call is enough.

There are two switches for the building process:
- `DEBUG`: Building the program with debug information in the binary file (default)
- `RELEASE`: Building with compiler optimization.

`make clean` removes all compilation files.

### Interface

The CLI accepts the following main inputs, that are required:
- `-i`, `--input=<str>`: First input file
- `-j`, `--input2=<str>`: Second input file
- `-o`, `--output=<str>`: Output file

Optional arguments:
- `-f`, `--format`: Format the output for better readability in a normal editor?
- `-s`, `--sentence_offset`: Calculate sentence offsets? (In normal cases this is not necessary)
- `--no_part_matches`: No part matches will appear in the export file
- `--no_full_matches`: No full matches will appear in the export file
- `-h`, `--help`: Show a help message and exit

Debugging arguments:
- `-A`, `--abort=<float>`: Abort the calculation after X percent
- `-T`, `--run_all_test_functions`: Runing all test functions. This argument overrides all other arguments, except -h.(Only useful for debugging)



## Technical information

To make the implementation simple, the in and output will be created in a simple way. The name of the input files will be passed trough CLI parameter and the results will be represented as JSON file.

### Used libraries

- *argparse*: Simple and powerful CLI parser. Here available on [GitHub](https://github.com/cofyc/argparse).
- *cJSON*: Lightweight (and beautiful) JSON parser. Also available on [GitHub](https://github.com/DaveGamble/cJSON).
- *TinyTest*: Simple and tiny test suite. You can find it here on [GitHub](https://github.com/joewalnes/tinytest).



## Future features

- Determine the end of sentences (Using POS tags from the source files; list of common abbreviations; 3rd party sentence tokenizer)
- Calculate the error rate of the sentence end determining
- "Smart" Case insensitive comparison of the tokens:
    - Abbreviations should be compared case-sensitive
    - They need to be appear in their original representation in the result file
    - In all other cases a case insensitive comparison
- Word offsets (the current implementation provides char and sentence offsets)
- Change focus of the offsets to the origial file (in the current implementation the offset is focused on preprocessed data (stop word removed etc.))
    - "Where are the objects in the source data?"
- A more user-friendly interface. Maybe with a GUI. But the priority in this project is low.
