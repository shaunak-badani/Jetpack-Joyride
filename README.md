# Making Jetpack Joyride using OpenGL

### Assignment 1 of Graphics course

---

# File structure

Showing only the files relevant to the project, i.e. editable.

```bash
src/   
├── ball.cpp    
├── ball.h  
├── color.cpp   
├── config.h    
├── eyes.h  
├── fireline.h  
├── functions.h 
├── graphics_asgn1  
├── input.cpp   
├── main.cpp        
├── main.h  
```
### Function of each file

| Filename      | Purpose       |
|:-------------:|:-------------| 
| ball.cpp      | Contains the main parent class Ball which describes all the functions of class Ball |
| ball.h      | Contains function declaration of all the functions of class Ball      | 
| color.cpp | Stores structures of the various colours used throughout the project in RGB format|
| config.h | Declaration of all variables rendered on the screen|
| eyes.h | Contains eye, target, and up, to be accessible by all files |
| fireline.h | Contains FireLine subclass inherited from Ball |
| functions.h | |
| graphics_asgn1 | Executable to play the game |
| input.cpp | Contains code dealing with handling input |
| main.cpp | Initializes all elements, and contains the main game loop|
| main.h | Header file for main.cpp |

# Run the game

Execute following commands in /build directory :
```bash
cmake ..
cd ../src
make all
./graphics_asgn1
```
An OpenGL window should pop up, and the game Jetpack Joyride can be played.

# Controls

| Key      | Action       |
|:-------------:|:-------------| 
| d      | Move Forward |
| a      | Move Backward |
| spacebar      | Jump / Use Jetpack |
| q      | Quit the game |




License
-------
The MIT License https://meghprkh.mit-license.org/

Copyright &copy; 2018 Megh Parikh <meghprkh@gmail.com>
