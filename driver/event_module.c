#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/input.h>                	//包含input子系统的相关头文件
#include <mach/gpio.h>

static struct  input_dev *btn_dev;  	//定义一个input_dev结构指针

struct btn_t {
	int gpio;
	int key;
	char *name;
};
static struct btn_t btns[] = {
	{ EXYNOS4_GPX3(2), KEY_L , "K1" },
	{ EXYNOS4_GPX3(3), KEY_S , "K2" },
	{ EXYNOS4_GPX3(4), KEY_ENTER,"K3" },
	{ EXYNOS4_GPX3(5), KEY_LEFTSHIFT,"K4" },
};

static irqreturn_t btn_irq_handler(int irq, void *dev)
{
	struct btn_t * p = (struct btn_t *)dev;
	int pinval = 0;
	pinval = gpio_get_value(p->gpio); /* 松开 :  1 */
	if (pinval){
		input_report_key(btn_dev,p->key,1);  	//上报松开事件
		input_sync(btn_dev);					//上报同步事件
	}
	else{
		input_report_key(btn_dev,p->key,0);	//上报按下事件
		input_sync(btn_dev);					//上报同步事件
	}
	return IRQ_HANDLED;
}

static int __init btn_drv_init(void)
{
	int irq;
	int i;

	/* 分配一个input_dev结构体 */
	btn_dev = input_allocate_device();       //动态分配输入设备结构体

	/* 设置 能产生哪类事件 */
	set_bit(EV_KEY, btn_dev->evbit);      //设置能产生按键事件
	set_bit(EV_REP, btn_dev->evbit);	   //设置能产生重复事件
	
	/* 能产生这类操作里的哪些事件: L,S,ENTER, LEFTSHIFT */
	set_bit(KEY_L, btn_dev->keybit);  
	set_bit(KEY_S, btn_dev->keybit);
	set_bit(KEY_ENTER, btn_dev->keybit);
	set_bit(KEY_LEFTSHIFT, btn_dev->keybit);

	/*  注册input dev */
	input_register_device(btn_dev);
	
	for(i = 0;i<4;i++){
		irq = gpio_to_irq(btns[i].gpio);
		request_irq(irq, btn_irq_handler, IRQ_TYPE_EDGE_BOTH, 
				btns[i].name, &btns[i]);
	}

	return 0;
}

static void __exit btn_drv_exit(void)
{
	int irq;
	int i;
	for(i = 0;i<4;i++){
		irq = gpio_to_irq(btns[i].gpio);
		free_irq(irq, &btns[i]);
	}
	input_unregister_device(btn_dev);  	//注销 input  dev
	input_free_device(btn_dev);		//释放input dev内存
}

module_init(btn_drv_init);
module_exit(btn_drv_exit);
MODULE_LICENSE("GPL");

