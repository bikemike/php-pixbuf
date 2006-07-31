/* include standard header */
#include <php.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "php-pixbuf.h"

#define PHP_PIXBUF_GDKPIXBUF_RES_NAME "GdkPixbuf"
int le_gdkpixbuf;

ZEND_DECLARE_MODULE_GLOBALS(pixbuf);

//PHP_INI_BEGIN()
//PHP_INI_END()


/* compiled function list so Zend knows what's in this module */
zend_function_entry pixbuf_functions[] =
{
	PHP_FE(pixbuf_new_from_file_at_size, NULL)
	PHP_FE(pixbuf_rotate_simple, NULL)
	PHP_FE(pixbuf_dump, NULL)
	//PHP_FE(pixbuf_save_to_buffer, NULL)
	{NULL, NULL, NULL}
};

/* compiled module information */
zend_module_entry pixbuf_module_entry =
{
	STANDARD_MODULE_HEADER,
	PHP_PIXBUF_EXTNAME,
	pixbuf_functions,
	PHP_MINIT(pixbuf),
	PHP_MSHUTDOWN(pixbuf),
	PHP_RINIT(pixbuf),
	//PHP_RSHUTDOWN(pixbuf),
	NULL,
	PHP_MINFO(pixbuf),
	//NO_VERSION_YET,
	PHP_PIXBUF_VERSION,
	STANDARD_MODULE_PROPERTIES
};

/* implement standard "stub" routine to introduce ourselves to Zend */
#if COMPILE_DL_PIXBUF
ZEND_GET_MODULE(pixbuf)
#endif



static void php_pixbuf_init_globals(zend_pixbuf_globals *pixbuf_globals)
{
}


static void _g_object_unref(GdkPixbuf * pixbuf)
{
	g_object_unref(pixbuf);
}

PHP_MINIT_FUNCTION(pixbuf)
{
	ZEND_INIT_MODULE_GLOBALS(pixbuf, php_pixbuf_init_globals, NULL);

	//REGISTER_INI_ENTRIES();
	
	g_type_init();

	le_gdkpixbuf = register_list_destructors(_g_object_unref,NULL);

	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(pixbuf)
{
	return SUCCESS;
}

PHP_RINIT_FUNCTION(pixbuf)
{
	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(imlib)
{
	return SUCCESS;
}

PHP_MINFO_FUNCTION(pixbuf)
{
}

/* implement function that is meant to be made available to PHP */
PHP_FUNCTION(pixbuf_new_from_file_at_size)
{
	char *filename;
	int filename_length;
	long width,height;

	GError *error;
	error = NULL;
	GdkPixbuf * pixbuf;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sll", &filename, &filename_length, &width, &height) == FAILURE) 
	{
		RETURN_NULL();
	}

	pixbuf = gdk_pixbuf_new_from_file_at_size(filename,width,height,&error);

	if (NULL == pixbuf)
	{
		RETURN_FALSE;
	}
	else
	{
		ZEND_REGISTER_RESOURCE(return_value,pixbuf,le_gdkpixbuf);
	}
}

PHP_FUNCTION(pixbuf_rotate_simple)
{
	zval *zpixbuf;
	GdkPixbuf *pixbuf, *pixbuf_rotated;
	long rotation;
	GdkPixbufRotation r = GDK_PIXBUF_ROTATE_NONE;

	GError *error;
	error = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &zpixbuf, &rotation) == FAILURE) 
	{
		RETURN_NULL();
	}

	ZEND_FETCH_RESOURCE(pixbuf, GdkPixbuf*, &zpixbuf, -1, PHP_PIXBUF_GDKPIXBUF_RES_NAME, le_gdkpixbuf);

	switch (rotation)
	{
		case 90:
			r = GDK_PIXBUF_ROTATE_CLOCKWISE;
			break;
		case 180:
			r = GDK_PIXBUF_ROTATE_UPSIDEDOWN;
			break;
		case 270:
			r = GDK_PIXBUF_ROTATE_COUNTERCLOCKWISE;
			break;
		default:
			RETURN_FALSE;
	}

	pixbuf_rotated = gdk_pixbuf_rotate_simple(pixbuf,r);
	
	ZEND_REGISTER_RESOURCE(return_value,pixbuf_rotated,le_gdkpixbuf);

}

PHP_FUNCTION(pixbuf_dump)
{
	zval *zpixbuf;
	GdkPixbuf *pixbuf = NULL;
	long rotation;
	GdkPixbufRotation r = GDK_PIXBUF_ROTATE_NONE;
	char *type;
	int type_size = 0;
	gchar *buffer = NULL;
	gsize buffer_size;
	GError *error;
	error = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r|s", &zpixbuf, &type, &type_size) == FAILURE) 
	{
		RETURN_NULL();
	}

  ZEND_FETCH_RESOURCE(pixbuf, GdkPixbuf*, &zpixbuf, -1, PHP_PIXBUF_GDKPIXBUF_RES_NAME, le_gdkpixbuf);

	if (type_size == 0)
	{
		type = "jpeg";
	}
	int w = gdk_pixbuf_get_width(pixbuf);
	if ( gdk_pixbuf_save_to_buffer(pixbuf,&buffer, &buffer_size,type, &error,NULL) )
	{
		PHPWRITE(buffer,buffer_size);
		g_free(buffer);
		RETURN_TRUE;
	}
	RETURN_FALSE;
}
