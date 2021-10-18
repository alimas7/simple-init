/*
 *
 * Copyright (C) 2021 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 */

#ifndef CONFD_H
#define CONFD_H
#include<stdint.h>
#include<stdbool.h>
#include<sys/types.h>
#ifdef ENABLE_UEFI
#include<Protocol/SimpleFileSystem.h>
#endif
#include"pathnames.h"
#define DEFAULT_CONFD _PATH_RUN"/confd.sock"

// config data types
enum conf_type{
	TYPE_KEY     =0xAF01,
	TYPE_STRING  =0xAF02,
	TYPE_INTEGER =0xAF03,
	TYPE_BOOLEAN =0xAF04,
};

// src/confd/client.c: open confd socket
extern int open_confd_socket(bool quiet,char*tag,char*path);

// src/confd/client.c: close confd socket
extern void close_confd_socket();

// src/confd/client.c: set confd socket fd
extern int set_confd_socket(int fd);

#ifndef ENABLE_UEFI
// src/confd/client.c: start a config daemon in protect mode
extern int start_confd(char*tag,pid_t*p);
#endif

// src/confd/client.c: terminate remote confd
extern int confd_quit();

// src/confd/client.c: dump config store to logger
extern int confd_dump();

// src/confd/client.c: save config store as a file
#ifdef ENABLE_UEFI
extern int confd_save_file(EFI_FILE_PROTOCOL*fd,const char*file);
#else
extern int confd_save_file(const char*file);
#endif

// src/confd/client.c: load config store from a file
#ifdef ENABLE_UEFI
extern int confd_load_file(EFI_FILE_PROTOCOL*fd,const char*file);
#else
extern int confd_load_file(const char*file);
#endif

// src/confd/client.c: set default config file path
extern int confd_set_default_config(const char*file);

// src/confd/client.c: delete a config item
extern int confd_delete(const char*path);

// src/confd/client.c: list config items
extern char**confd_ls(const char*path);

// src/confd/client.c: get type of config item
extern enum conf_type confd_get_type(const char*path);

#define DECLARE_FUNC(func,arg,type,ret) \
	extern ret func(const char*path,type arg); \
	extern ret func##_base(const char*base,const char*path,type arg);\
	extern ret func##_dict(const char*base,const char*key,const char*path,type arg);\
	extern ret func##_array(const char*base,int index,const char*path,type arg);
DECLARE_FUNC(confd_set_integer, data,int64_t,int);
DECLARE_FUNC(confd_set_string,  data,char*,  int);
DECLARE_FUNC(confd_set_boolean, data,bool,   int);
DECLARE_FUNC(confd_get_string,  data,char*,  char*);
DECLARE_FUNC(confd_get_integer, data,int64_t,int64_t);
DECLARE_FUNC(confd_get_boolean, data,bool,   bool);

// open default socket
#define open_default_confd_socket(quiet,tag) open_confd_socket(quiet,tag,DEFAULT_CONFD)

#endif
