# Primal
[![Build Status](https://travis-ci.org/aschult5/primal.svg?branch=master)](https://travis-ci.org/aschult5/primal)


Primal is a server-client application that tests the [Primality] of an integer.

## Building
### Dependencies
* C++ compiler with C++14 support
* Boost 1.63.0
* googletest

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
./primal
./primal -n 18446744073709551557 #largest 64-bit prime
Batch mode

18446744073709551557 is PRIME
```

### 128-bit Example
```sh
./primal
./primal -n 58760334599773844685608985979864046177 #128 bit prime
Batch mode

58760334599773844685608985979864046177 is probably PRIME
```

### Interactive Example
```sh
./primal
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

## Testing
googletest is used for unit testing. `make check` or `nix-build --arg doCheck true` will build and run gtest.

Docker is used for end-to-end testing.
```sh
sudo docker run -itd --expose 1734 --name primal_server primal -
sudo docker run -it --expose 1734 primal --ip $(docker inspect -f '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' primal_server)
sudo docker run -it --expose 1734 primal --ip $(docker inspect -f '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' primal_server)
sudo docker run -it --expose 1734 primal --ip $(docker inspect -f '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' primal_server)
...
```

Batch mode allows you to specify many integers to test in one connection.
Inputs to batch mode are either a file or an integer passed on startup.

Interactive mode allows you to manually test new cases, before they're automated.


## How it works
### Primality Testing
See [A014233]

Using the first n primes as bases for the Miller-Rabin test, you can determine whether an integer is prime or not, up to a certain value.
The code does a binary search in the list of pseudoprimes for the integer to test to determine if it's eligible for the guaranteed algorithm,
otherwise it's passed onto boost::multiprecision::miller_rabin_test with 25 trials.

| n | Prime | Smallest Possible Pseudoprime if <=nth primes tested as base |
| ------ | ------ | ------ |
| 1 | 2 | 2047 |
| 2 | 3 | 1373653 |
| 3 | 5 | 25326001 |
| 4 | 7 | 3215031751 |
| ... | ...| ... |
| 11 | 31 | 3825123056546413051 |
| 12 | 37 | 318665857834031151167461 |

By default, any integer [1-318665857834031151167461) is guaranteed to be correctly identified as prime or composite.

Note that 64-bit integers are entirely covered.
Any integer outside that range will be identified as probably prime or composite.
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
