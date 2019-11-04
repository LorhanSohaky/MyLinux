#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/kd.h>
#include <linux/vt.h>
#include <linux/console_struct.h>

#define DRIVER_AUTHOR "Lorhan Sohaky"
#define DRIVER_DESC   "A simple test"
#define DRIVER_LICENSE "MIT"


#define RESTORE_LEDS 0xFF

extern int fg_console;

struct tty_driver *driver;
char status = 0;

/*
 * Bit 2 : CapsLock LED
 * Bit 1 : NumLock LED
 * Bit 0 : ScrollLock LED
 */

static unsigned char LED[] = { 0x04, 0x02, 0x01 };


int init_module(void){
	int i;
	int led = LED[0];

	printk(KERN_DEBUG "Iniciado modulo para acender LED\n");

	for (i = 0; i < MAX_NR_CONSOLES; i++) {
		if (!vc_cons[i].d)
			break;
	}

	driver = vc_cons[fg_console].d->port.tty->driver;

	((driver->ops)->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED, led);

	return 0;
}

void cleanup_module(void){
	((driver->ops)->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED, RESTORE_LEDS);
	printk(KERN_DEBUG "Finalizado modulo de acender LED\n");
}

MODULE_LICENSE(DRIVER_LICENSE);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);

