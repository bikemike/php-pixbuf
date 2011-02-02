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
	PHP_FE(pixbuf_new_from_data, NULL)
	PHP_FE(pixbuf_new_from_file, NULL)
	PHP_FE(pixbuf_new_from_file_at_size, NULL)
	PHP_FE(pixbuf_rotate_simple, NULL)
	PHP_FE(pixbuf_scale_simple, NULL)
	PHP_FE(pixbuf_composite, NULL)
	PHP_FE(pixbuf_get_width, NULL)
	PHP_FE(pixbuf_get_height, NULL)
	PHP_FE(pixbuf_save, NULL)
	PHP_FE(pixbuf_dump_and_save, NULL)
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


/*
static void _g_object_unref(GdkPixbuf * pixbuf)
{
	g_object_unref(pixbuf);
}
*/

PHP_MINIT_FUNCTION(pixbuf)
{
	ZEND_INIT_MODULE_GLOBALS(pixbuf, php_pixbuf_init_globals, NULL);

	//REGISTER_INI_ENTRIES();
	
	g_type_init();

	le_gdkpixbuf = register_list_destructors(g_object_unref,NULL);

    REGISTER_LONG_CONSTANT("GDK_INTERP_NEAREST", GDK_INTERP_NEAREST, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("GDK_INTERP_TILES", GDK_INTERP_TILES, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("GDK_INTERP_BILINEAR", GDK_INTERP_BILINEAR, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("GDK_INTERP_HYPER", GDK_INTERP_HYPER, CONST_CS | CONST_PERSISTENT);

    REGISTER_LONG_CONSTANT("GDK_PIXBUF_ROTATE_NONE", GDK_PIXBUF_ROTATE_NONE, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("GDK_PIXBUF_ROTATE_COUNTERCLOCKWISE", GDK_PIXBUF_ROTATE_COUNTERCLOCKWISE, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("GDK_PIXBUF_ROTATE_UPSIDEDOWN", GDK_PIXBUF_ROTATE_UPSIDEDOWN, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("GDK_PIXBUF_ROTATE_CLOCKWISE", GDK_PIXBUF_ROTATE_CLOCKWISE, CONST_CS | CONST_PERSISTENT);

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
PHP_FUNCTION(pixbuf_new_from_data)
{
	char *filedata;
	int filedata_length;

	GError *error;
	error = NULL;
	GdkPixbufLoader *loader;
	GdkPixbuf *pixbuf;


	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &filedata, &filedata_length) == FAILURE) 
	{
		RETURN_NULL();
	}
	loader = gdk_pixbuf_loader_new();
	gdk_pixbuf_loader_write(loader,(guchar*)filedata,filedata_length,&error);
	gdk_pixbuf_loader_close(loader,&error);

	pixbuf = gdk_pixbuf_loader_get_pixbuf(loader);
	g_object_ref(pixbuf);
	g_object_unref(loader);

	if (NULL == pixbuf)
	{
		RETURN_FALSE;
	}
	else
	{
		ZEND_REGISTER_RESOURCE(return_value,pixbuf,le_gdkpixbuf);
	}
}

PHP_FUNCTION(pixbuf_new_from_file)
{
	char *filename;
	int filename_length;

	GError *error;
	error = NULL;
	GdkPixbuf * pixbuf;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &filename, &filename_length) == FAILURE) 
	{
		RETURN_NULL();
	}

	pixbuf = gdk_pixbuf_new_from_file(filename,&error);

	if (NULL == pixbuf)
	{
		RETURN_FALSE;
	}
	else
	{
		ZEND_REGISTER_RESOURCE(return_value,pixbuf,le_gdkpixbuf);
	}
}

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
		case GDK_PIXBUF_ROTATE_CLOCKWISE:
		case GDK_PIXBUF_ROTATE_UPSIDEDOWN:
		case GDK_PIXBUF_ROTATE_COUNTERCLOCKWISE:
			r = rotation;
			break;
		default:
			r = GDK_PIXBUF_ROTATE_NONE;
	}

	pixbuf_rotated = gdk_pixbuf_rotate_simple(pixbuf,r);
	
	ZEND_REGISTER_RESOURCE(return_value,pixbuf_rotated,le_gdkpixbuf);

}
PHP_FUNCTION(pixbuf_scale_simple)
{
	zval *zpixbuf;
	GdkPixbuf *pixbuf, *pixbuf_scaled;
	long w, h, interp_type;
	GdkInterpType i = GDK_INTERP_BILINEAR;

	GError *error;
	error = NULL;

	interp_type = i;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rll|l", &zpixbuf, &w, &h, &interp_type) == FAILURE) 
	{
		RETURN_NULL();
	}

	ZEND_FETCH_RESOURCE(pixbuf, GdkPixbuf*, &zpixbuf, -1, PHP_PIXBUF_GDKPIXBUF_RES_NAME, le_gdkpixbuf);

	switch (interp_type)
	{
		case GDK_INTERP_NEAREST:
		case GDK_INTERP_TILES:
		case GDK_INTERP_BILINEAR:
		case GDK_INTERP_HYPER:
			i = interp_type;
			break;
		default:
			i = GDK_INTERP_BILINEAR;
	}

	pixbuf_scaled = gdk_pixbuf_scale_simple(pixbuf, w, h, i);
	
	ZEND_REGISTER_RESOURCE(return_value,pixbuf_scaled,le_gdkpixbuf);

}

PHP_FUNCTION(pixbuf_composite)
{
	zval *zpixbuf_src;
	zval *zpixbuf_dst;
	GdkPixbuf *pixbuf_src, *pixbuf_dst;
	long dest_x = 0, dest_y = 0, dest_width = 0, dest_height =0, overall_alpha = 0, interp_type = GDK_INTERP_BILINEAR;
	double offset_x = 0., offset_y = 0., scale_x = 1., scale_y = 1.;

	GError *error;
	error = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rrllllddddll", 
		&zpixbuf_src, &zpixbuf_dst, 
		&dest_x, &dest_y, &dest_width, &dest_height,
		&offset_x, &offset_y, &scale_x, &scale_y,
		&interp_type, &overall_alpha) == FAILURE) 
	{
		RETURN_NULL();
	}

	ZEND_FETCH_RESOURCE(pixbuf_src, GdkPixbuf*, &zpixbuf_src, -1, PHP_PIXBUF_GDKPIXBUF_RES_NAME, le_gdkpixbuf);
	ZEND_FETCH_RESOURCE(pixbuf_dst, GdkPixbuf*, &zpixbuf_dst, -1, PHP_PIXBUF_GDKPIXBUF_RES_NAME, le_gdkpixbuf);

	switch (interp_type)
	{
		case GDK_INTERP_NEAREST:
		case GDK_INTERP_TILES:
		case GDK_INTERP_BILINEAR:
		case GDK_INTERP_HYPER:
			break;
		default:
			interp_type = GDK_INTERP_BILINEAR;
	}

	gdk_pixbuf_composite (pixbuf_src,
		pixbuf_dst,
		dest_x,
		dest_y,
		dest_width,
		dest_height,
		offset_x,
		offset_y,
		scale_x,
		scale_y,
		interp_type,
		overall_alpha);

	RETURN_TRUE;
}

