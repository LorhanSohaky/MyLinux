#include <linux/kernel.h>
#include <linux/module.h>

int init_module( void ) {
	printk( KERN_INFO "Primeiro modulo.\n" );
	return 0;
}

void cleanup_module( void ) {
	printk( KERN_INFO "Fim do primeiro modulo.\n" );
}
