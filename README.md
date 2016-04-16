json [![travis](https://travis-ci.org/LB--/json.svg?branch=C%2B%2B14)](https://travis-ci.org/LB--/json)
====

My personal C++14 JSON library in the public domain.
Notable features include support for duplicate keys in objects, and distinction between integers and reals.
The latter feature was the main inspiration to make yet another JSON library, as most that I found did not make any such distinction.

## Usage
### CMake
From the `cmake` directory, copy the `FindLB` directory to a place in your `CMAKE_MODULE_PATH`.
Then, add `find_package(LB/json REQUIRED)` to your CMake script.
You may need to set the CMake variable `LB/json_ROOT` if you installed to a nonstandard location.
Finally, link to the `LB::json` imported target with `target_link_libraries()`.

#### Utilities
By default the optional utilities library is built and installed, but if you don't need it you can tell CMake `-DBUILD_UTIL=OFF`.
If you do plan to use it, you must ask for it in your `find_package` command with either `COMPONENTS` or `OPTIONAL_COMPONENTS`.
Then, link to the `LB::json::util` imported target with `target_link_libraries()`.

### C++
`#include <LB/json/json.hpp>`

TODO
