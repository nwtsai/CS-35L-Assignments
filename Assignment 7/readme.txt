Copy srt.tgz into working directory
  wget http://web.cs.ucla.edu/classes/fall16/cs35L/assign/srt.tgz

Unzip the tgz file with
  tar -xvzf srt.tgz

Copy srt directory and name new directory originalSrt
  cp -R srt originalSrt

Move into the srt directory
  cd srt

Compile the files
  make

Test the file with 
  ./srt 1 | wc -l
Output:
17296

I first modified the Makefile by adding:

-lpthread 

to the line

LBLIBS: -lm 

Now I look into the main.c file

The two for loops describe each pixel in the picture, so we should
parallel these for loops.

I went into the main function in main.c, and separated the pixel
generation into another function named generatePixels. This function 
is included in the arguments of pthread_create.

At first, in this generatePixels function, I initially declared many 
variables inside the draw function, but I realized that the speed of 
my program could increase if I declared variables as global outside 
both main and draw, because less computations would be executed.
Declaring variables as global significantly increased the efficiency 
of the program

Now compare the efficiency of original srt and my srt:

CHECK ORIGINAL SRT:

Commands:
cd srtOriginal
time ./srt 1-test.ppm >1-test.ppm.tmp

Output:
real   0m56.896s
user   0m56.889s
sys    0m0.002s

CHECK PARALLEL THREADS IMPLEMENTATION:

Command: 
make clean check

Output:
time ./srt 1-test.ppm >1-test.ppm.tmp
real	0m48.498s
user	0m48.512s
sys	0m0.001s

time ./srt 2-test.ppm >2-test.ppm.tmp
real	0m24.759s
user	0m49.208s
sys	0m0.000s

time ./srt 4-test.ppm >4-test.ppm.tmp
real	0m12.616s
user	0m49.272s
sys	0m0.004s


time ./srt 8-test.ppm >8-test.ppm.tmp
real	0m6.798s
user	0m51.934s
sys	0m0.001s

CONCLUSIONS:

– user is around the same, slightly better as a result of threads
– real time is a lot faster as a result of threads!
– real time = user time / # of threads