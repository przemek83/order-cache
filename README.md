[![Build & test](https://github.com/przemek83/order-cache/actions/workflows/buld-and-test.yml/badge.svg)](https://github.com/przemek83/order-cache/actions/workflows/buld-and-test.yml)
[![CodeQL](https://github.com/przemek83/order-cache/actions/workflows/github-code-scanning/codeql/badge.svg)](https://github.com/przemek83/order-cache/actions/workflows/github-code-scanning/codeql)
[![codecov](https://codecov.io/gh/przemek83/order-cache/graph/badge.svg?token=WCCXX1G5SL)](https://codecov.io/gh/przemek83/order-cache)

[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=przemek83_order-cache&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=przemek83_order-cache)
[![Bugs](https://sonarcloud.io/api/project_badges/measure?project=przemek83_order-cache&metric=bugs)](https://sonarcloud.io/summary/new_code?id=przemek83_order-cache)
[![Code Smells](https://sonarcloud.io/api/project_badges/measure?project=przemek83_order-cache&metric=code_smells)](https://sonarcloud.io/summary/new_code?id=przemek83_order-cache)
[![Coverage](https://sonarcloud.io/api/project_badges/measure?project=przemek83_order-cache&metric=coverage)](https://sonarcloud.io/summary/new_code?id=przemek83_order-cache)
[![Duplicated Lines (%)](https://sonarcloud.io/api/project_badges/measure?project=przemek83_order-cache&metric=duplicated_lines_density)](https://sonarcloud.io/summary/new_code?id=przemek83_order-cache)

## Table of content
- [About project](#about-project)
- [Problem description](#problem-description)
- [Getting Started](#getting-started)
   * [Prerequisites](#prerequisites)
   * [Building](#building)
- [Built with](#built-with)
- [Usage:  ](#usage)
- [Testing](#testing)
- [License](#license)

## About project
Financial orders cache with the option to add, remove and match orders quickly.

## Problem description
Create an order cache with orders on input and the following operations available in the public interface:
- `addOrder` having Order object as param and adding it to cache,
- `cancelOrder` having an order id as param and canceling an order with the given id,
- `cancelOrdersForUser` having user id as param and canceling all orders from given user,
- `cancelOrdersForSecIdWithMinimumQty` having security id and minimum quantity, fulfilling cancellation for given security if there is quantity threshold reached,
- `getMatchingSizeForSecurity` having security id as param and returning matched (buy to sell or sell to buy) quantity,
- `getAllOrders` returning vector of orders.

The solution needed to be very fast.

The application was initially a solution for an interview assignment. After some extensions, cleaning and removal of not-needed code, I've decided to publish it. Solution did not pass, minimal feedback stated that it was too complex. Still I had a lot of fun doing it, especially profiling and gradually improving performance :)

**Input**:  
Application expects orders as input, one per line. Order contains:
| field | description | restrictions |
| --- | --- | --- |
| id | order id | not empty |
| security id | security id used in order| not empty |
| side | type of order | Buy/Sell |
| quantity | quantity of securities to buy/sell | > 0 |
| user | user making order | not empty |
| company | company making order | not empty |

Example:

    OrdId1 SecId2 Buy 1100 User10 Company1

**Output**:  
Run of application outputs for each security-matched quantity in the current order cache. Output in each line prints the security id followed by a colon and quantity.

Example:

    SecId1: 3250

## Getting Started
This section describes briefly how to setup the environment and build the project.

### Prerequisites
C++ compiler with C++17 support as a minimum and CMake 3.14+. Additionally, the Git tool for downloading Catch2 test library as it is needed by test subproject.

### Building
Clone and use CMake directly or via any IDE supporting it. CMake should:

- configure everything automatically,
- compile and create binaries.

As a result of compilation, binary for simulations and binary for testing should be created.

## Built with
| Tool |  Windows 10 | Ubuntu 24.04 |
| --- | --- | --- |
| OS version | 10 22H2 | 24.04 |
| GCC | 13.1.0 | 13.2.0 |
| CMake | 3.30.2 | 3.28.3 |
| Git | 2.46.0 | 2.43.0 |
| Catch2 | 3.7.0 | 3.7.0 |

## Usage:  
The program can run in multiple ways. Here are 3 examples of how to run it:
1. Pass example file named `sample.txt` from project repo:
    ```shell
    $ ./order-cache < sample.txt
    Matching quantity:
    SecId1: 3250
    SecId2: 850
    SecId3: 1300
    ```
2. Use binary directly and type data:
    ```shell
    $ ./order-cache
    OrdId1 SecId2 Buy 1100 User10 Company1
    OrdId2 SecId2 Sell 2500 User8 Company1
    OrdId3 SecId1 Sell 800 User13 Company2
    OrdId4 SecId1 Buy 600 User12 Company3
    OrdId5 SecId3 Buy 300 User7 Company3
    OrdId6 SecId3 Sell 550 User3 Company1
    OrdId7 SecId3 Sell 750 User10 Company3
    OrdId8 SecId1 Buy 850 User2 Company3
    OrdId9 SecId1 Buy 950 User6 Company1
    OrdId10 SecId3 Buy 900 User5 Company2
    OrdId11 SecId2 Sell 1150 User13 Company1
    OrdId12 SecId2 Sell 850 User9 Company2
    OrdId13 SecId1 Buy 1350 User1 Company3
    OrdId14 SecId1 Sell 1250 User5 Company2
    OrdId15 SecId3 Buy 1400 User13 Company1
    OrdId16 SecId1 Sell 1200 User9 Company2
    OrdId17 SecId2 Sell 1050 User1 Company1
    Matching quantity:
    SecId1: 3250
    SecId2: 850
    SecId3: 1300

    ```
3. Pipe input:
    ```shell
    $ cat sample.txt | ./order-cache 
    Matching quantity:
    SecId1: 3250
    SecId2: 850
    SecId3: 1300
    ```
## Testing
For testing purposes, the Catch2 framework is used. Build the project first. Make sure that the `order-cache-test` target is built. Modern IDEs supporting CMake also support running tests with monitoring of failures. But in case you would like to run it manually, go to the `build/test` directory, where the⁣ binary `order-cache-test` should be available. Launching it should produce the following output on Linux:
Example run:
```
$ ./order-cache-test
Randomness seeded to: 3795383916
===============================================================================
All tests passed (46 assertions in 6 test cases)
```
As an alternative, CTest can be used to run tests from the `build/test` directory:

    $ ctest
    Test project /home/ja/GitHubProjects/order-cache/build/test
        Start 1: Adding order
    1/6 Test #1: Adding order .......................................   Passed    0.00 sec
        Start 2: cancel order
    2/6 Test #2: cancel order .......................................   Passed    0.00 sec
        Start 3: cancel orders for user
    3/6 Test #3: cancel orders for user .............................   Passed    0.00 sec
        Start 4: cancel orders for security with minimum quantity
    4/6 Test #4: cancel orders for security with minimum quantity ...   Passed    0.00 sec
        Start 5: matching size for security
    5/6 Test #5: matching size for security .........................   Passed    0.00 sec
        Start 6: benchmarks
    6/6 Test #6: benchmarks .........................................   Passed    1.16 sec

    100% tests passed, 0 tests failed out of 6

    Total Test time (real) =   1.18 sec

## License
This project is licensed under the MIT License. See the LICENSE file for details.

The project uses the following open-source software:
| Name | License | Home | Description |
| --- | --- | --- | --- |
| Catch2 | BSL-1.0 | https://github.com/catchorg/Catch2 | testing framework for C++ |