# ARM裸机（IML6ULL）

## 常用寄存器

| 寄存器名称   | 寄存器描述                 | 描述  /  常用位                                              |
| ------------ | -------------------------- | ------------------------------------------------------------ |
| cpsr         | 程序状态寄存器             | M[4:0]：控制处理器模式                                       |
| lr           | 链接寄存器                 | 存放当前子程序的返回地址                                     |
| GPIOx_DR     | GPIO数据寄存器             | 一个 GPIO 组最大只有 32 个 IO，因此 DR 寄存器中的每个位都对应 一个 GPIO。 |
| GPIOx_GDIR   | GPIO方向寄存器             | 此寄存器用来设置某个 IO 的工作方向，是输入还是输出。         |
| GPIOx_ICR1/2 | GPIO中断控制寄存器         | ICR1用于配置低16个GPIO， ICR2 用于配置高 16 个 GPIO.一个 GPIO 用两个 位，这两个位用来配置中断的触发方式（见开发指南314） |
| GPIOx_IMR    | GPIO中断屏蔽寄存器         | 一个 GPIO 对应一个位，IMR 寄存器用来控制 GPIO 的中断禁止和使能 |
| GPIOx_ISR    | GPIO中断状态寄存器         | 一个 GPIO 对应一个位，只要某个 GPIO 的中断发生，那么 ISR 中相应的位就会被置 1。处理完中断以后，必须清除中断标志位 |
| GPIOx_EDFE   | 边沿选择寄存器             | 覆盖 ICR1 和 ICR2 的设置.设1 则为双边沿触发。一位对应一个GPIO |
| CCM_CCGRx    | 外 设 时 钟 使 能 寄 存 器 | 一般设置全F即可                                              |



## Cortex-A7 MPCore

### Cortex-A 处理器运行模型

| 模式            | 描述                                                     |
| --------------- | -------------------------------------------------------- |
| User(USR)       | 用户模式，非特权模式，大部分程序运行的时候就处于此模式。 |
| FIQ             | 快速中断模式，进入 FIQ 中断异常                          |
| IRQ             | 一般中断模式。                                           |
| Supervisor(SVC) | 超级管理员模式，特权模式，供操作系统使用。               |
| Monitor(MON)    | 监视模式？这个模式用于安全扩展模式。                     |
| Abort(ABT)      | 数据访问终止模式，用于虚拟存储以及存储保护。             |
| Hyp(HYP)        | 超级监视模式？用于虚拟化扩展。                           |
| Undef(UND)      | 未定义指令终止模式。                                     |
| System(SYS)     | 系统模式，用于运行特权级的操作系统任务                   |

### Cortex-A 寄存器组

![image-20230907010037720](Image\1.png)

前面提到的每一种运行模式都有一组与之对应的寄存器组，这些寄存器中，有些是所有模式所共用的同一个物理寄存器，有一些是各模式自己所独立拥有的

![image-20230907010210012](Image\2.png)

#### 通用寄存器

R0~R15 就是通用寄存器，通用寄存器可以分为以下三类：

1. 未备份寄存器，即 R0~R7。
2. 备份寄存器，即 R8~R14。
3. 程序计数器 PC，即 R15。  R15 (PC)值 = 当前执行的程序位置 + 8 个字节

#### CPSR  程序状态寄存器

所有的处理器模式都共用一个 CPSR 物理寄存器，因此 CPSR 可以在任何模式下被访问。CPSR 是当前程序状态寄存器，该寄存器包含了条件标志位、中断禁止位、当前处理器模式标志 等一些状态位以及一些控制位。bit[4-0]是处理器模式控制位. 其他位见开发指南**293**页

## ARM汇编基本语法

### GNU 汇编语法

GNU 汇编语法适用于所有的架构，并不是 ARM 独享的，GNU 汇编由一系列的语句组成， 每行一条语句，每条语句有三个可选部分，如下：

```
label：instruction @ comment
```

**label** 即标号，表示地址位置，有些指令前面可能会有标号，这样就可以通过这个标号得到 指令的地址，标号也可以用来表示数据地址。注意 label 后面的“：”，任何以“：”结尾的标识 符都会被识别为一个标号。
**instruction** 即指令，也就是汇编指令或伪指令。
**comment** 就是注释内容。

#### 伪操作

用户可以使用.section 伪操作来定义一个段，汇编系统预定义了一些段名：

- .text 表示代码段。
- .data 初始化的数据段。
- .bss 未初始化的数据段。
- rodata 只读数据段。

汇编程序的默认入口标号是_start，不过我们也可以在链接脚本中使用 ENTRY 来指明其它 的入口点。常见的伪操作有：

- .byte 定义单字节数据，比如.byte 0x12。 

- .short 定义双字节数据，比如.short 0x1234。

-  .long 定义一个 4 字节数据，比如.long 0x12345678。 

- .equ 赋值语句，格式为：.equ 变量名，表达式，比如.equ num, 0x12，表示 num=0x12。

-  .align 数据字节对齐，比如：.align 4 表示 4 字节对齐。

-  .end 表示源文件结束。 

