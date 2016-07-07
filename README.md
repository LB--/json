json [![travis](https://travis-ci.org/LB--/json.svg?branch=C%2B%2B14)](https://travis-ci.org/LB--/json)
====

My personal C++14 JSON library in the public domain.
Notable features include support for duplicate keys in objects, preservation of the order of keys, and distinction between integers and reals.

Consider instead [nlohmann/json](https://github.com/nlohmann/json) (MIT license) if you want a more robust and feature-rich C++ json library (though preservation of order of keys is not provided by default, you can provide your own map type).
Or, if you're looking for a C JSON library with similar properties, consider [udp/json-parser](https://github.com/udp/json-parser) and [udp/json-builder](https://github.com/udp/json-builder) (BSD license).

## Usage
### CMake
From the `cmake` directory, copy the `FindLB` directory to a place in your `CMAKE_MODULE_PATH`.
Then, add `find_package(LB/json REQUIRED)` to your CMake script.
You may need to set the CMake variable `LB/json_ROOT` if you installed to a nonstandard location.
Finally, link to the `LB::json` imported target with `target_link_libraries()`.

#### Utilities
By default the optional utilities library is built and installed, but if you don't need it you can tell CMake `-DBUILD_UTIL=OFF`.
If you do plan to use it, you must ask for `util` in your `find_package` command with either `COMPONENTS` or `OPTIONAL_COMPONENTS`.
Then, link to the `LB::json::util` imported target with `target_link_libraries()`.

### C++
`#include <LB/json/json.hpp>`

TODO
