/*
 *
 * Copyright (C) 2021 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 */

#ifndef _MSGBOX_H
#define _MSGBOX_H
#include"gui.h"
typedef bool(*msgbox_callback)(uint16_t id,const char*btn,void*user_data);
extern struct msgbox*msgbox_create_yesno(msgbox_callback callback,const char*content,...) __attribute__((format(printf,2,3)));
extern struct msgbox*msgbox_create_ok(msgbox_callback callback,const char*content,...) __attribute__((format(printf,2,3)));
extern struct msgbox*msgbox_create_custom(msgbox_callback callback,const char**btn,const char*content,...) __attribute__((format(printf,3,4)));
extern struct msgbox*msgbox_alert(const char*content,...) __attribute__((format(printf,1,2)));
extern void msgbox_set_user_data(struct msgbox*box,void*user_data);
#endif
