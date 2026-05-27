#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>

#define REG_SET(r,b) ((r) |=  BIT(b))
#define REG_CLR(r,b) ((r) &= ~BIT(b))
#define REG_TOG(r,b) ((r) ^=  BIT(b))
#define REG_TST(r,b) (((r) >> (b)) & 1U)

int main(void)
{
    uint32_t reg = 0UL;
    REG_SET(reg, 3); REG_SET(reg, 7);
    printk("set  3,7: 0x%08X\n", reg);
    REG_CLR(reg, 3);
    printk("clr  3:   0x%08X\n", reg);
    REG_TOG(reg, 7);
    printk("tog  7:   0x%08X\n", reg);
    printk("tst  0:   %u\n", REG_TST(reg, 0));

    uint32_t status = 0x03B4UL;
    uint8_t field = (uint8_t)((status >> 4) & GENMASK(3, 0));
    printk("field[7:4] = 0x%X\n", field);

    while (1) { k_msleep(5000); }
    return 0;
}
