# ba_co

## Description
ba_co is an implementation using Boost Asio and coroutines. Currently only clang 8 supports coroutines.

## Thoughts
The code does not look nice, especially with all the logging. But to understand what is happening it is needed.
Want to avoid as much allocation of memory as possible. But the container with the key/value pairs are std::string
which allocated data. Reference into small_buffer_??

## building ba_co

~~~
cmake '-DCMAKE_CXX_COMPILER=clang++-8' '-DCMAKE_CXX_STANDARD=20' '-DCMAKE_CXX_FLAGS=-stdlib=libc++ -fcoroutines-ts'
~~~
