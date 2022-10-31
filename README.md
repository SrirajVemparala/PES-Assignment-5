# PES-Assignment-5
CODE OPTIMIZATION</br>

The summary of the changes performed are:</br>
1) The data copy is performed using for loop and to optimize this memcopy and memset function was used</br>
2) To load and store 32bit integers from/to 8bit integers built-in swap functions were used. The swap operation can be seen in the functions ISHAResult() and F()</br>
3) Loop unrolling is performed which reduces the time. This can be observed in F() function.</br>
4) Assembly code usage will help optimize the code. The assembly code was written for the function ISHAProcessMessageBlock() </br>
5) Unnecessary code which would not run was removed. </br>
6) Length High was removed and only length low was used for buffer calculation and corrupted check is removed. This can be observed in ISHAResult and ISHAinput respectively.</br>
7) The ISHAReset and ISHAInput functions required to be calculated once for inner padding and outer padding.Hence static structures were used for optimization</br>
8)Added assembly code for ISHAProcessMessageBlock() function to reduce the time</br>

Results:</br>
Before Optimisation code size - 21056 bytes </br>
Before Optimisation run time - 8744ms</br>
After Optimisation code Size - 19468 bytes</br>
After Opitimised run time - 1393ms</br>

Debug Mode:
In debug code under optimisation code size is 20776 and run time is  1396 milliseconds  </br>
Release Mode:</br> 
In release mode under optimisation code size is 19468 and run time is 1393 milliseconds</br>
Extra Credit: </br>
1. Optimized the code size with assembly to 1393 ms </br>
