<div align="center">

  <h1>fox-engine</h1>
  
  <p>
  3D Entity-Component-System Architecture Game Engine
  </p>

  ![screenshot](img/preview.gif)

</div>


# Table of Contents

- [About the Project](#about-the-project)
- [Tech Stack](#tech-stack)
- [Features](#features)
- [Usage](#usage)

## About the Project

`fox-engine` is a game engine / C++ library that provides scaffolding for building a 3D game. The current version of the engine provides an efficient real-time rendering and content-building framework.

`fox-engine` uses an Entity-Component-System architecture to organize data and provide a series of extensible interfaces for content development purposes.

For examples of the engine in action, see [Fox n' Fowl](https://github.com/realchrismartin/fox-n-fowl).

### Tech Stack

[![C++](https://img.shields.io/badge/C++-%2300599C.svg?logo=c%2B%2B&logoColor=white)](#)
[![SDL](https://img.shields.io/badge/SDL-blue)](#)
[![OpenGL](https://img.shields.io/badge/OpenGL-blue)](#)

### Features

- **Entity-Component Architecture**: `fox-engine` leverages generics to allow for the creation of arbitrary `Components` that are allocated at load-time and maintained in Memory Arenas. Localization of data allows for more efficient operations on batches of component data (e.g. when copying MVP Transform data to the GPU)
- **Scene Graph**: `fox-engine` scenes are constructed as logical Graphs - entities with `TransformComponent`s can be transformed relative to one another.
- **Event Relay**: synchronous message broker allows for sending and receiving arbitrary data without coupling elements directly.
- **Statically allocated content**: `Scene` configuration and `GameEntity` configuration can both be specified at compile-time and used/reused as needed. 
- **Keyframe Animation (tweening)**: Supports animation of 3D Models using linear interpolation of vertex data

### Usage 

`fox-engine` must be built and run on Windows for now. 

- Recursively checkout this repository to obtain its dependencies.
- Build the project using `cmake`.
- `fox-engine` can be included as a static library in a `cmake` project.