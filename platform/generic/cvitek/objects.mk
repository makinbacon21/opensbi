#
# SPDX-License-Identifier: BSD-2-Clause
#
# Copyright (C) 2025 Thomas Makin.
#

carray-platform_override_modules-$(CONFIG_PLATFORM_CVITEK_CV181X) += cvitek_cv181x
platform-objs-$(CONFIG_PLATFORM_CVITEK_CV181X) += cvitek/cv181x.o
