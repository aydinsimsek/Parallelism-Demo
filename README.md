## Method    
In parallel execution, each processor on the system is utilized for calculating a specific chunk of the matrix multiplication.

## Build Steps

1-) Create and navigate to build directory    
~~~
mkdir build    
cd build      
~~~
2-) Generate Makefile     
* Windows:
~~~ 
cmake -G "MinGW Makefiles" .. 
~~~  
* Unix-based systems:
~~~ 
cmake .. 
~~~ 
3-) Run make command  
* Windows:
~~~ 
mingw32-make 
~~~  
* Unix-based systems:
~~~ 
make 
~~~

## Sample Result on Ubuntu
![Multithreading-Ubuntu](https://github.com/user-attachments/assets/5218497a-ef44-4f2d-b950-7ddb8592fe68)

## Sample Result on Windows
![Multithreading-Windows](https://github.com/user-attachments/assets/510d903b-f23c-494b-ba98-f3c2f3e02240)