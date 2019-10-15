# libscgi

## Description
libscgi is the generic/reusable SCGI parts. Functions and data types needed for
basic handling of SCGI.

## Thoughts
libscgi should explore different ways to minimize memory and CPU usage.

Also, parsing the header should be very safe and very fast. Only using standard
C-string length is not safe enough.

Current implementation with a vector should be changed to some static_vector.
We do not want to risk memory allocation.

Also the C-style array should be change to some safe array, but with no default
initialization of the bytes. Or a opt in/out of initialization of the memory.
Same should go for a static_vector. But might have a clean memory function when
leaving the scope.

The interface to access the 'raw_header' is also not very nice. But how to get
it nicer when you look how boost::asio want the memory buffer to look. How is
the STL memory buffer going to look? And the size of the raw_header is most
likely not good. What size should be good??

Currently using std::optional instead of exception for the at() member function.
Thats fine for me. As long as it is fast. Have to investigate performance.

## Benchmark
Benchmarking is hard. If benchmarks take to long it might slow your CPU down
because of overheating. And then it will give you wrong results. Make sure this
does not happen when you run the benchmars. Use the -l and -g command line
options to run only parts of the benchmark.

## Test
Need more test. What we have now is probably not very easy for fuzzy testing.
But that will be needed. And also should garantie that sanitizers find bugs if
there are any.
