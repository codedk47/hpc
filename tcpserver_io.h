#ifndef TCPSERVER_IO_H
#define TCPSERVER_IO_H

#include "phpserver.h"

zend_module_entry tcpserver_io;
zend_class_entry *tcpserver_io_ce;

ZEND_BEGIN_ARG_INFO_EX(arginfo_a_string, 0, 0, 1)
ZEND_ARG_TYPE_INFO(0, q, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_a_ref, 0, 0, 1)
ZEND_ARG_INFO(1, ref)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo____set, 0, 0, 2)
ZEND_ARG_TYPE_INFO(0, q, IS_STRING, 0)
ZEND_ARG_TYPE_INFO(0, w, IS_STRING, 0)
ZEND_END_ARG_INFO()

#endif//TCPSERVER_IO_H