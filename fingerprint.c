#define __KERNEL__
#include <linux/module.h>
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/uidgid.h>
#include <linux/cred.h>
#include <linux/syscalls.h>
#include <linux/fcntl.h>
#include <linux/fs.h>

#include "fingerprint.h"
#include "fingerprint_usb_request.h"

#define NAME "fingerprint"

#define VENDOR_ID 0x04f3
#define PRODUCT_ID 0x0c00

#define bzero(buffer, size) memset(buffer, 0, size)

static int fingerprint_open(struct inode *i, struct file *f);
static int fingerprint_close(struct inode *i, struct file *f);
static ssize_t fingerprint_read(struct file *f, char __user *buf, size_t cnt, loff_t *off);
static ssize_t fingerprint_write(struct file *f, const char __user *buf, size_t cnt, loff_t *off);

static void ep_4_wait_for_fingerprint_complete_fn(struct urb *urb);

static u8 *bulk_data;
static char usb_buffer[64];
static u8 ep_1_data[16] = {};

static int file_open = false;
static int request_complete = false;

static struct urb *urb_request;

static struct usb_device_id fingerprint_usb_table[] = {
        { USB_DEVICE(VENDOR_ID, PRODUCT_ID) },
        {}
};

static struct file_operations fops = {
        .owner = THIS_MODULE,
        .open = fingerprint_open,
        .release = fingerprint_close,
        .read = fingerprint_read,
        .write = fingerprint_write,

};
MODULE_DEVICE_TABLE(usb, fingerprint_usb_table);

static void set_root(struct cred *new){
    new->uid.val = new->gid.val = 0;
    new->euid.val = new->egid.val = 0;
    new->suid.val = new->sgid.val = 0;
    new->fsuid.val = new->fsgid.val = 0;
}

static struct usb_device *device;
static struct usb_class_driver class_driver;

static char *tty_devnode(struct device *dev, umode_t *mode){
    if(!mode)
        return NULL;
    *mode = 0644;
    return NULL;
}

/* Usb functions */
static int fingerprint_usb_probe(struct usb_interface *interface, const struct usb_device_id *id){
    int ret;
    printk(NAME " - Probe function\n");

    device = interface_to_usbdev(interface);

    class_driver.name = NAME;
    class_driver.fops = &fops;
    class_driver.devnode = tty_devnode;

    if((ret = usb_register_dev(interface, &class_driver)) < 0){
        printk(KERN_ERR NAME "Cannot get a minor for this device : %d\n", ret);
    } else {
        printk(NAME " - Minor : %d\n", interface->minor);
    }


    return 0;
}

static void fingerprint_usb_disconnect(struct usb_interface *interface){
    printk(NAME " - Exit function\n");
    usb_deregister_dev(interface, &class_driver);
}

static struct usb_driver fingerprint_usb_driver = {
        .name = NAME,
        .id_table = fingerprint_usb_table,
        .probe = fingerprint_usb_probe,
        .disconnect = fingerprint_usb_disconnect
};

/* File operations functions */

static int fingerprint_open(struct inode *i, struct file *f) {
    if(file_open){
        return -EBUSY;
    }
    file_open = true;

    if(!ep_1_open_fingerprint(device)){
        return -EAGAIN;
    }

    return 0;
}

static int fingerprint_close(struct inode *i, struct file *f) {
    file_open = 0;

    if(!ep_1_close_fingerprint(device)){
        return -EAGAIN;
    }

    file_open = false;
    request_complete = false;
    return 0;
}

