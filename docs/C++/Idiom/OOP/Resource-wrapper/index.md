# Object-based resource management

本章讨论C++中实现object-based resource management的各种idiom

Object-based resource management包括: 

一、类似于

二、resource wrapper，需要考虑resource safety、resource ownership，本文对此进行探讨，并且梳理C++中实现resource wrapper的established idiom。



## Resource wrapper: Single responsibility +  rule of three/five/zero + RAII 

使用"Single responsibility+rule of three/five/zero+RAII"来实现Resource wrapper。



## Copyable and movable

参见 `Copyable-and-movable` 章节。

## RAII

一、避免leakage

参见 `RAII` 章节。

## Rule of three/five/zero

参见 
