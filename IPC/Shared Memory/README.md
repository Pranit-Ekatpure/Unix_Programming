# Shared Memory
Shared memory is the fastest form of IPC available. Once the memory is mapped into the address space of the processes that are sharing the memory region, no kernel involvement occurs in passing data between the processes.

The **mmap** function maps either a file or a Posix shared memory object into the address space of a process. Address can specify the starting address within the process of where the descriptor **fd** should be mapped. 
Normally, this is specified as a null pointer, telling the kernel to choose the starting address. It returns starting address of mapped region if OK, MAP_FAILED on error. 
### This section shows example of sharing memory between a parent and child by `mmap` call before calling fork.
Here, **mmap** called to map the file that was just opened into the memory of this process. The first argument is a null pointer, telling the system to pick the starting address. The lenght is the size of an integer, and specified read-write access. By specifying a fourth argument if MAP_SHARED, any changes made by the parent will be seen by the child, and vice versa. The return value is the starting address of the memory region that will be shared, and store it in ptr. 
Memory-mapped files are handled specially by **fork**, in that memory mappings created by the parent before calling **fork** are shared by child. Therefore, opening the file and calling **mmap** with the MAP_SHARED flag provide a piece of memory that is shared between the parent and child.

Here we see that parent and child each have thier own copy of the pointer ptr, but each copy points to the same integer in shared memory.

### Compilation Steps
- To generate executable use command:
``` 
    make all
```
- To remove output files use command:
```
    make clean
```

### Running the example
- Run example as follows
```
    $ ./shm_parent_child
    child: 0    child starts first
    child: 1
    . . .
    child: 129
    parent: 130     child is stopped, parent starts
    parent: 131
    . . .
    . . .
    parent: 19999       final line of output

    $ ./shm_parent_child1
    (Output similar to shm_parent_child)
```
We show that the parent and child each have their own copy of the pointer `ptr`, but each copy points to the same integer in shared memory: the counter that both processes icrement.
