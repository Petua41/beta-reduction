# β-reduction interpreter

Simple interpreter of β-reduction on λ-term

## Features

### Strategies

The interpreter supports four β-reduction strategies:
* NO (Normal order)
* APPL (Applicative order)
* CBN (Call by name)
* CBV (Call by value)

### Brackets

The interpreter can add missing brackets:
* Missing outermost ones: `Lx.t` -> `(Lx.t)`
* In the right-hand side of abstractions: `(Lx.t x)` -> `(Lx.(t x))`

### Macros

The interpreter can process macros (i. e. named terms) \
Currently supported "macro-to-term" macros:
- Boolean values:
    - `true`
    - `false`
- Church numerals
- Operators:
    - Unary:
        - Boolean:
            - `NOT`
        - Arithmetic:
            - `ISZERO`
    - Binary:
        - Boolean:
            - And (typed as `&&`)
            - Or (typed as `||`)
        - Arithmetic:
            - Addition (typed as `+`)
            - Multiplication (typed as `*`)

Currently supported "term-to-macro" macros:
- Boolean values:
    - `true` (false isn't supported, because it's term is the same as the 0's one)
- Church numerals

## Installation

### To compile interpreter, run  
`git clone https://github.com/Petua41/beta-reduction` \
`cd beta-reduction` \
`mkdir build && cd build` \
`cmake .. && make -jN`, where N is number of simultaneous jobs

### To run tests, run
`build/target/Beta-reduction_test` or `cd build && ctest`

## Usage

`cd build/target`
`./beta-red [term]` \
Run `./beta-red --help` for more information