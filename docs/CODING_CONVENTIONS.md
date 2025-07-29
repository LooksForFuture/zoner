# CODING CONVENTIONS
## Introduction
The coding style has been inspired by OpenBSD, Linux kernel, FreeBSD and 
suckless software with some slight modifications. It is recommended to read 
about the coding style of mentioned projects.

## struct
If a struct has a pointer to itself, then its name is "name_s". But, if not, 
then it doesn't need a name and only uses typedef.

## typedef
Monkey: Typedef good. Typdef makes code short. Typedef everywhere<br />
Human: Typedef is a double edged sword and should be used with caution.<br />
<br />
Typedef makes writing code easier. But, we have some rules for using it.<br />
First of all, it should be mentioned that typedef is used for structs.
When you see a typedef, you can be sure that it is a struct and nothing else. 
Nothing other than struct is allowed to use typedef in this project. We don't 
want to hide things under the hood. We just want to make coidng easier. So, 
typedef is only for structs.
