# pages [A Survey of Garbage Collection Techniques](http://pages.cs.wisc.edu/~zhong/termproj_surveyGC.html)

## Abstract

This paper starts with some background information on memory allocation and deallocation in computing history, to arrive at the point why Garbage Collection (GC) is imperative in today’s computing world. It then discusses some benefit and cost issues associated with GC: time, space, and locality tradeoffs.

This paper surveys a number of classical GC algorithms: Reference Counting, Mark and Sweep, Copying, incremental and Generational GC. It categories the basic classes of the collectors, analyzes each approach’s pros and cons and in comparison with other algorithms. It also examines some implementation techniques and critical points, and demonstrates some optimizations that have been come up for them. Emphasis is placed upon the more promising Generational GC for it has been widely used.