# TileGenCPP

C++ implementation of https://github.com/inoble6502/TileGen

This project aims to implement a method of 3D tile-map generation and traversing akin to early first-person dungeon crawlers.

The TileFloor class reads in .CSV format data generated from https://github.com/inoble6502/DMapDesigner. Using the data the engine generates the map with instanced static meshes and pre-calculated points. 

The meshes in BasicTileGraphics were exported from Blender.SS