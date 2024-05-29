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
* Extra outermost ones: `((Lx.t))` -> `(Lx.t)`
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
`./build.sh -n -jN`, where N is number of simultaneous jobs

### To compile and run tests, run
`git clone https://github.com/Petua41/beta-reduction` \
`cd beta-reduction` \
`./build.sh -jN`, where N is number of simultaneous jobs \
`./Beta-reduction_test`

## Usage

`./beta-red [term]` \
Run `./beta-red --help` for more information