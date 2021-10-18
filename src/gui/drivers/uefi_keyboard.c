/*
 *
 * Copyright (C) 2021 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 */

#ifdef ENABLE_GUI
#ifdef ENABLE_UEFI
#include<stdio.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<stddef.h>
#include<stdlib.h>
#include<stdbool.h>
#include<Library/UefiBootServicesTableLib.h>
#include<Protocol/SimpleTextIn.h>
#define TAG "uefikeyboard"
#include"gui.h"
#include"logger.h"
#include"defines.h"
#include"gui/guidrv.h"
static bool keyboard_read(lv_indev_drv_t*indev_drv,lv_indev_data_t*data){
	EFI_SIMPLE_TEXT_INPUT_PROTOCOL*keyboard=indev_drv->user_data;
	EFI_INPUT_KEY p;
	data->state=LV_INDEV_STATE_REL;
	if(!EFI_ERROR(keyboard->ReadKeyStroke(keyboard,&p))){
		data->state=0;
		if(p.ScanCode!=0){
			if(lv_group_get_editing(gui_grp))switch(p.ScanCode){
				// why UP and DOWN map to LEFT and RIGHT?
				// because volume keys only have UP and DOWN.
				case SCAN_UP:
				case SCAN_LEFT:data->key=LV_KEY_LEFT;break;
				case SCAN_PAGE_UP:data->key=LV_KEY_UP;break;
				case SCAN_DOWN:
				case SCAN_RIGHT:data->key=LV_KEY_RIGHT;break;
				case SCAN_PAGE_DOWN:data->key=LV_KEY_DOWN;break;
			}else switch(p.ScanCode){
				case SCAN_UP:
				case SCAN_LEFT:
				case SCAN_PAGE_UP:data->key=LV_KEY_PREV;break;
				case SCAN_DOWN:
				case SCAN_RIGHT:
				case SCAN_PAGE_DOWN:data->key=LV_KEY_NEXT;break;
			}
		}else if(p.UnicodeChar!=0)switch(p.UnicodeChar){
			case ' ':case '\n':case '\r':data->key=LV_KEY_ENTER;break;
		}else return false;
		data->state=LV_INDEV_STATE_PR;
	}
	return false;
}
int keyboard_register(){
	bool found=false;
	UINTN cnt=0;
	EFI_HANDLE*hands=NULL;
	EFI_STATUS st=gBS->LocateHandleBuffer(
		ByProtocol,
		&gEfiSimpleTextInProtocolGuid,
		NULL,&cnt,&hands
	);
	if(EFI_ERROR(st))return trlog_warn(-1,"locate keyboard failed: %lld",st);
	for(UINTN i=0;i<cnt;i++){
		EFI_SIMPLE_TEXT_INPUT_PROTOCOL*keyboard=NULL;
		if(EFI_ERROR(gBS->HandleProtocol(
			hands[i],
			&gEfiSimpleTextInProtocolGuid,
			(VOID**)&keyboard
		))||!keyboard)continue;
		static lv_indev_drv_t drv;
		lv_indev_drv_init(&drv);
		drv.type=LV_INDEV_TYPE_KEYPAD;
		drv.read_cb=keyboard_read;
		drv.user_data=keyboard;
		lv_indev_set_group(lv_indev_drv_register(&drv),gui_grp);
		tlog_debug("found uefi keyboard %p",keyboard);
		found=true;
	}
	return found?0:trlog_warn(-1,"no uefi keyboard found");
}
#endif
#endif
