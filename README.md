Introduction

---

CChart is a simple and convenient curve plotting library. It can draw many kinds of 2D and 3D curves. It is based on C++ and Win32 API.

The prototype of CChart is CPlot, which has been extensively extended. CPlot's official website is http://cplot.sourceforge.net/, thanks to its author Todd Clements.

The author uses Microsoft Visual C++ 6.0 to develop. The code supports all higher versions of Visual Studio. The code does not use any new functions after C++ 98 standard, and the selected Third-party libraries are compatible with VC6.0.

The author is not a professional programmer, so the code is not particularly standard, especially the comments are incomplete, but the author has tried to make the code have a certain self-annotation function.

At present, a small number of functions of this library are still under development, without affecting the use.

Further information can be obtained at the website www.cchart.net, and the author can be contacted by e-mail baita00@hotmail.com.




Target users:

---

Mainly for the programmers who use Microsoft Visual C++, hoping to easily draw the data curve.

Through Dll mode, It can also be used in C#, Visual Basic.Net, Visual Basic 6, Visual Fortran, Delphi, BCB, MingW, HTML and other languages or IDEs.



Features:

---

* Full-featured.

It can draw a variety of 2D and 3D curves, such as line chart, contour chart, cloud chart, pie chart, bar chart, scatter chart, area chart, histogram chart, polar chart, Gantt chart, radar chart, waterfall chart, thermal chart, box chart, spectrum chart, etc. It also supports the drawing of various dashboard interfaces.

* Plenty of internal options.

Whether the curve itself, or the title, coordinate axis, legend, background, image annotation and other parts, there are a large number of options available for setting, even message response, right-click menu and other elements, all provide a custom method.

* Friendly user interface.

It simplifies the calling interface as much as possible. Without special requirements, it can usually draw delicate images with only a few lines of code.

* Good compatibility.

Only the Win32/Win64 API is used in the code, and no framework codes (such as MFC, QT, etc.) are used. Therefore, the users are not limited to a framework.

* Built-in thread security.

You can safely use it in a multi-threaded environment, without crash.

* Compatible with 64 bits.

It can compile 32-bit programs or 64-bit programs.



Quick start

---

1. Compile the library file first.

Open Plot.sln in the Source folder and compile the static link library.

Open CChart.sln in the Dll folder and compile the dynamic link library.

2. Header files

When programming with static libraries, the header file you need to include is PlotInterface.h in the Source/Plot folder, or Chart.h in the Source/Plot/Wraper folder.

When programming with dynamic libraries, the header file needed to be included is Chart. h in the Source/Plot/Wraper folder, but it needs to be modified slightly.

You can copy the file and modify the following two lines at the beginning of this file.

//# define CChart_API__declspec(dllimport)
# define CChart_API

Revised as follows

#define CChart_API__declspec(dllimport)
//#define CChart_API

3. Resource files

When using static libraries, you should include resource files by the following steps.

When using VC6, you can add the following commands in the View -> Resource Includes menu.

#include "Plot\Resources\PlotResources.rc"

At the same time, you need to set Additional resource include directories in the project properties as

Plot\Resources

You may find corresponding actions in higher version Visual Studio.

Using dynamic libraries does not require setting up resource files.



FAQs

---

Q1: When using static libraries, compilation errors occur as follows. How to solve them?

Fatal error C1076: compiler limit: internal heap limit reached; use/Zm to specify a higher limit

A1: Add /Zm600 to the compilation options.


CChart简介
---
CChart是一款简单方便的曲线绘制库，基于C++和Win32API，可以绘制多种二维、三维曲线。
CChart的原型是CPlot，已经在其基础上进行了大量的扩充。CPlot的官方网址是http://cplot.sourceforge.net/，感谢其作者Todd Clements。
作者使用Microsoft Visual C++ 6.0开发，支持所有更高版本的Visual Studio。代码中没有使用C++ 98标准以后的任何新功能，所选用的三方库，也都兼容VC6.0。
作者不是专业程序员，因此代码并不特别规范，特别是注释并不完整，但作者已尽量使得代码具有一定的自注释功能。
目前，本库的少量功能还在开发中，不影响使用。
可以访问www.cchart.net获得进一步的信息，亦可通过电子邮件baita00@hotmail.com联系作者。


CChart的目标用户:
---
主要针对使用Microsoft Visual C++，希望方便地实现数据曲线绘制的软件开发人员。
通过Dll模式，CChart也可以用于C#、Visual Basic .Net、Visual Basic 6、Visual Fortran、Delphi、BCB、MingW、HTML等语言或IDE。

CChart的特点：
---
* 功能全面。
    可以绘制多种二维、三维曲线，如折线图、等高线图、云图、饼图、柱图、散点图、面积图、直方图、极线图、甘特图、雷达图、瀑布图、热图、箱型图，频谱图等，同时支持多种仪表盘界面的绘制。 
* 内部选项丰富。
    无论是曲线本身，还是标题、坐标轴、图例、背景、图像注释等部分，都有大量的选项可供设置，甚至消息响应、右键菜单等元素，都提供了自定义的方法。
* 用户界面友好。
    尽可能地简化了调用接口，在没有特殊要求的情况下，一般只用几句代码就可以画出精致的图像。
* 兼容性好。
    开发中只使用了最基础的Win32/Win64 API，并没有使用任何框架代码（如MFC、QT等）。因此，用户程序所受的限制很小，不会局限于某种框架。
* 内置线程安全性。
    可以放心地在多线程环境中使用，避免崩溃。
* 兼容64位。
    可以编译32位程序，也可以编译64位程序。
    
快速开始
---
1.首先编译库文件。
打开Source文件夹里的Plot.sln，编译出静态链接库。
打开Dll文件夹里的CChart.sln，编译出动态链接库。
2.头文件
用静态库编程时，需要包含的头文件是Source/Plot文件夹中的PlotInterface.h，或者Source/Plot/Wraper文件夹中的Chart.h
用动态库编程时，需要包含的头文件是Source/Plot/Wraper文件夹中的Chart.h，但需要略加修改。
可以拷贝该文件，把文件开头两行
//#	define CChart_API __declspec(dllimport)
#	define CChart_API
修改如下
#	define CChart_API __declspec(dllimport)
//#	define CChart_API
3.资源文件
使用静态库时，请包含资源文件
VC6中可在View->Resource Includes菜单中添加下面的命令。
#include "Plot\Resources\PlotResources.rc"
同时，还需要项目属性中设置Additional resource include directories为
Plot\Resources
高版本Visual Studio中可以找到对应的动作。
使用动态库不需要设置资源文件。

常见问题
---
Q1:使用静态库，出现编译错误如下，怎么解决？
fatal error C1076: compiler limit : internal heap limit reached; use /Zm to specify a higher limit
A1:在编译选项中添加/Zm600即可。
