
To run this project
1. Downloud files, the project structer should be likw:
    project-root/
    ├── CMakeLists.txt
    ├── src/
    ├── mainstorage.cpp
    └── mainstorage.h

2. In the project root directory (project-root)
   create a build folder:
   
     mkdir build
     cd build
3. Generate the build system using CMake:
  cmake ..
4. Build the project:
  cmake --build .
5. Run the resulting executable:
  ./MyProject
