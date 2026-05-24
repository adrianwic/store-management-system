C++ Store Management System

## Overview
This is a console-based store management application developed to practice and demonstrate advanced Object-Oriented Programming concepts in C++. It simulates a store's inventory, allowing the user to add, search, sell, and manage products, as well as process daily revenue and handle data persistence.

## Technical Highlights & Features
* **Object-Oriented Programming:** Implements inheritance and polymorphism (abstract base class Produkt and derived classes like Zywnosc, Zabawka, Urzadzenie).
* **Memory Management:** Utilizes dynamic memory allocation and manages a vector array of pointers.
* **Standard Template Library (STL):** Uses std::vector, std::sort , and std::remove_if alongside lambda expressions for efficient data manipulation and sorting.
* **Templates:** Features custom template functions for secure, type-safe user input validation.
* **Exception Handling:** Custom class Blad and try-catch blocks for robust error management and input sanitization.
* **File I/O:** Persistent data storage using fstream (saving and loading inventory states and daily financial reports).

## Project Structure
* main.cpp - The main application loop and interactive menu.
* klasy.h / klasy.cpp - Class declarations and method implementations.
* szablony.cpp - Custom template functions.

## How to Run
This project was developed using standard C++ and can be compiled using any modern C++ compiler or simply opened and built within Visual Studio.**
