all: Bootloader Kernel32 Kernel64 Utility Disk.img

Bootloader:
	@echo ==build Bootloader===

	make -C 00.Bootloader
	@echo ==build finished==

Kernel32:
	@echo ==build Kernel32===

	make -C 01.Kernel32
	@echo ==build finished==

Kernel64:
	@echo ==build Kernel64===

	make -C 02.Kernel64
	@echo ==build finished==


Utility:
	@echo ==build Utility===

	make -C 04.Utility
	@echo ==build finished==
	
Disk.img: 00.Bootloader/Bootloader.bin 01.Kernel32/Kernel32.bin 02.Kernel64/Kernel64.bin
	./ImageMaker $^


clean:
	make -C 00.Bootloader clean
	make -C 01.Kernel32 clean
	make -C 02.Kernel64 clean
	make -C 04.Utility clean
	rm -f Disk.img
	rm -f ImageMaker
