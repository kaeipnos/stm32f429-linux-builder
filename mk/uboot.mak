build-uboot:
	$(shell mkdir -p ${target_out_uboot})
	export CROSS_COMPILE=arm-uclinuxeabi-
	make -C $(uboot_dir) stm32429-disco_config
	make -C $(uboot_dir)
	cp $(uboot_dir)/u-boot.bin $(target_out)/uboot/u-boot.bin
