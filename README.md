# College Map Navigation System

## Project Overview
A C-based navigation system that helps users find routes between college locations and identify the shortest path.

## Features
- Display all available college locations
- Search for a college location
- Find all possible routes between two locations
- Calculate the shortest route using Dijkstra's algorithm
- Display the three shortest routes
- Estimate walking time
- View recent searches during program execution

## Technologies Used
- Programming Language: C
- Data Structure: Graph using an adjacency matrix
- Algorithms: Depth-First Search (DFS), backtracking, and Dijkstra's algorithm

## How to Compile
```bash
gcc college_map.c -o college_map
```

Run on Windows:
```bash
college_map.exe
```

## Limitations
The current version uses 12 sample locations and illustrative road distances. Actual campus navigation requires verified locations, road connections, and distances.

## Future Enhancements
- Add all verified campus locations
- Improve alternative route selection
- Save search history permanently
- Develop a graphical map interface

## Author
Rathesh V and Kadar 

## Team Members
Rathesh V 
Kadar Haneef
