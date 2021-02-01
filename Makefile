CC = g++
GPPPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
ASPARAMS = --32
OBJS = loader.o gdt.o interruptstubs.o interrupts.o port.o keyboard.o kernel.o

%.o: %.cpp
	$(CC) $(GPPPARAMS) -o $@ -c $<

%.o: %.s
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
	rm -f $(OBJS) mykernel.bin mykernel.iso