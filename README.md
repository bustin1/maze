# Maze
![maze](maze.png)
Top figure is the spanning tree\
Bottom figure is the maze genernated\
# Overview: 
Randomly generates mazes based on a minimum spanning tree, whose weights are randomly generated.\
Prints maze on console rather than SDL, or openGL, no GUI.
```
Compile(w/ debug): make or make maze
Compile(w/o debug): make maze-debug
```

To change the size of the maze, change the variable num_of_vert(of spanning tree). I.E if set to 100, then spanning tree will
be 10x10 => 11x11 maze. If set to a non-perfect square, then maze.c will find the closert factor to its square-root. I.E if 
set to 40, then the dim of the spanning tree will be 5x8, with the width always being longer than the length.

All credit goes to CMU 15122 Fall '19 for the knowledge of header files, debugging, and data structures, such as
priority queues and graphs.