PHP_FUNCTION(pixbuf_get_width)
{
	zval *zpixbuf;
	GdkPixbuf *pixbuf;

	GError *error;
	error = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zpixbuf ) == FAILURE) 
	{
		RETURN_NULL();
	}

	ZEND_FETCH_RESOURCE(pixbuf, GdkPixbuf*, &zpixbuf, -1, PHP_PIXBUF_GDKPIXBUF_RES_NAME, le_gdkpixbuf);
	RETURN_LONG(gdk_pixbuf_get_width(pixbuf));
}

PHP_FUNCTION(pixbuf_get_height)
{
	zval *zpixbuf;
	GdkPixbuf *pixbuf;

	GError *error;
	error = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zpixbuf ) == FAILURE) 
	{
		RETURN_NULL();
	}

	ZEND_FETCH_RESOURCE(pixbuf, GdkPixbuf*, &zpixbuf, -1, PHP_PIXBUF_GDKPIXBUF_RES_NAME, le_gdkpixbuf);
	RETURN_LONG(gdk_pixbuf_get_height(pixbuf));
}


PHP_FUNCTION(pixbuf_save)
{
	zval *zpixbuf;
	GdkPixbuf *pixbuf = NULL;
	char *fname,*type;
	int fname_size=0,type_size = 0;
	GError *error;
	error = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs|s", &zpixbuf, &fname, &fname_size, &type, &type_size) == FAILURE) 
	{
		RETURN_NULL();
	}

	ZEND_FETCH_RESOURCE(pixbuf, GdkPixbuf*, &zpixbuf, -1, PHP_PIXBUF_GDKPIXBUF_RES_NAME, le_gdkpixbuf);

	if (type_size == 0)
	{
		type = "jpeg";
	}
	if ( gdk_pixbuf_save(pixbuf, fname, type, &error,NULL) )
	{
		RETURN_TRUE;
	}
	RETURN_FALSE;
}

PHP_FUNCTION(pixbuf_dump_and_save)
{
	zval *zpixbuf;
	GdkPixbuf *pixbuf = NULL;
	char *fname,*type;
	int fname_size=0,type_size = 0;
	gchar *buffer = NULL;
	gsize buffer_size,written_size;
	GError *error;
	error = NULL;
	gboolean rval = FALSE;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs|s", &zpixbuf, &fname, &fname_size, &type, &type_size) == FAILURE) 
	{
		RETURN_NULL();
	}

	ZEND_FETCH_RESOURCE(pixbuf, GdkPixbuf*, &zpixbuf, -1, PHP_PIXBUF_GDKPIXBUF_RES_NAME, le_gdkpixbuf);

	if (type_size == 0)
	{
		type = "jpeg";
	}

	if ( gdk_pixbuf_save_to_buffer(pixbuf,&buffer, &buffer_size,type, &error,NULL) )
	{
		PHPWRITE(buffer,buffer_size);
		
		FILE *output = fopen(fname,"w");
		if (NULL != output)
		{
			written_size = fwrite(buffer, sizeof(gchar), buffer_size, output);
			if (written_size == buffer_size)
			{
				rval = TRUE;
			}
			fclose(output);
		}
		
		g_free(buffer);
		
		if (rval)
		{
			RETURN_TRUE;
		}
		else
		{
			RETURN_FALSE;
		}
	}
	RETURN_FALSE;
}
PHP_FUNCTION(pixbuf_dump)
{
	zval *zpixbuf;
	GdkPixbuf *pixbuf = NULL;
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
	if ( gdk_pixbuf_save_to_buffer(pixbuf,&buffer, &buffer_size,type, &error,NULL) )
	{
		PHPWRITE(buffer,buffer_size);
		g_free(buffer);
		RETURN_TRUE;
	}
	RETURN_FALSE;
}
