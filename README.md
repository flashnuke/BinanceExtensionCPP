# BinanceExtensionCPP

## Intro 

This library is an extension for the API of Binance. It is used to help write trading algorithms on Binance. 
The design is delibaretly attempting to reduce 'thinking' during runtime, by using clever methods such as CRTP and the 'Params' object. 
No virtual classes/methods are used in this library.

## Documentation

WIP
# Optimizations
For Microsoft compilers set the following flags for better runtime performance:
* Optimization: /O2
* Instrinsic functions: /Oi

## Examples
* orderbook_manager.cpp = Connecting to a symbol orderbook and fetching live data + getting initial snap via REST (not thread-safe).

## Dependencies

1. JsonCPP https://github.com/open-source-parsers/jsoncpp
2. CURL 
3. Boost/Beast websockets https://github.com/boostorg/beast

These 3 must be present in order to use the library.


## Lined up
* constexpr for endpoints + api base after the release of c++20 (support for constexpr and std::string)