- .global 定义一个全局符号，格式为：.global symbol，比如：.global _start。

#### 常用汇编指令

| 指令                   | 描述                                                         |
| ---------------------- | ------------------------------------------------------------ |
| MOV R0，R1             | 将 R1 里面的数据复制到 R0 中。(后面的寄存器可以换成立即数前加#) |
| MRS R0, CPSR           | 将特殊寄存器 CPSR 里面的数据复制到 R0 中。                   |
| MSR CPSR, R0           | 将 R1 里面的数据复制到特殊寄存器 CPSR 里中。                 |
| LDR Rd, [Rn , #offset] | 从存储器 Rn+offset 的位置读取数据存放到 Rd 中。（LDR 加载立即数的时候要使用“=”，而不是“#”） |
| STR Rd, [Rn, #offset]  | 将 Rd 中的数据写入到存储器中的 Rn+offset 位置。              |
| PUSH <reg list>        | 将寄存器列表存入栈中。                                       |
| POP <reg list>         | 从栈中恢复寄存器列表。                                       |
| B <label>              | 跳转到 label                                                 |
| BX <Rm>                | 间接跳转，跳转到存放于 Rm 中的地址处，并且切换指令集         |
| BL <label>             | 跳转到标号地址，并将返回地址保存在 LR 中。                   |
| BLX <Rm>               | 结合 BX 和 BL 的特点，跳转到 Rm 指定的地址，并将返回地 址保存在 LR 中，切换指令集。 |

##### 算术运算指令

| 指令               | 计算公式                | 备注                         |
| ------------------ | ----------------------- | ---------------------------- |
| ADD Rd, Rn, Rm     | Rd = Rn + Rm            |                              |
| ADD Rd, Rn, #immed | Rd = Rn + #immed        | 加法运算，指令为 ADD         |
| ADC Rd, Rn, Rm     | Rd = Rn + Rm + 进位     |                              |
| ADC Rd, Rn, #immed | Rd = Rn + #immed +进位  | 带进位的加法运算，指令为 ADC |
| SUB Rd, Rn, Rm     | Rd = Rn – Rm            |                              |
| SUB Rd, #immed     | Rd = Rd - #immed        |                              |
| SUB Rd, Rn, #immed | Rd = Rn - #immed        | 减法                         |
| SBC Rd, Rn, #immed | Rd = Rn - #immed – 借位 |                              |
| SBC Rd, Rn ,Rm     | Rd = Rn – Rm – 借位     | 带借位的减法                 |
| MUL Rd, Rn, Rm     | Rd = Rn * Rm            | 乘法(32 位)                  |
| UDIV Rd, Rn, Rm    | Rd = Rn / Rm            | 无符号除法                   |
| SDIV Rd, Rn, Rm    | Rd = Rn / Rm            | 有符号除法                   |

##### 逻辑运算指令

| 指令               | 计算公式            | 备注     |
| ------------------ | ------------------- | -------- |
| AND Rd, Rn         | Rd = Rd &Rn         |          |
| AND Rd, Rn, #immed | Rd = Rn &#immed     |          |
| AND Rd, Rn, Rm     | Rd = Rn & Rm        | 按位与   |
| ORR Rd, Rn         | Rd = Rd \| Rn       |          |
| ORR Rd, Rn, #immed | Rd = Rn \| #immed   |          |
| ORR Rd, Rn, Rm     | Rd = Rn \| Rm       | 按位或   |
| BIC Rd, Rn         | Rd = Rd & (~Rn)     |          |
| BIC Rd, Rn, #immed | Rd = Rn & (~#immed) |          |
| BIC Rd, Rn , Rm    | Rd = Rn & (~Rm)     | 位清除   |
| ORN Rd, Rn, #immed | Rd = Rn \| (#immed) |          |
| ORN Rd, Rn, Rm     | Rd = Rn \| (Rm)     | 按位或非 |
| EOR Rd, Rn         | Rd = Rd ^ Rn        |          |
| EOR Rd, Rn, #immed | Rd = Rn ^ #immed    |          |
| EOR Rd, Rn, Rm     | Rd = Rn ^ Rm        | 按位异或 |

## I.MX6ULL IO初始化

1. 使能时钟，CCGR0~CCGR6这7个寄存器控制着6ULL所有外设时钟的使能。为了简单，设置CCGR0~CCGR6这7个寄存器全部为0XFFFFFFFF，相当于使能所有外设时钟。
2. IO复用，将寄存器IOMUXC_SW_**MUX**_CTL_PAD_GPIO1_IO03的bit3~0设置为0101=5，这样GPIO1_IO03就复用为GPIO。IO复用寄存器详情见参考手册
3. 寄存器IOMUXC_SW_**PAD**_CTL_PAD_GPIO1_IO03是设置GPIO1_IO03的电气属性。包括压摆率、速度、驱动能力、开漏、上下拉等。
4. 配置GPIO功能，设置输入输出。设置GPIO1_DR寄存器bit3为1，也就是设置为输出模式。设置GPIO1_DR寄存器的bit3，为1表示输出高电平，为0表示输出低电平。