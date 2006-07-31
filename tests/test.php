<?
dl_local("../src/php-pixbuf.so");

$file = "stuff.png";
$pixbuf = pixbuf_new_from_file_at_size($file,100,100);
$pixbuf_r = pixbuf_rotate_simple($pixbuf,90);
$pixbuf_s = pixbuf_scale_simple($pixbuf,200,200);

for ( $i = 0 ; $i < 10; $i ++)
{
	echo "scale attempt: $i\n";
	$pixbuf_s = pixbuf_scale_simple($pixbuf,500,500);
}
echo pixbuf_get_width($pixbuf) . "\n";
echo pixbuf_get_height($pixbuf) . "\n";
echo pixbuf_get_width($pixbuf_r) . "\n";
echo pixbuf_get_height($pixbuf_r) . "\n";
echo pixbuf_get_width($pixbuf_s) . "\n";
echo pixbuf_get_height($pixbuf_s) . "\n";

pixbuf_save($pixbuf_s,"test_image.png","png");
pixbuf_save($pixbuf_s,"test_image.jpg","jpeg");
pixbuf_save($pixbuf_s,"test_image.bmp","bmp");

Header("Content-type: image/jpeg");

//pixbuf_dump_and_save($pixbuf_s,"test_image_dump.jpg","jpeg");

//pixbuf_dump($pixbuf_r);




function dl_local( $extensionFile ) {
   //make sure that we are ABLE to load libraries
   if( !(bool)ini_get( "enable_dl" ) || (bool)ini_get( "safe_mode" ) ) {
     die( "dh_local(): Loading extensions is not permitted.\n" );
   }

     //check to make sure the file exists
   if( !file_exists( $extensionFile ) ) {
     die( "dl_local(): File '$extensionFile' does not exist.\n" );
   }
  
   //check the file permissions
   if( !is_executable( $extensionFile ) ) {
     die( "dl_local(): File '$extensionFile' is not executable.\n" );
   }

 //we figure out the path
 $currentDir = getcwd() . "/";
 $currentExtPath = ini_get( "extension_dir" );
 $subDirs = preg_match_all( "/\//" , $currentExtPath , $matches );
 unset( $matches );
 
     //lets make sure we extracted a valid extension path
   if( !(bool)$subDirs ) {
     die( "dl_local(): Could not determine a valid extension path [extension_dir].\n" );
   }
 
 $extPathLastChar = strlen( $currentExtPath ) - 1;
 
   if( $extPathLastChar == strrpos( $currentExtPath , "/" ) ) {
     $subDirs--;
   }

 $backDirStr = "";
     for( $i = 1; $i <= $subDirs; $i++ ) {
     $backDirStr .= "..";
       if( $i != $subDirs ) {
         $backDirStr .= "/";
       }
   }

 //construct the final path to load
 $finalExtPath = $backDirStr . $currentDir . $extensionFile;
 
   //now we execute dl() to actually load the module
     if( !dl( $finalExtPath ) ) {
     die();
   }

 //if the module was loaded correctly, we must bow grab the module name
 $loadedExtensions = get_loaded_extensions();
 $thisExtName = $loadedExtensions[ sizeof( $loadedExtensions ) - 1 ];
 
 //lastly, we return the extension name
  return $thisExtName;

}//end dl_local()
?>
