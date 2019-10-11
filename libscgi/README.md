# libscgi

## Description
libscgi is the generic/reusable SCGI parts. Functions and data types needed for
basic handling of SCGI.

## Thoughts
libscgi should explore different ways to minimize memory and CPU usage.

Also, parsing the header should be very safe and very fast. Only using standard
C-string length is not save enough.

Current implementation with a vector should be changed to some static_vector.

Also the C-style array should be change to some safe array, but with no default
initialization of the bytes. Or a opt in/out of initialization of the memory.
Same should go for a static_vector.

The interface to access the 'raw_header' is also not very nice. But how to get
it nicer when you look how boost::asio want the memory buffer to look. How is
the STL memory buffer going to look?

Currently using std::optional instead of exception for the at() member function.
Thats fine for me. As long as it is fast.

## Benchmark

## Test
