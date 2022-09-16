/*
 *
 * Copyright (C) 2021 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 */

#include"fs_internal.h"

extern void fsdrv_register_assets(bool deinit);
extern void fsdrv_register_socket(bool deinit);
extern void fsdrv_register_posix(bool deinit);
extern void fsdrv_register_zip(bool deinit);

list*fs_drivers=NULL;
list*fs_volumes;
list*fs_volume_infos;
mutex_t fsdrv_lock;
mutex_t fsvol_lock;
mutex_t fsvol_info_lock;
fs_initiator_function*fs_initiator[]={
	fsdrv_register_assets,
	#ifndef ENABLE_UEFI
	fsdrv_register_socket,
	fsdrv_register_posix,
	#endif
	#ifdef ENABLE_LIBZIP
	fsdrv_register_zip,
	#endif
	NULL
};
