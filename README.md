# Schelling-Simulations
Code for simulations and animations of a collection of 1D and 2D Schelling segregation models 
(most in C++, some using OpenGL/GLUT graphics, and few in python).

It is pretty fast and can deal with millions of nodes (we occasionally did 20 million) through
phase transitions and characteristics of the process are clearly visible in much smaller sizes (about 1000 or less).

No parallelization is used, but we worked on the implementation for making the code fast, without any compromises
on the model being simulated (probability, transitions and dynamics remain exactly as specified). Some versions even avoid
the use of C++ classes for dynamic arrays (like vectors, replacing them with low-level special-purpose array-updates) 
for optimal results. The most stripped-down versions are essentially C-code, writting directly the pixel array in a bitmap 
format, producing any specified resolution in the output image. The higher-level versions use basic C++ classes like vectors, 
and the OpenGL/GLUT framework for interactive 1D, 2D animations, as well as 3D graphics of the 2D model.

More information: http://barmpalias.net/schelcode.shtml 

![alt text](https://github.com/bob7/Schelling-Simulations/blob/master/images_readme/1Dfless.png)


Written by Barmpalias, Lewis-Pye and Elwes as part of their mathematical work on Schelling segregation.
Part of this code was used in the following published research (all by G.Barmpalias, R.Elwes and A.Lewis-Pye):

Digital morphogenesis via Schelling segregation. 
Nonlinearity 31 (2018) and FOCS 2014, 55th Annual IEEE Symposium on Foundations of Computer Science.

Minority population in the one-dimensional Schelling model of segregation. 
Journal of Statistical Physics 173(5), 2018

Unperturbed Schelling Segregation in Two and Three Dimensions
Journal of Statistical Physics 164(6) (2016) 

Tipping Points in 1-Dimensional Schelling Models with Switching Agents
Journal of Statistical Physics (2015) 158

Precise specifications of the models can be found in these articles, also available at: 
http://barmpalias.net/publications/publ.shtml

