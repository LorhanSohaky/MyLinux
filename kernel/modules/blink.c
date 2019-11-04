#include <linux/init.h>
#include <linux/kd.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/tty.h>
#include <linux/vt.h>

#include <linux/console_struct.h>
#include <linux/timer.h>

#define DRIVER_AUTHOR "Lorhan Sohaky"
#define DRIVER_DESC "A simple test"
#define DRIVER_LICENSE "MIT"

#define RESTORE_LEDS 0xFF

extern int fg_console;

int g_time_interval = 100;

struct timer_list my_timer;

struct tty_driver *driver;
char status = 0;

/*
 * Bit 2 : CapsLock LED
 * Bit 1 : NumLock LED
 * Bit 0 : ScrollLock LED
 */

static unsigned char LED[] = {0x02, 0x04, 0x01};

static void my_timer_func( struct timer_list *ptr ) {
	static int num = 0;
	int state	   = LED[num];

	num = ( num + 1 ) % 3;

	( ( driver->ops )->ioctl )( vc_cons[fg_console].d->port.tty, KDSETLED, state );

	mod_timer( &my_timer, jiffies + msecs_to_jiffies( g_time_interval ) );
}

int init_module( void ) {
	int i;

	printk( KERN_DEBUG "Iniciado modulo para piscar LED\n" );

	for( i = 0; i < MAX_NR_CONSOLES; i++ ) {
		if( !vc_cons[i].d )
			break;
	}

	driver = vc_cons[fg_console].d->port.tty->driver;

	/* Starting the timer */
	timer_setup( &my_timer, my_timer_func, 0 );
	mod_timer( &my_timer, jiffies + msecs_to_jiffies( g_time_interval ) );

	return 0;
}

void cleanup_module( void ) {
	del_timer( &my_timer );

	( ( driver->ops )->ioctl )( vc_cons[fg_console].d->port.tty, KDSETLED, RESTORE_LEDS );
	printk( KERN_DEBUG "Finalizado modulo de piscar LED\n" );
}

MODULE_LICENSE( DRIVER_LICENSE );

MODULE_AUTHOR( DRIVER_AUTHOR );
MODULE_DESCRIPTION( DRIVER_DESC );
