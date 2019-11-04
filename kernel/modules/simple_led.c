#include <linux/kernel.h>
#include <linux/module.h>

#define DRIVER_AUTHOR "Lorhan Sohaky"
#define DRIVER_DESC   "A simple module"
#define DRIVER_LICENSE "MIT"

int init_module( void ) {
	printk( KERN_INFO "Modulo simples.\n" );

	while (inb(0x64) & 0x2);
	outb(0xED, 0x60);
	while (inb(0x64) & 0x2);
	outb(0x07, 0x60);

	return 0;
}

void cleanup_module( void ) {
	printk( KERN_INFO "Fim do modulo modulo.\n" );
}

MODULE_LICENSE(DRIVER_LICENSE);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
