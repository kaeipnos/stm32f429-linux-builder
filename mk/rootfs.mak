build-rootfs: localprgs busybox $(rootfs_target)

busybox:
	$(shell mkdir -p ${target_out_busybox})
	$(shell mkdir -p ${target_out_romfs})
	cp -f configs/busybox_config $(target_out_busybox)/.config
	make -C $(busybox_dir) \
		O=$(target_out_busybox) oldconfig
	make -C $(target_out_busybox) \
		ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) \
		CFLAGS=$(ROOTFS_CFLAGS) SKIP_STRIP=y \
		CONFIG_PREFIX=$(target_out_romfs) install
localprgs:
	make -C $(localprgs_dir) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) install

$(rootfs_target): $(rootfs_dir) $(target_out_busybox)/.config
	cp -f $(localprgs_dir)/bin/* $(target_out_romfs)/usr/bin
	cp -af $(rootfs_dir)/* $(target_out_romfs)
	cp -f uclinux/fs/ext2/ext2.ko $(target_out_romfs)/lib/modules
	cp -f uclinux/fs/mbcache.ko $(target_out_romfs)/lib/modules
	cd $(target_out) && genromfs -v \
		-V "ROM Disk" \
		-f romfs.bin \
		-x placeholder \
		-d $(target_out_romfs) 2> $(target_out)/romfs.map
