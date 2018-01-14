TARGET=i686-elf
GCC=$(TARGET)-g++
CFLAGS=-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
ASM=$(TARGET)-as

BOOT=boot
BOOT_EXT=s
KERNEL=kernel
KERNEL_EXT=cpp
LINK=linker.ld

LINKER=$(TARGET)-g++
LFLAGS=-ffreestanding -O2 -nostdlib $(BOOT).o $(KERNEL).o -lgcc

KERNEL_NAME=sandbox

QEMU=qemu-system-i386


.PHONY: clean run cdrom

all: $(KERNEL_NAME).bin

$(BOOT).o: $(BOOT).$(BOOT_EXT)
	$(ASM) $(BOOT).$(BOOT_EXT) -o $(BOOT).o

$(KERNEL).o: $(KERNEL).$(KERNEL_EXT)
	$(GCC) -c $(KERNEL).$(KERNEL_EXT) -o $(KERNEL).o $(CFLAGS)

$(KERNEL_NAME).bin: $(BOOT).o $(KERNEL).o $(LINK)
	$(LINKER) -T $(LINK) -o $(KERNEL_NAME).bin $(LFLAGS)

run:
	ifeq ("$(wildcard $(./$(KERNEL_NAME).iso))","")
		$(QEMU) -cdrom $(KERNEL_NAME).iso
	else 
		ifeq ("$(wildcard $(./$(KERNEL_NAME).bin))","")
			$(QEMU) -kernel $(KERNEL_NAME).bin
		else
			$(error Binary or cd image required to run the kernel.)
		endif
	endif

cdrom: $(KERNEL_NAME).bin
	# something like...
	#mkdir -p isodir/boot/grub
	#cp $(KERNEL_NAME).bin isodir/boot/$(KERNEL_NAME).bin
	#echo "menuentry "$(KERNEL_NAME)" {" > isodir/boot/grub/grub.cfg
	#echo -e "\tmultiboot /boot/$(KERNEL_NAME).bin" >> isodir/boot/grub/grub.cfg
	#echo "}" >> isodir/boot/grub/grub.cfg
	#grub-mkrescue -o $(KERNEL_NAME).iso isodir
	#rm -rf isodir

clean:
	rm *.o $(KERNEL_NAME).bin