# Primal
[![Build Status](https://travis-ci.org/aschult5/primal.svg?branch=master)](https://travis-ci.org/aschult5/primal)


Primal is a server-client application that tests the [Primality] of an integer.

## Building
### Dependencies
C++ compiler with C++14 support
Boost 1.63.0
googletest

### nix
[nix] is the recommended (and well-tested) method, as it will fulfill dependencies, build, and install, all in isolation.
```sh
nix-build                        #build
nix-build --arg doCheck true     #unit test
nix-env -f default.nix -i primal #install
```

### autotools
bootstrap, configure.ac, and Makefile.am files are included.
```sh
./bootstrap
./configure [--with-boost-libdir=<boost path>]
make
make check
```
Note that googletest support in autotools is limited.
It should be installed in default search paths, or you'll need to pass the appropriate flags.


## Usage
Run with --help to see available options.

primal will run as either a client or server depending on command-line arguments.
A default port is used from the IANA User Port range.
If no IP is specified, 127.0.0.1 is assumed.

If you pass garbage values, you'll get garbage back, but it shouldn't crash.

### Basic Example
```sh
./primal       #server
./primal -n 13 #client
```

### 64-bit Example
```sh
./primal --pseudoprime 318665857834031151167461
./primal -n 18446744073709551557 #largest 64-bit prime
Batch mode

18446744073709551557 is PRIME
```

### 128-bit Example
```sh
./primal --pseudoprime 318665857834031151167461
./primal -n 58760334599773844685608985979864046177 #128 bit prime
Batch mode

58760334599773844685608985979864046177 is probably PRIME
```

### Interactive Example
```sh
./primal --pseudoprime 318665857834031151167461
./primal --ip 127.0.0.1
Interactive mode
Type white-space-separated integers to test, followed by enter:
1 2 3 4 5 6 7

1 is not prime
2 is PRIME
3 is PRIME
4 is not prime
5 is PRIME
6 is not prime
7 is PRIME
```


## Why Require an integer literal to have 64-bit & 128-bit support?
GCC does not support 128bit integer literals.

## How it works
### Primality Testing
Using the first n primes as bases for the Miller-Rabin test, you can determine whether a integer is prime or not
with confidence, up to a certain value. The code does a binary search in the list of pseudoprimes (right-column below) See [A014233]
2 -> 2047
3 -> 1373653
5 -> 25326001
7 -> 3215031751
...
31 -> 3825123056546413051
37 -> 318665857834031151167461

By default, any integer [1-3825123056546413051) is guaranteed to be correctly identified as prime or composite.

If you supply the 37's pseudoprime to the server instance,
the guaranteed range improves to [1-318665857834031151167461)

Note that 64-bit integers are entirely covered by the second range.
Any integer outside those ranges will be identified as probably prime or composite.
"Probably prime" is an effect of the [Miller-Rabin Primality Test].
Note: M-R can have false positives but not false negatives.

### Networking
[boost::asio] is used for server-client communications.
The server allows multiple concurrent requests from different clients.
For a given client, requests are processed and responded to in-order.


[Primality]: <https://en.wikipedia.org/wiki/Primality_test>
[nix]: <https://nixos.org/nix>
[Miller-Rabin Primality Test]: <https://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test>
[A014233]: <http://oeis.org/A014233>
[boost::asio]: <http://www.boost.org/doc/libs/1_63_0/doc/html/boost_asio/reference.html>
