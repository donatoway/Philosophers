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
- BE careful!!
- occorre gestire la mutua esclusione per evitare che più thread utilizzino in maniera
scoordinata i dati condivisi, modificandoli in momenti sbagliati.

# MUTEX
A mutex provides mutual exclusion, either producer or consumer can have the key (mutex) and proceed with their work. As long as the buffer is filled by the producer, the consumer needs to wait, and vice versa. to block a procedure use mutex_lock () and to unblock mutex_unlock ()

# SEMAPHORE
A semaphore is a generalized mutex. In lieu of a single buffer, we can split the 4 KB buffer into four 1 KB buffers (identical resources). A semaphore can be associated with these four buffers. The consumer and producer can work on different buffers at the same time. to block a procedure use sem_wait () and to unblock sem_post ()

# PARALLELISM
parallelism is an important technology for increasing performance. here you can read some information about it
https://www.perforce.com/blog/qac/multithreading-parallel-programming-c-cpp


