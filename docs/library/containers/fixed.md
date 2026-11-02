---
title: Fixed-Capacity Containers
description: Constexpr-capable containers with compile-time capacity
---

# Fixed-Capacity Containers

Fixed-capacity containers provide zero-allocation, constexpr-capable storage with compile-time capacity bounds.

## FixedRingBuffer

## FixedQueue / FixedStack

## fixed::Bitset

## inplace_vector

`std::inplace_vector` is a fixed-capacity, dynamically sized vector with inline
storage, polyfilling the C++26 type. It is exported by `import inplace_vector;`
and `import synodic.periapsis;`. Adapted from the Beman Project implementation
(MIT or Apache-2.0 WITH LLVM-exception).
