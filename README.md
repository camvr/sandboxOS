# sandboxOS
A bare-bones kernel for developed from scratch purely for educational/entertainment purposes and to explore my interest in operating systems.

## Development
The kernel is written in C++ using gcc cross-compiled for i686-ELF. Therefore, you must have compile gcc for i686-ELF. For specific steps, refer to http://wiki.osdev.org/GCC_Cross-Compiler.

### Dependencies
The tools required to build this kernel are
- `i686-elf-gcc` *Note: When compiling gcc be sure to include c++ in the languages flag*
- `GNU make`

Optionally, these dependencies are required for certain make tasksL
- `qemu` for testing the build
- `GRUB`, `xorriso` for building a bootable `iso`

### Building
- `make` to build the kernel. 
- `make run` to run the binary on `qemu`.
- `make cdrom` to build a bootable `iso` with `grub`
- `make run-cdrom` to boot qemu from the `iso`
- `make clean` to remove the object files and other disposable files

## Resources
Much of the kernel is developed referencing [osdev.org](http://wiki.osdev.org/Main_Page) for its excellent resources and community guides. Certainly a great starting place for anyone looking into operating system development.

Some resources in particular used are as follows:
- http://wiki.osdev.org/GCC_Cross-Compiler
- http://wiki.osdev.org/Bare_Bones