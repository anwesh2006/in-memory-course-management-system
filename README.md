# In-Memory Course Management System (C++)

## Overview
This project implements an in-memory course management system using Object-Oriented Programming in C++.
It allows managing courses, prerequisites, and enrollment eligibility while preventing cyclic dependencies.

## Features
- Add and remove courses
- Add prerequisites with cycle detection
- List direct and indirect prerequisites
- Check enrollment eligibility

## In-Memory Data Storage
This system is implemented as a fully **in-memory application**.

- All course and prerequisite data is stored in RAM using C++ STL containers such as `unordered_map` and `unordered_set`.
- No external database, file system, or persistent storage is used.
- All data exists only during program execution and is lost when the application terminates.

This design is intentional and suitable for the given constraints (up to 1000 courses).

## Design
- Courses are modeled as nodes in a directed graph
- Prerequisites are directed edges
- DFS is used for cycle detection and prerequisite traversal