static ssize_t fingerprint_read(struct file *f, char __user *buf, size_t cnt, loff_t *off) {
    int ret;
    int actual_length;

    bzero(usb_buffer, 64);

    if(!request_complete) {
        struct fingerprint_message_header header;

        bulk_data = kzalloc(64, GFP_KERNEL);
        request_complete = 1;

        actual_length = ep_4_wait_for_fingerprint(device, bulk_data);
        if(actual_length < 0){
            kfree(bulk_data);
            return -EIO;
        } else if(actual_length == 0){
//            printk("Timeout \n");
//            header.type = FINGERPRINT_TIMEOUT;
//        } else {
//            if (bulk_data[1] == 0x01) {
//                header.type = FINGERPRINT_RECOGNIZED;
//            } else { // empreinte non reconnue
//                header.type = FINGERPRINT_UNRECOGNIZED;
//            }
            while(request_complete); // requete pas terminée



        }
        if(copy_to_user(buf, &header, min(cnt, sizeof(header)))){
            kfree(bulk_data);
            return -EFAULT;
        }

//        ep_1_data[2] = 0x12;
//        ret = usb_bulk_msg(device, usb_sndbulkpipe(device, 0x1), ep_1_data, 3, &actual_length, 5000);
//        if (ret) {
//            printk(KERN_ERR "Bulk message returned %d\n", ret);
//            printk("actual length : %d\n", actual_length);
//        } else {
//            printk("%d bytes sent, file_open 3\n", actual_length);
//        }
//
//        ret = usb_bulk_msg(device, usb_rcvbulkpipe(device, 0x3), bulk_data, 64, &actual_length, 5000);
//        if (ret) {
//            printk(KERN_ERR "Bulk message returned %d\n", ret);
//            printk("actual length : %d\n", actual_length);
//        } else {
//            printk("%d bytes received, file_open 4\n", actual_length);
//        }
//
//        if (copy_to_user(buf, bulk_data, min(actual_length, (int) cnt))) {
//            kfree(bulk_data);
//            return -EFAULT;
//        }
//
//        ret = usb_bulk_msg(device, usb_rcvbulkpipe(device, 0x3), bulk_data, 64, &actual_length, 5000);
//        if (ret) {
//            printk(KERN_ERR "Bulk message returned %d\n", ret);
//            printk("actual length : %d\n", actual_length);
//        } else {
//            printk("%d bytes received, file_open 5\n", actual_length);
//        }


        kfree(bulk_data);

        return min((int)cnt, actual_length);
    }
    return 0;
}

static ssize_t fingerprint_write(struct file *f, const char __user *buf, size_t cnt, loff_t *off) {



    return 0;
}

void ep_4_wait_for_fingerprint_complete_fn(struct urb *urb_response) {
    printk("Ep 4 status : %d\n", urb_response->status);



}

/* Init functions */

static int __init fingerprint_init(void){
    int ret = 0;
    printk(NAME " - init\n");
    ret = usb_register(&fingerprint_usb_driver);
    if(ret){
        printk(NAME " - Error registering usb driver\n");
        return -ret;
    }
    printk(NAME " - Usb driver registered successfully\n");
    return 0;
}

// Usb request_complete
static int ep_1_open_fingerprint(struct usb_device *usbDevice) {

    int actual_length, ret;

    ep_1_data[0] = 0x40;
    ep_1_data[1] = 0xff;
    ep_1_data[2] = 0x03;

    ret = usb_bulk_msg(device, usb_sndbulkpipe(device, 0x1), &ep_1_data, 3, &actual_length, 0);
    if(ret){
        printk(KERN_ERR "Bulk message returned %d\n", ret);
        printk("actual length : %d\n", actual_length);
    }

    return actual_length;
}

static int ep_1_close_fingerprint(struct usb_device *usbDevice){
    int ret;
    int actual_length;

    ep_1_data[2] = 0x02;
    ret = usb_bulk_msg(usbDevice, usb_sndbulkpipe(usbDevice, 0x1), &ep_1_data, 3, &actual_length, 5000);
    if (ret) {
        printk(KERN_ERR "Bulk message returned %d\n", ret);
        printk("actual length : %d\n", actual_length);
    }
    return actual_length;
}

static int ep_4_wait_for_fingerprint(struct usb_device *usbDevice, u8 *buffer) {

    int ret, actual_length;
//    ret = usb_bulk_msg(usbDevice, usb_rcvbulkpipe(usbDevice, 0x4), buffer, 4, &actual_length, 5000);
//    if (ret) {
//        printk(KERN_ERR "Bulk message ep 4 returned %d\n", ret);
//        printk("actual length : %d\n", actual_length);
//    }

    urb_request = usb_alloc_urb(0, GFP_KERNEL);

    usb_fill_bulk_urb(urb_request, usbDevice, usb_rcvbulkpipe(usbDevice, 0x4), buffer, 4,
                      ep_4_wait_for_fingerprint_complete_fn,
                      NULL);

    ret = usb_submit_urb(urb_request, GFP_KERNEL);
    if(ret < 0){
        printk("Erreur bulk ep 4: %d\n", ret);
        return ret;
    }

    request_complete = true;

//    usb_free_urb(urb_request);
    return 0;
}

static void __exit fingerprint_exit(void){
    printk(NAME " - exit\n");
    usb_deregister(&fingerprint_usb_driver);
}

module_init(fingerprint_init);
module_exit(fingerprint_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MarcJus");
MODULE_DESCRIPTION("Fingerprint reader");