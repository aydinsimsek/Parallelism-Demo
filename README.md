## Method    
In parallel execution, each processor on the system is utilized for calculating a specific chunk of the matrix multiplication.      
My system has 4 physical cores and 8 logical processors (via Hyper-Threading), therefore ```std::thread::hardware_concurrency()``` indicates that up to 8 threads can run in parallel on my system.       

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
* Unix-like systems:
~~~ 
cmake .. 
~~~ 
3-) Run make command  
* Windows:
~~~ 
mingw32-make 
~~~  
* Unix-like systems:
~~~ 
make 
~~~

## Sample Result on Ubuntu
![screenshotUbuntu](https://github.com/user-attachments/assets/236cf411-348a-4142-bdc1-98aecb352e89)

## Sample Result on Windows
![screenshotWindows](https://github.com/user-attachments/assets/5d1340e1-36fb-4467-9dd8-ac61e6b0b51c)
