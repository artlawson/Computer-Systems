ReadMe
Arthur Lawson

##Part 0: Intro
For each trial I compiled with the given line from the instructions:
gcc -O3 -std=c99 -march=native -o driver -Wall -Wextra -pedantic *.c


##Part 1: Naive Implementation

I decided to stick with the given c code, because why not? Running the standard atoi function on the virtual machine, 
I got a best runtime of 0.059199s.
For my naive implementation, I looked up the source code for atoi and took the atoi function
and the getDecDigits function and altered them for our specfic case. One of the first checks was
to edit the switch cases in atoi (I renamed apple) so it covers only string lengths that appear
in the text file. With these initial optimizations I improved to runtime of 0.025917s (56% performance increase). 
I attribute this performance increase mostly to the decrease of cases from 0-9 length strings to 3-5 to match the 
behavior exhibted by the data.


##Part 2: Optimization

The first thing I tried was working to unroll the for loop (calling apple assignment multiple times to maximize performance in relation to the given overhead). Initially I tried using 5,10, and 20 assignments and found that 10 & 20 provided
similar (or ever worse) performance compared to just using one assignment. At this point I accepted 5 as the optimal amount of
unrolling and proceeded to mess around with using different int types (uint32_t, fast_int, short,long....) and found that the difference was
not very noticable and decided to stick with int for most cases. At 10:15 pm before the assignment was due, I sat down at my laptop
just to fiddle with things some more and see if I could get lucky. At this point my current best time was around 0.0241 (6-7% performance
increase). I looked back at the lecture notes and messed around with the amount of unrolling once more. This time I tried to use smaller
numbers because I never gave 2-4 a real chance. After trying 4, I got a time of around 0.0201, then tried two then came back to one
and got my most optimal time of 0.019059s (26.4% performance increase compared to part 1). I am not really sure why this worked out this
way, especially since I started out with just one apple call in the beginning. I still take away that unrolling is a good start for 
micro-optimization moving forward. I placed the second call, commented out, at the bottom of the loop to give an idea of what it looked like for the 5-20 cases. Of course, I incrememnted i after every call except for the final one. On one hand, I am glad to have achieved a last minute performance bump, on the other, it is not very satisfying to see it play out like this, but I suppose it may have something to do
with changes I made elsewhere in the code.