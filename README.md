* For test purposes currently program stops after exceeding 1000 ms, to avoid this please comment line 162 in mainstorage.h:

        flag = (delta > 1000) ? false:true;

* The project written in c++ 20 (CMakeList also addresses it: 


        set(CMAKE_CXX_STANDARD 20)

        set(CMAKE_CXX_STANDARD_REQUIRED True)

   )
        For smaller versions code and cMakeList.txt should be adjasted


* To run this project
        1. Downloud files, the project structer should be like:
   
            project-root/
   
            ├── CMakeLists.txt
   
            ├── src/
   
                ├── mainstorage.cpp
   
                └── mainstorage.h

        3. In the project root directory (project-root)
           create a build folder:
   
             mkdir build
   
             cd build
   
        4. Generate the build system using CMake:
   
             cmake ..
  
        5. Build the project:
   
             cmake --build .
  
        6. Run the resulting executable:
 
            ./MyProject
