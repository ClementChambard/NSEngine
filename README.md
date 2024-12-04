# NSEngine
A barebone game engine written in C++ using OpenGL
Currently only working on Linux.

# Installation

Required:
- CMake
- A C++ compiler
- GNU Make or Ninja
- Libraries: OpenGL, glew, glm, OpenAL if building audio module, SDL2 if building with SDL

```git clone https://github.com/ClementChambard/NSEngine```

```cd NSEngine```

You can modify CMakeLists.txt to choose the modules to include and the windowing backend 

```mkdir build && cd build```

```cmake .. # add -GNinja to use Ninja instead of GNU Make```

```make # or ninja```

