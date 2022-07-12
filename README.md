# Custom cpp map 📘

Custom cpp map is an implementation of C++ [`std::map` container](https://en.cppreference.com/w/cpp/container/map) using a [skip list](https://en.wikipedia.org/wiki/Skip_list) other than a traditional red-black tree to achieve _O(log n)_ search and insert complexity. The custom map consists of the basic CRUD methods, as well as, comparison operators between maps. Additionally, this implementation provides custom iterators, constant iterators, and reverse iterators to the skip list for traversing through the data structure.

## Getting Started

These instructions will give you a copy of the neural network up and running on
your local machine for development and testing purposes.

### Prerequisites

To run this application locally on your computer, you'll need `Git` and `g++` installed on your computer.

### Installing

Then run the following command in the command line and go to the desired directory to store this project:

Clone this repository:

    git clone https://github.com/JonathanCen/custom-cpp-map.git

Go into the cloned repository:

```
cd custom-cpp-map
```

Compile cpp program:

```
make all
```

Run executable:

```
./Map
```

Running the executable will run the program on the `basic_test.cpp` file.

## Contributing

All issues and feature requests are welcome.
Feel free to check the [issues page](https://github.com/JonathanCen/custom-cpp-map/issues) if you want to contribute.

## Authors

- **Jonathan Cen** - [LinkedIn](https://www.linkedin.com/in/jonathancen/), [Github](https://github.com/JonathanCen)

## License

Copyright © 2022 [Jonathan Cen](<ADD PERSONAL WEBSITE LINK>).\
This project is [MIT licensed](https://github.com/JonathanCen/custom-cpp-map/blob/main/LICENSE).
