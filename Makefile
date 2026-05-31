CC = gcc
LD = ld

CFLAGS = -m64 -march=x86-64 -g -ffreestanding -fno-stack-protector -fno-stack-check \
	-fno-lto -fno-pie -fno-pic -mno-red-zone -mno-mmx -mno-sse -mno-sse2 \
	-mcmodel=kernel -mno-80387 -mno-fp-ret-in-387 \
	-Iarch/x86_64/include -O2 -Wall -Wextra

LDFLAGS = -m elf_x86_64 -nostdlib -z max-page-size=0x1000 -T arch/x86_64/linker.ld

.PHONY: all clean run iso

all: build/kernel.bin

build/kernel.o: arch/x86_64/kernel/kernel.c arch/x86_64/include/video.h arch/x86_64/include/terminal.h
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/video.o: arch/x86_64/drivers/video.c arch/x86_64/include/video.h
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/terminal.o: arch/x86_64/drivers/terminal.c arch/x86_64/include/terminal.h
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/kernel.bin: build/kernel.o build/video.o build/terminal.o
	$(LD) $(LDFLAGS) $^ -o $@

iso: build/kernel.bin
	@mkdir -p build/iso_root
	cp build/kernel.bin build/iso_root/
	cp limine.cfg build/iso_root/
	
	@if [ ! -d "build/limine" ]; then \
	git clone https://github.com/limine-bootloader/limine.git --branch=v5.x-branch-binary --depth=1 build/limine; \
	make -C build/limine; \
	fi
	
	cp build/limine/limine-bios.sys build/limine/limine-bios-cd.bin build/limine/limine-uefi-cd.bin build/iso_root/
	
	xorriso -as mkisofs -b limine-bios-cd.bin \
	-no-emul-boot -boot-load-size 4 -boot-info-table \
	--efi-boot limine-uefi-cd.bin \
	-efi-boot-part --efi-boot-image --protective-msdos-label \
	build/iso_root -o build/gloria.iso
	
	./build/limine/limine bios-install build/gloria.iso

run: iso
	qemu-system-x86_64 -M q35 -m 256M -cdrom build/gloria.iso -boot d

clean:
	rm -rf build
