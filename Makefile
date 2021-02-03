CC = g++
GPPPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
ASPARAMS = --32
OBJS = obj/loader.o \
		obj/gdt.o \
		obj/mmu.o \
		obj/drivers/driver.o \
		obj/hardwarecom/interruptstubs.o \
		obj/hardwarecom/interrupts.o \
		obj/hardwarecom/port.o \
		obj/hardwarecom/pci.o \
		obj/multitasking.o \
		obj/drivers/keyboard.o \
		obj/drivers/mouse.o \
		obj/drivers/vga.o \
		obj/gui/widget.o \
		obj/gui/desktop.o \
		obj/gui/window.o \
		obj/kernel.o

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	$(CC) $(GPPPARAMS) -o $@ -c $<

obj/%.o: src/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $<

mykernel.bin: linker.ld $(OBJS)
	ld $(LDPARAMS) -T $< -o $@ $(OBJS)

install: mykernel.bin
	sudo cp $< /boot/mykernel.bin


mykernel.iso: mykernel.bin
	rm -rf iso
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot
	@echo 'set timeout=0' >> iso/boot/grub/grub.cfg
	@echo 'set default=0' >> iso/boot/grub/grub.cfg
	@echo '' >> iso/boot/grub/grub.cfg
	@echo 'menuentry "My Operating System" {' >> iso/boot/grub/grub.cfg
	@echo '	multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	@echo '	boot' >> iso/boot/grub/grub.cfg
	@echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso

run: mykernel.iso
	(killall VirtualBoxVM && sleep 1) || true
	VirtualBoxVM --startvm "My Operating System" &	

.PHONY: clean
clean:
	rm -rf obj mykernel.bin mykernel.iso