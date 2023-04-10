This folder contains development codes for the NUCLEO-F446RE development board by STMicroelectronics.  
When creating a new project, the directory structure is as follows:  
**Inc**  
&ensp;...  
**Src**  
&ensp;...  
  
I empty the folders and create subfolders as follows:  
**Inc**  
&ensp;inc  
&ensp;stm32f446inc  
**Src**  
&ensp;src  
&ensp;stm32f446src  
  
The inc and src folders are contained in this repo, whereas the stm32f446inc and stm32f446src folders contain specific code and are not.  The contents of these folders are as follows:  
**stm32f446inc**  
&ensp;cmsis_compiler.h  
&ensp;cmsis_gcc.h  
&ensp;cmsis_version.h  
&ensp;core_cm4.h  
&ensp;mpu_armv7.h  
&ensp;stm32f446xx.h  
&ensp;system_stm32f4xx.h  
**stm32f446src**  
&ensp;syscalls.c  
&ensp;sysmem.c  

The files in these directories can be downloaded here: https://www.st.com/en/embedded-software/stm32cubef4.html  
