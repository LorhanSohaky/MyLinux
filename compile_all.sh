#!/bin/bash

##
#  Run with fakeroot eg. fakeroot ./compile_all.sh
#  Then run qemu-system-x86_64 -kernel kernel/bzImage -initrd kernel/initramfs.cpio
##

# Create directory
	echo 'Creating directories'
	mkdir -p kernel
	mkdir -p kernel/distro
	mkdir -p kernel/distro/bin
	mkdir -p kernel/distro/dev
	mkdir -p kernel/distro/usr/share/kbd/keymaps/
	mkdir -p kernel/distro/etc/init.d/
	mkdir -p kernel/distro/proc
	mkdir -p kernel/distro/sys
	mkdir -p kernel/distro/modules

cd kernel


# Download source code
if [ ! -f linux-5.3.2.tar.xz ]
then
	echo 'Downloading kernel Linux 5.3.2'
	wget https://cdn.kernel.org/pub/linux/kernel/v5.x/linux-5.3.2.tar.xz
	
	# Extract file
	echo 'Extracting kernel'
	tar -xf linux-5.3.2.tar.xz
fi


# Compile Linux
if [ ! -f linux-5.3.2/arch/x86/boot/bzImage ]
then
	echo 'Compiling kernel'
	cd linux-5.3.2/
	make defconfig
	make -j 8
	cp arch/x86/boot/bzImage ../

	cd ../
fi

if [ ! -f modules/*.ko ]
then
	echo 'Compiling modules'
	cd modules
	make
	cd ..
fi


# Download BusyBox
if [ ! -f busybox-1.31.0.tar.bz2 ]
then
	echo 'Downloading Busybox 1.31.0'
	wget https://busybox.net/downloads/busybox-1.31.0.tar.bz2 

	# Extract file
	echo 'Extracting Busybox'
	tar -xf busybox-1.31.0.tar.bz2
fi

# Compile Busybox
if [ ! -f busybox-1.31.0/busybox ]
then
	echo 'Compiling Busybox'
	cd busybox-1.31.0/
	make defconfig
	sed -e 's/.*CONFIG_STATIC.*/CONFIG_STATIC=y/' -i .config
	make -j 8

	echo 'Installing comandos do Busybox'
	./busybox --install ../distro/bin

	cd ../
fi

# Download keymap
if [ ! -f distro/usr/share/kbd/keymaps/br-abnt2.bmap ]
then
	echo 'Downloading Keymap'
	wget https://dev.alpinelinux.org/bkeymaps/br/br-abnt2.bmap
	mv br-abnt2.bmap distro/usr/share/kbd/keymaps/br-abnt2.bmap
fi

# Copy modules
if [ ! -f distro/modules/*.ko ]
then
	echo 'Copying modules'
	cp modules/*.ko distro/modules
fi

# Configure distro
echo 'Configuring distro'
cd distro

if [ ! -f init ]
then
	mv bin/init .
	mv bin/run-init .
fi

echo '#!/bin/sh' > etc/init.d/rcS
echo 'mount -t proc proc proc/' >> etc/init.d/rcS
echo 'mount --rbind /sys sys/' >> etc/init.d/rcS
echo 'mount --rbind /dev dev/' >> etc/init.d/rcS
echo 'loadkmap < usr/share/kbd/keymaps/br-abnt2.bmap' >> etc/init.d/rcS
chmod +x etc/init.d/rcS
chown root etc/init.d/rcS	


## Configure tty
for i in {1..4}; do
	mknod dev/tty$i c 4 $i
done

find . | cpio -H newc -o > ../initramfs.cpio

echo 'Finished!'
