#include <linux/init.h>
#include <linux/kd.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/tty.h>
#include <linux/vt.h>

#include <linux/console_struct.h>
#include <linux/timer.h>

#include <linux/gpio.h>

#define DRIVER_AUTHOR "Lorhan Sohaky"
#define DRIVER_DESC "A simple test"
#define DRIVER_LICENSE "GPL"

extern int fg_console;

int interval = 100;

struct timer_list my_timer;

int led = 16;

static void my_timer_func( struct timer_list *ptr ) {
	static int state = 0;

	printk( KERN_NOTICE "pisca pisca %i", state );
	gpio_direction_output(led, state);
	state = !state;

	mod_timer( &my_timer, jiffies + msecs_to_jiffies( interval ) );
}

static ssize_t dev_write( struct file *file, const char __user *buf, size_t count, loff_t *offset );

static struct file_operations simple_driver_fops = {.owner = THIS_MODULE, .write = dev_write};

static int device_file_major_number = 333;
static const char device_name[]		= "blink-led";
static int register_device( void ) {
	int result = 0;
	
	gpio_request(led, NULL);

	printk( KERN_NOTICE "blink-led: register_device() is called." );
	result = register_chrdev( 0, device_name, &simple_driver_fops );

	if( result < 0 ) {
		printk( KERN_WARNING "blink-led:  can\'t register character device with errorcode = %i",
				result );
		return result;
	}

	device_file_major_number = result;
	printk(
		KERN_NOTICE
		"blink-led: registered character device with major number = %i and minor numbers 0...255",
		device_file_major_number );
	return 0;
}

void unregister_device( void ) {
	printk( KERN_NOTICE "blink-led: unregister_device() is called" );
	if( device_file_major_number != 0 ) {
		unregister_chrdev( device_file_major_number, device_name );
	}
}

int my_init( void ) {
	register_device();

	return 0;
}

void my_exit( void ) {
	del_timer( &my_timer );

	//gpio_free(led);
	printk( KERN_DEBUG "Finalizado modulo de piscar LED\n" );
}

static ssize_t
dev_write( struct file *file, const char __user *buf, size_t count, loff_t *offset ) {
	size_t maxdatalen = 30, ncopied;
	uint8_t databuf[maxdatalen];

	if( count < maxdatalen ) {
		maxdatalen = count;
	}

	ncopied = copy_from_user( databuf, buf, maxdatalen );

	databuf[maxdatalen] = 0;

	printk( "Data from the user: %s\n", databuf );

	if( databuf[0] == '0' ) {
		my_exit();
	} else {
		printk( KERN_DEBUG "Iniciado modulo para piscar LED\n" );

		interval = databuf[1];

		/* Starting the timer */
		timer_setup( &my_timer, my_timer_func, 0 );
		mod_timer( &my_timer, jiffies + msecs_to_jiffies( interval ) );
	}

	return count;
}

module_init( my_init );
module_exit( my_exit );

MODULE_LICENSE( DRIVER_LICENSE );

MODULE_AUTHOR( DRIVER_AUTHOR );
MODULE_DESCRIPTION( DRIVER_DESC );
