#### **INSTALL**
**Build and install with cmake**:	  
	- Install **cmake**.     
	- Come to root folder, **mkdir build && cd build**.  
	- For windows-32: `cmake .. -G "Visual Studio 17 2022" -A win32`.  
	- For windows-32: `cmake .. -G "Visual Studio 17 2022" -A win32 -D__OPTIMZE_64CORE__=1`.1-64 cores, recommend.  
	- For windows-64: `cmake .. -G "Visual Studio 17 2022" `. 1-64 cores.  
	- For windows-64: `cmake .. -G "Visual Studio 17 2022" -D__OPTIMZE_64CORE__=1`. 1-64 cores, recommend.  
	- For windows-64: `cmake .. -G "Visual Studio 17 2022" -D__OPTIMZE_MORE_64CORE__=1`. More 64 cores, not yet checked b/c have environment.  
	- For Linux (Standard): **cmake .. -DUNIX_LINUX=1**.  If not support _GNU_SOURCE.  
	- For Linux (With GNU Extensions): `cmake .. -DUNIX_LINUX=1 -D_GNU_SOURCE=1 -D__LINUX__=1`.Recommend.  
	- For MAC-OSX/Apple: **cmake .. -DUNIX_LINUX=1 -DMACOSX=1**.  
	  
 

**Video Install/Demo(old way):**  
	- For [Windows 10 64bit](https://drive.google.com/file/d/1Ls4aD8fd65qcc7qgRKns2SlWw6RAhH5l/view?usp=sharing)  
	- For [VM Linux CentOs9 64bit](https://drive.google.com/file/d/1wxBjI6654wZqL7vTCZtvyiIRkJk-68HF/view?usp=sharing)  
	- For MacOSX 64bit, come to **src/mach**, and do similar steps of Linux.