/*
 *
 * Copyright (C) 2021 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 */

#define _GNU_SOURCE
#include"str.h"
#include"confd.h"
#include"logger.h"
#define TAG "cmdline"

int cmdline_dpi(char*k __attribute__((unused)),char*v){
	int r=parse_int(v,0);
	if(r<0||r>1000)
		return trlog_warn(0,"invalid dpi '%s'",v);
	confd_set_integer("runtime.cmdline.dpi",r);
	return 0;
}

int cmdline_dpi_force(char*k __attribute__((unused)),char*v){
	int r=parse_int(v,0);
	if(r<0||r>1000)
		return trlog_warn(0,"invalid dpi_force '%s'",v);
	confd_set_integer("runtime.cmdline.dpi_force",r);
	return 0;
}

int cmdline_backlight(char*k __attribute__((unused)),char*v){
	confd_set_string("runtime.cmdline.backlight",v);
	return 0;
}
