build-kernel:
	$(shell mkdir -p ${target_out_kernel})
	cp -f configs/kernel_config uclinux/.config
	make -C uclinux
	cd uclinux/arch/arm/boot/; ./make_uboot_ximage
	cp uclinux/arch/arm/boot/xipuImage.bin out/kernel/