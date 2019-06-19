/*
 * Test Server
 *
 * Copyright IBM, Corp. 2011
 *
 * Authors:
 *  Anthony Liguori   <aliguori@us.ibm.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 *
 */

#ifndef QTEST_H
#define QTEST_H

#include "qemu-common.h"

extern bool qtest_allowed;

static inline bool qtest_enabled(void)
{
	return true;
    return qtest_allowed;
}

bool qtest_driver(void);

#ifndef CONFIG_FUZZ
void qtest_init(const char *qtest_chrdev, const char *qtest_log, Error **errp);
#else
void qtest_init_server(const char *qtest_chrdev, const char *qtest_log, Error **errp);
#endif

void qtest_server_recv(GString *inbuf);

#endif
