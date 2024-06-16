# Application-Framework
My application framework lets you easily create new applications for windows (Mac and Linux in the future). This framework includes GUI and Rendering capabilities for making any application you can think of.

## Setup
Once you clone the repo run the following commands in your root folder.
``` git
git submodule init
git submodule update
```
This will make sure the submodules are cloned.

Then run the file titled `GenerateProjects.bat` this will generate a visual studio project file.

## Goals
- Implement a fully featured 2d and 3d renderer that uses the OpenGL and Vulkan api's.
- Create a easy to use application system that includes features such as input, event handling, and windows.
- Create a library that is compatible with windows and linux.