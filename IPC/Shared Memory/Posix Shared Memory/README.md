### Example of shared memory between unrelated processes.
Posix provides two ways to share memory between unrelated processes.
> 1. Memory-mapped file: a file is opened by `open`, and the resulting descriptor is mapped into the address space of the process by `mmap`. Memory-mapped files can also
also be shared between unrelated processes.<br />
> 2. Shared memory objects: the fuction `shm_open` open a Posix.1 IPC name (perhaps a pathname in the filesystem), returning a descriptor that is then mapped into the address
space of the process by `mmap`.

The two-step process involved with Posix shared memory requires.
> 1. calling `shm_open`, specifying a name argument, to either  create a new shared memory object or to open an existing shared memory object, followed by<br />
> 2. calling `mmap` to map the shared memory into the address space of the calling process.

When dealing with `mmap`, the size of regular file or a shared memory object can be changed by calling `ftruncate`.
