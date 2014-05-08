/* Copyright (c) 2014 Michael Morrison */
/* See LICENSE.txt for details.        */

#ifndef PHP_PIXBUF_H
#define PHP_PIXBUF_H

#define PHP_PIXBUF_VERSION "0.1"
#define PHP_PIXBUF_EXTNAME "php-pixbuf"

ZEND_BEGIN_MODULE_GLOBALS(pixbuf)
ZEND_END_MODULE_GLOBALS(pixbuf)


PHP_MINIT_FUNCTION(pixbuf);
PHP_MSHUTDOWN_FUNCTION(pixbuf);
PHP_RINIT_FUNCTION(pixbuf);
PHP_RSHUTDOWN_FUNCTION(pixbuf);
PHP_MINFO_FUNCTION(pixbuf);

PHP_FUNCTION(pixbuf_new_from_file);
PHP_FUNCTION(pixbuf_new_from_data);
PHP_FUNCTION(pixbuf_new_from_file_at_size);
PHP_FUNCTION(pixbuf_rotate_simple);
PHP_FUNCTION(pixbuf_scale_simple);

PHP_FUNCTION(pixbuf_composite);

PHP_FUNCTION(pixbuf_get_width);
PHP_FUNCTION(pixbuf_get_height);
PHP_FUNCTION(pixbuf_save);
PHP_FUNCTION(pixbuf_dump_and_save);
PHP_FUNCTION(pixbuf_dump);

extern zend_module_entry pixbuf_module_entry;
#define phpext_pixbuf_ptr &pixbuf_module_entry

#endif
