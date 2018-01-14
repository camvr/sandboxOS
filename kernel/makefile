TARGET=i686-elf
HOSTARCH=i386

KERNEL_NAME=sandbox

CC=$(TARGET)-g++
ASM=$(TARGET)-as
LINKER=$(CC)
QEMU=qemu-system-$(HOSTARCH)

AFLAGS?=
CFLAGS?=-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
LDFLAGS?=-ffreestanding -O2
LIBS?=-nostdlib -lgcc

ARCHDIR=arch/$(HOSTARCH)

-include $(ARCHDIR)/make.config

AFLAGS:=$(AFLAGS) $(KERNEL_ARCH_AFLAGS)
CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)

KERNEL_OBJS=$(KERNEL_ARCH_OBJS) kernel/kernel.o
OBJS=$(KERNEL_OBJS)
LINK_LIST=$(LDFLAGS) $(KERNEL_OBJS) $(LIBS)

.PHONY: all clean cdrom run run-cdrom

all: $(KERNEL_NAME).bin

$(ARCHDIR)/boot.o: $(ARCHDIR)/boot.s
	$(ASM) $(ARCHDIR)/boot.s -o $@ $(AFLAGS)

kernel/kernel.o: kernel/kernel.cpp
	$(CC) -c kernel/kernel.cpp -o $@ $(CFLAGS)

$(KERNEL_NAME).bin: $(OBJS) $(ARCHDIR)/linker.ld
	$(LINKER) -T $(ARCHDIR)/linker.ld -o $@ $(LINK_LIST)

cdrom: $(KERNEL_NAME).bin
	$(shell echo "menuentry \"$(KERNEL_NAME)\" {\n\tmultiboot /boot/$(KERNEL_NAME).bin\n}" > grub.cfg)
	mkdir -p isodir/boot/grub
	cp $(KERNEL_NAME).bin isodir/boot/$(KERNEL_NAME).bin
	mv grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(KERNEL_NAME).iso isodir

run: $(KERNEL_NAME).bin
	$(QEMU) -kernel $(KERNEL_NAME).bin

run-cdrom: $(KERNEL_NAME).iso
	$(QEMU) -cdrom $(KERNEL_NAME).iso

clean:
	rm -f $(KERNEL_NAME).bin
	rm -f $(OBJS) *.o */*.o */*/*.o