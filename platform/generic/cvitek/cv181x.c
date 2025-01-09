/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2025 Thomas Makin.
 *
 * Authors:
 *   Thomas Makin <halorocker89@gmail.com>
 */

#include <platform_override.h>
#include <sbi/sbi_system.h>
#include <sbi/riscv_io.h>
#include <sbi_utils/fdt/fdt_helper.h>

static inline void writel_reg(u32 value, uintptr_t base, u32 reg) {
	writel(value, (volatile void *)(base + reg));
}

static inline u32 readl_reg(uintptr_t base, u32 reg) {
	return readl((volatile void *)(base + reg));
}

static int cv181x_system_reset_check(u32 type, u32 reason)
{
	return 1;
}

#define REG_RTC_CTRL_BASE 0x05025000
#define   RTC_CTRL0_UNLOCKKEY 0x4
#define   RTC_CTRL0           0x8

#define REG_RTC_BASE 0x05026000
#define   RTC_EN_WARM_RST_REQ 0xCC

static void cv181x_system_reset(u32 type, u32 reason)
{
	u32 val;

	writel_reg(0x01, REG_RTC_BASE, RTC_EN_WARM_RST_REQ);
	while (readl_reg(REG_RTC_BASE, RTC_EN_WARM_RST_REQ) != 0x01);
	writel_reg(0xAB18, REG_RTC_CTRL_BASE, RTC_CTRL0_UNLOCKKEY);
	val = readl_reg(REG_RTC_CTRL_BASE, + RTC_CTRL0);
	val |= (0xFFFF0800 | (0x1 << 4));
	writel_reg(val, REG_RTC_CTRL_BASE, RTC_CTRL0);

	while (1);
}

static struct sbi_system_reset_device cv_reset = {
	.name = "cv181x_reset",
	.system_reset_check = cv181x_system_reset_check,
	.system_reset = cv181x_system_reset
};

static int cv181x_early_init(bool cold_boot, const void *fdt, const struct fdt_match *match)
{
	if (cold_boot)
		sbi_system_reset_add_device(&cv_reset);

	return 0;
}

static const struct fdt_match cvitek_cv181x_match[] = {
	{ .compatible = "cvitek,cv181x" },
	{ },
};

const struct platform_override cvitek_cv181x = {
	.match_table = cvitek_cv181x_match,
	.early_init = cv181x_early_init,
};
