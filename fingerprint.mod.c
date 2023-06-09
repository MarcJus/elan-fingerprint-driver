#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif


static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x92997ed8, "_printk" },
	{ 0xa82d41ff, "usb_deregister_dev" },
	{ 0xe638dc6a, "usb_register_dev" },
	{ 0x92271252, "usb_bulk_msg" },
	{ 0xd0da656b, "__stack_chk_fail" },
	{ 0xf301d0c, "kmalloc_caches" },
	{ 0x35789eee, "kmem_cache_alloc_trace" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x37a0cba, "kfree" },
	{ 0xaca7784b, "usb_register_driver" },
	{ 0x6d7d5d36, "usb_deregister" },
	{ 0x541a6db8, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("usb:v04F3p0C00d*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "DB39D86E8DBEDB33A355C7C");
