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

PHP_FUNCTION(pixbuf_new_from_file_at_size);
PHP_FUNCTION(pixbuf_rotate_simple);
PHP_FUNCTION(pixbuf_dump);

extern zend_module_entry pixbuf_module_entry;
#define phpext_pixbuf_ptr &pixbuf_module_entry

#endif
