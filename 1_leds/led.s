.global _start /*全局标号*/

/*
5 * 描述： _start 函数，程序从此函数开始执行此函数完成时钟使能、
6 * GPIO 初始化、最终控制 GPIO 输出低电平来点亮 LED 灯。
7 */

_start:
/*使能时钟*/
ldr r0, =0X020C4068
ldr r1, =0XFFFFFFFF
str r1, [r0]

ldr r0, =0X020C406C
str r1, [r0]

ldr r0, =0X020C4070
str r1, [r0]

ldr r0, =0X020C4074
str r1, [r0]

ldr r0, =0X020C4078
str r1, [r0]

ldr r0, =0X020C407C
str r1, [r0]

ldr r0, =0X020C4080
str r1, [r0]

/*设置 GPIO1_IO03 复用为 GPIO1_IO03*/
ldr r0, =0X020E0068
ldr r1, =0X5
str r1,[r0]

/* 3、配置 GPIO1_IO03 的 IO 属性 
*bit 16:0 HYS 关闭
*bit [15:14]: 00 默认下拉
*bit [13]: 0 kepper 功能
*bit [12]: 1 pull/keeper 使能
*bit [11]: 0 关闭开路输出
*bit [7:6]: 10 速度 100Mhz
*bit [5:3]: 110 R0/6 驱动能力
*bit [0]: 0 低转换率
*/
ldr r0, =0X020E02F4
ldr r1, =0X10B0
str r1,[r0]

/*设置 GPIO1_IO03 为输出*/
ldr r0, =0X0209C004
ldr r1, =0X0000008
str r1,[r0]

/*打开LED0*/
ldr r0, =0X0209C000
ldr r1, =0
str r1,[r0]

loop:
    b loop