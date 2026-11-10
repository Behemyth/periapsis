---
title: Utilities
description: Miscellaneous utilities and helpers
---

# Utilities

Miscellaneous utility functions and types.

## nth_element

## NotImplementedException

## Hash Utilities

## function_ref

`std::function_ref` is a non-owning, type-erased callable reference, polyfilling
the C++26 type, along with `std::nontype`. It is exported by
`import function_ref;` and `import synodic.periapsis;`. Adapted from
[nontype_functional](https://github.com/zhihaoy/nontype_functional)
(BSD-2-Clause).

## generator

`std::generator` is a coroutine that yields a lazily computed range. Standard
libraries that provide `<generator>` supply it. For those that do not yet,
`import generator;` and `import synodic.periapsis;` export a polyfill of the
C++23 type.

The polyfill supports nested generators and other ranges through
`std::ranges::elements_of`. It ignores the allocator parameter.
