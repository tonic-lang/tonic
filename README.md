# Tonic
A superset language of C++, designed specifically for fast-paced competitive programming. Tonic makes competing much faster to save those precious seconds, without sacrificing runtime speed.

**Currently in early stages of development. So far the lexer, AST systems have been implemented, with parser and code generator remaining for a POC.**

## Why Tonic?
- Aims to be significantly less verbose than C++, geared towards competitions.
- Directly allows usage of C++ components.
- Low abstraction or overhead to keep runtime speed intact.
- Some competitive programming specific functionality including additional data structures, algorithms, tracing and debugging capabilities (in-progress).
- Since it compiles to C++ as a superset, the code can be directly used in competitions that support C++.

## Documentation
[Quick start](about:blank)<br />
[Full documentation](about:blank)

## Current state
Currently, I'm working towards a working version of Tonic with minimal error handling. For instance, certain statements that are direct C++ equivalents are directly transferred to the codegen for C++ representation in the current implementation. This will change with time and traction.

### Implementation
- Parse trees are bypassed in this implementation, as tokens are directly parsed to AST. However, with growing complexity, parse trees will prove useful.

## Use the web compiler to C++
WIP

## Install the CLI compiler
WIP

## Quick examples
WIP

## Contribute
WIP