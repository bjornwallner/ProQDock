<?php
/*    Begin the class    */
class tplSys
{
    var $loci = array();
    /*    Holds the html template    */
    var $tpl_htmlcode = array();
    /*    Holds variable references    */
    var $tpl_dirtycode = array();
    /*    Holds compiled and clean code    */
    var $tpl_cleancode = array();

    function tplSys($root)
    {
        /*    Constructor, sets tpl and cache directories    */
        if( empty( $root ) )
        {
            die("tplSys(): No root directory set.");
        }
        if( !is_dir( $root ) )
        {
            die("tplSys(): $root Directory doesn't exist.");
        }
        $this->loci['root'] = $root;
        $this->loci['cache'] = $root . 'cache/';
    
    }

    /*    Grabs file from directory and loads
        all the code into _dirtycode array    */
    function getFile($file_array)
    {
        if( !is_array($file_array) )
        {
            die("getFile(): $file_array not an array.");
        }

        foreach( $file_array as $ref=>$filename )
        {
            $fileref = $this->loci['root'] . $filename;
            $this->tpl_htmlcode[$ref] = implode( "", @file($fileref) );
        }
    }

    /*    Define references inside the HTML file    */
    function varRef( $handle, $refArray )
    {
        if( !is_array($refArray) )
        {
            die("varRef(): $refArray is not an array.");
        }
        if( empty($handle) )
        {
            die("varRef(): $handle was empty.");
        }
        
        if( !empty( $this->tpl_htmlcode[$handle] ) )
        {
            /*    Code exists, create references    */
            foreach( $refArray as $ref=>$replace )
            {
                $this->tpl_dirtycode['{' . $ref . '}'] = $replace;
            }
        }
        else
        {    
            die("varRef(): Template data for $handle didn't exist.");
        }
    }

    /*    Parses dynamic code and prints it to the browser    */
    function parseDynamic( $handle )
    {
        if( empty( $handle ) )
        {
            die("parseDynamic(): $handle was empty.");
        }
        if( !empty( $this->tpl_htmlcode[$handle] ) )
        {
            $this->tpl_cleancode[$handle] = $this->compile( $handle );
            eval( $this->tpl_cleancode[$handle] );
        }
    }

    function compile( $handle )
    {
        if( empty( $this->tpl_htmlcode[$handle] ) )
        {
            die("compile(): No code existed for $handle.");
        }
        if( empty( $handle ) )
        {
            die("compile(): $handle was empty.");
        }
        $code = $this->tpl_htmlcode[$handle];

        /*    Count and clean up the code for parsing    */
        $code = str_replace('\\', '\\\\', $code);
        $code = str_replace('\'', '\\\'', $code);

        $d_code = explode( "\n", $code );
        $line_count = sizeof( $d_code );

        /*    Loop through and add 'echo' and '\\n' to each line    */
        for( $x = 0; $x < $line_count; $x++ )
        {
            foreach( $this->tpl_dirtycode as $varref=>$replace )
            {
                if( preg_match( "/^(.*)" . $varref . "(.*)$/", $d_code[$x] ) )
                {
                    $d_code[$x] = preg_replace( "/^(.*)" . $varref . "(.*)$/", "\\1" . $replace . "\\2", $d_code[$x] );
                }
            }
            /*    This is where the loop data and logic will be taken care of    */
            $d_code[$x] = 'echo \'' . $d_code[$x] . '\' . "\\n";';
        }
        $code = implode("\n", $d_code);

        return $code;
    }
}
/*    EOF    */
?> 