 
Grabber
*****


Introduction
============

Grabber is program, that can extract image from game Lotus 3 - the Ultimate Challenge.
Program work with folder LOTUS.DAT, who is all images of game. Grabber can repleace original
images on users images, but have some restrictions.


Building Grabber
================

Grabber is written in QT version 5.2.1. Program is possible simple building 
oppenning in QT Creator and build in IDE.

Run Grabber
===========

For running program Grabber is neccesary argument path of folder LOTUS.DAT. For example:
grabber -repleaceFile "../LOTUS.DAT"
Other parameters are optional (see main.cpp). 


Restrictions
============

In file LOTUS.DAT is compress image with specify size. Compress data of user images have muss be
bellow that original compress data. If this condition isn't execution, program lotus crashed. Grabber haven't so good compress algritmus how original. 
