READ Me File

Please refer the latest information on the software at:
http://arlivre.ddo.jp/Physica/index.php/Particula03:Particula_Numerica

0.First
Particula is still in beta quality as of April 2008 but upgrade to 0.3
gaining several functions, we believe the software doesn't have any
serious bugs but still in beta. Please consider it when you use it. As
the very first step, you might be intersted in the Installation
instruction. If you already installed the software, you might want to
see the First Step. 

1.What is changed from version 0.2
 Mouse Manipulating 3D View
 Rotating 3D View with mouse becomes more intuitive. You can move the
 sight point by Opt/Alt-Dragging in 3D view. 

 Initial Conditions
 Several popular initial conditions are
 implemented. Plummer/Hernquest/Jaffe/Miyamoto-Nagai distribution are 
 available now. The velocity distributions obey polytrope of order of 5.

2. What is Particula
Thank you for your interest in my software "Particula Numerica" in
shortly "Particula" which is a Gravitational N-Body Simulator. The
software give you REAL-TIME three dimensional images for gravitationally
interacting bodies, which are thought to be stars. REAL-TIME isn't meant
real stellar time but you can see the MOTION of stars. The time
indicated in the software is normalized time. You have the actual time
by muliplying by a factor: sqrt(G/MR),
where M, R and G is a mass unit, length unit and gravitational constant,
respectively. 

3. What Particula can do
3.1 Initial Condition
 From 0.3, Particula gives several initial popular initial
 conditions. You can simulate using any mixture of the available initial
 conditions. 

 Random Sphere
 You can set a simple homo-temperature spherical distribution model as
 an initial condition. The temperature and the number of the particles
 are able to be set. 

 Load NEMO File
 You can also use NEMO ASCII files (stoa) as an initial data.

 Plummer
 Spherical symmetric distribution with Plummer model. The velocity is
 set to obey the polytrope of order 5. 

 Hernquest
 Spherical symmetric distribution with Hernquest model. The velocity is
 set to obey the polytrope of order 5. 

 Jaffe
 Spherical symmetric distribution with Jaffe model. The velocity is set
 to obey the polytrope of order 5. 

 Miyamoto
 Cylindrical symmetric distribution with Miyamoto-Nagai model. The
 velocity is set to obey the polytrope of order 5 but the z component is
 omitted. 

3.2 File I/O
Particula can save NEMO file format by clicking "Save" button in
"Physical" panel. Also you can load NEMO file as an initial
condition. If you don't familiar with NEMO please check it at:
http://bima.astro.umd.edu/nemo/ 

3.3 Simulation Control
You can set the total simulation time. You can start/stop when you like.

3.4 Visualization
Particula provide you both two and three dimensional view. It shows you
the overall 3D view of the particles. In 2D view you can see a
trajectory of a certain particle and the temporal variation of potential
energies (Kinetic/red and Potential/blue). To change the line of sight
in 3D view, just drag to rotate and shift-drag to zoom in/out. From
version 0.3, you can move the rotation/view centre by Opt/Alt-Mouse
dragging. 

3.5 Movie (a series of images) Creation
From 0.2, Particula can save images of 3D visual to a series of
images. The image is exactly same with the 3DView window. You can set
the interval of saving image with preference panel (Preference
menu). The interval indicates the iteration number of time-step to save
image. 

4. Characteristics
 Multi-Thread
 Particula is multi-thread-ed so you would get the benefit of multi-core
 processors. 

 SIMD vectorized
 Particula is vectorized for SIMD architectures. So far only Macintosh
 (PowerPC and Intel) version has the benefit. 

 Two additional Algorithms for effective computation
 You can select one of three algorithms for computation in
 simulationcontrol (Sim Ctrl) menu. The three algorithms are Direct,
 Oct-Tree and AD-Tree. When the number of particles are less than 1000,
 Direct is the most effective and less than 10000 Oct-Tree is
 effective. More than 10000 AD-Tree would be most effective. Direct
 method is a method just add the whole Newtonian force between any two
 particles. Oct-Tree method is a Octal-Tree based method as usual
 adopted. AD-Tree stands for Alternate digital tree and is used instead
 of Oct-Tree. 

5. The others
Particula binaries (Mac/IntelMac/Windows) are available free of
charge. You can redistribute the software to anyone you want. You are
expected to contact me when you would copy/distribute for commercial
use. 

6. Acknowledgement
The author of the software is grateful to the Open Source Communities
for the useful software, which are wxWidgets, wxmathplot and Pthread,
GLUT for win32, they are specific for MS Windows. 

7. Have fun!
The author of the software is ABE Hiroshi. You can reach me at:
habe36 at gmail.com
Your suggestion/help are really appreciated. Especially the author is
grateful for your helps on the initial conditions and on the post-data
processing. 

ABE Hiroshi, from Tokorozawa, JAPAN. 2nd of May 2008.

