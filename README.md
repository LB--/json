json [![travis](https://travis-ci.org/LB--/json.svg?branch=C%2B%2B14)](https://travis-ci.org/LB--/json)
====

My personal C++14 JSON library in the public domain.

## Usage
### CMake
From the `cmake` directory, copy the `FindLB` directory to a place in your `CMAKE_MODULE_PATH`.
Then, add `find_package(LB/json REQUIRED)` to your CMake script.
You may need to set the CMake variable `LB/json_ROOT` if you installed to a nonstandard location.
Finally, link to the `LB::json` imported target with `target_link_libraries()`.

### C++
`#include <LB/json/json.hpp>`

TODO
