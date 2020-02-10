#ReadMe.txt

a. Explain in your README.md file how many iterations you picked at different sizes and why. There is no single correct answer here, I just want to see your reasoning. b. Explain how you picked a single number from repeated measurements and justify it. There is no single correct answer here either. c. Plot a line graph of mean update time per coordinate as a function of total no. of objects (first parameter). You can use Excel or other GUIs, but it's best to stay in the Linux environment. I recommend learning how to use the "gnuplot" tool, which is both powerful and relatively straightforward, especially with text inputs. Check out this quick guide

Part 1:

a.)
These are the iterations I picked for each number:
100,000 iterations for 2^8 - 2^12
10,000 iterations for 2^13 - 2^17
1,500 iterations for 2 ^18 - 2^20
1,000 iterations for 2^21 
318 iterations for 2^22
100 iterations for 2^23

(Also, in my c++ code I made size and iters variables instead of command line inputs)

I tried out 1,000,000 for 2^8 and realized that it was running to slow to be my starting point.
100,000 worked a lot better and at each change interval I decided to bump down because it was taking
a long time. The only "non standard" number used (318) was chosen as a midpoint between 500 and 100
to optimize accuracy (proper mean runtime) and actual time the program took to run.

b.) 
I took the average of 6 runs for each size. I chose average because I wanted to see a result that was
well representative of the 6 runs and I felt like average was the best way to do that. All of the runs
were consistent within each size so I do not think it would've make that big of a difference if I had
chosen minimum or median.

Part 2:
a.)
These are the iterations I picked for each number:
2^8 - 2^15
100000

2^16 & 2^17:
10000

2^18 - 2^21:
1000

2^22 - 2^24
100

I was able to maintain a larger number of iterations throughout because of how much faster
c++ was running. The results were also 100x faster than the python code. Towards the end, I started decreasing iterations in response to compiler 
overflow warnings. Once again, I took the average of 6 trials for each size.

Part 3: 

I tested each type under the conditions of size: 2^15 and 10,000 with 3 trials each. Floating point values resulted in 
the slowest results. All between 0.030-0.035. Doubles were the measure I used in my code; these numbers averaged to
 0.004401. The rest of the types were about the same. They all were between 0.025 and 0.0299. All of these are notably
 slower than the doubles I used in mine.

 Part 4:
I was unable to figure out how to access the memory used by the code, but I did extract the time for each.
 I used the time function in Linux, to get the following real, user, and sys times for size of 1048576 and iterations = 7.


Python: real (2m07.037s), user (2m04.713s), sys(0m0.566s)
C++: real (0m34.300s), user (0m35.429s), sys (0m0.489s)

It becomes clear that python uses much more real and user times while
they have similar sys times.










