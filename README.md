# Philosophers 
- is a project that introduces basics of multi-threaded processes and parallelism

# What mean Thread?
- A thread can be thought of as a procedure that works in parallel with others
procedure within a process. To run in parallel with the rest of the process, a thread must have
data structures ie its own execution environment, to realize
your own flow of control. Each procedure to be performed in parallel will be a thread.

# Advantages and Disadvantages of Using Thread

Advantages:
- Multiple streams of execution.
- simple handling of asynchronous events (I / O for example)
- Fast communications. All threads in a process share the same space as
  addressing, so inter-thread communications are simpler than communications
  between processes.
- Fast context switch. In the passage from one thread to another of the same process
  a large part of the environment is maintained.

