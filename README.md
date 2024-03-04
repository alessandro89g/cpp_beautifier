# cpp_beautifier

This project was made to organize source and header of a class by starting from the header file. 

It reorganizes the implementations in the source file by reordering them as they are ordered in the header file.

## Getting Started

### Prerequisites

None.

### Installation

to_do

## Usage

If the source file and the header file are in the same directory you can just feed the program the header file as
``` bash
  cpp_beautifier <class.h>
```
or
``` bash
  cpp_beautifier <class.hpp>
```

If they are in different folders you have to pass the relative or absolute path to both files.
``` bash  
  cpp_beautifier <path_header/class.h> <path_source/class.cpp>
```

## Running the Tests

to_do

## Contributing

to_do

## License

to_do

## Contact

ale.alessandro.grassi@gmail.com

## Acknowledgments

No one for now.


## To be done

-  the methods with two or more modifiers get only the last one.
-  the methods with definition in the header are not recognized as methods