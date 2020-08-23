# Shared Memory
Shared memory is the fastest form of IPC available. Once the memory is mapped into the address space of the processes that are sharing the memory region, no kernel involvement occurs in passing data between the processes.<br />
The `mmap` function maps either a file or a Posix shared memory object into the address space of a process. Address can specify the starting address within the process of where the descriptor `fd` should be mapped.
Normally, this is specified as a null pointer, telling the kernel to choose the starting address. 
### This section shows example of sharing memory between a parent and child by `mmap` call before calling fork.
Memory-mapped files are handled specially by `fork`, in that memory mappings created by the parent before calling `fork` are shared by child. Therefore, opening the file and calling `mmap` with the MAP_SHARED flag provide a piece of memory that is shared between the parent and child.
