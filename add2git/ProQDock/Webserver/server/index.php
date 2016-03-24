<?php
/* testing    */

require("class_template.php");
$host = GetHostByName($_SERVER['REMOTE_ADDR']); 

$tplObj = new tplSys("./");
$tplObj->getFile( array('pcons' => '/pcons.html' ));

$date=date("Y-m");
$TMP="/local/www/services/ProQ2/server/output/$date";
#$TMP="/Library/WebServer/ProQM/scratch/";
$citation="";
$pdb="";
$seqres="";
$target_name="";


$submission_form="<tr><td class=\"contentheading\" width=\"100%\">Model Quality Assessment of Proteins Submission Page</td>
                      </tr>
                      <tr> </tr>
                     </table>

                    <table class=\"contentpaneopen\">
                      <tr>
                        <td valign=\"top\" colspan=\"2\">
                           Paste in your PDB file 
                          <form method=\"post\" enctype=\"multipart/form-data\">
                            <textarea name=\"pdb\" rows=\"10\" cols=\"50\">$pdb</textarea>
                           
                           <p>Or upload a pdb: <input type=\"file\" name=\"pdbfile\" id=\"pdbfile\" /> </p>
                           Paste in the SEQRES of your PDB (optional, recommended if many models will be submitted for the same sequence)
                           <textarea name=\"seqres\" rows=\"3\" cols=\"50\">$seqres</textarea>
                           <p><input type=\"checkbox\" name=\"bfactorPDB\" value=\"Yes\" checked>Return model with quality in B-factor column</p> 
                           <p>Target name (max 20 chars) <input name=\"name\" value=\"$target_name\" type=\"text\" /> (optional)</p>
                            <p>E-mail <input name=\"email\" type=\"text\" /> </p>
                            <p><input type=\"submit\" name=do value=\"Submit\" /> <INPUT type=\"reset\" value=\"Clear form\" /></p>
                           
                          </form>                       
                        </td>
                      </tr></table>
		    $citation";

#$citation=cite($ref[pcons]);

#       <table class=\"contentpaneopen\"><tr><td class=\"contentheading\" width=\"100%\" valign=\"top\">Selected References</td></tr></table>
#            <table class=\"contentpaneopen\">
#                      <tr><td valign=\"top\" colspan=\"2\">$ref[pcons]</td></tr></table>";

if(isset($_POST{'do'}) && $_POST{'do'}=="Submit")
{

    
    
  
    
#Checking input for errors....
  $pdb_errors=0;
  $email_errors=0;
  $pdbfile_error=0;
  $fasta_info="NONAME";
  

  
  $pdb_data="";
  if ($_FILES["pdbfile"]["error"] > 0)
  {
      #echo "Error: " . $_FILES["pdbfile"]["error"] . "<br />";
      #echo $_FILES;
  }
#  if($_FILES["pdbfile"]["type"] == "chemical/x-pdb") {
  if($_FILES["pdbfile"]["size"] / 1024 / 1024 <  5) {
      #echo "Upload: " . $_FILES["pdbfile"]["name"] . "<br />";
      #echo "Type: " . $_FILES["pdbfile"]["type"] . "<br />";
      #echo "Size: " . ($_FILES["pdbfile"]["size"] ) . " bytes<br />";
      #echo "Stored in: " . $_FILES["pdbfile"]["tmp_name"];
      $tmpfile=$_FILES["pdbfile"]["tmp_name"];
      $pdb_data=shell_exec("cat $tmpfile");
#	  print $pdb_data;
  } else {
      echo "Error: Maximum file size 5MB <br />";
      echo "Size: " . ($_FILES["pdbfile"]["size"] ) . " bytes<br />";
      
      
  }

  #}


  
  if(isset($_POST{'pdb'}) || strlen($pdb_data)>0)
    {
	
	if(strlen($pdb_data)>0) {
	    $in_pdb=$pdb_data;
	} else {
	    $in_pdb=htmlspecialchars($_POST{'pdb'});
	}
#	print "in here\n";
#	print $in_pdb;
#	exit;
#	$fh=fopen("$TMP/test.pdb","w");
#	fwrite($fh,$in_pdb);
#	exit;
#	$in_pdb=strtoupper(htmlspecialchars($_POST{'pdb'}));
#	$in_pdb=strtoupper(($_POST{'pdb'}));
      
	$lines=preg_split('/\n/',$in_pdb); 
	$pdb="";
	$rescount=0;
	$all_count=0;
      foreach ($lines as $line)
	{
	#  print $line."[new] <br>\n";
	  if(preg_match('/^ATOM/',$line)) #,$matches))
	    {
	      #print "FASTA INFO $fasta_info <br>\n";
#print_r($matches);
		$all_count=$all_count+1;
		$pdb.="$line\n";
		if(preg_match('/CA/',$line)) #,$matches))
    		{
		    $rescount=$rescount+1;
	        }		
		#      $Fasta_info=$matches[1];
	     # print "FASTA INFO $fasta_info <br>\n";
	    }
	}



#      print "<hr>";
#      print $rescount;
#      print "<hr>";
  #    exit;
#print $pdb."[new]";
   #   $pdb=strtoupper($pdb);
   #   $pdb=preg_replace('/\s+/','',$pdb);
   #   $check_pdb=preg_replace('/[ACDEFGHIKLMNPQRSTVWY]/','',$pdb);
      
      
      if($rescount < 10)
	  $pdb_errors="The pdb contains to few $rescount < 10 residues";
      if($rescount==$all_count) 
	  $pdb_errors="Need all atoms not only CA\'s";
      if(strlen($pdb)==0)
	$pdb_errors="Missing pdb";
	

      $email="N/A";
      if(isset($_POST{'email'}))
	{
	    if(strlen($_POST{'email'})>0)
	    {
	      $email=htmlspecialchars($_POST{'email'});
	      if(!(preg_match("/proteinmodelportal.org/",$email)))
	      {
#		  print "<hr>";   
#		  print $email;
		  $email_errors=check_email_address($email);
	      } 
	      #else {
#		  print "PORTAL!!!!";
#	      }
#	      $email_errors="Yes";
	      

#$email_check2=CheckEmailExist($email);
#
	      #print "<hr>";
	      #print "$email_errors<br>";
	      #print "$email<br>";
	      #print "$email_check2<br>";
	      #print "<hr>";
	    }
	    else
	    {
		$email_errors="Need email";
	    }
	  
	}
	else
	{

	    $email_errors="Need email";
	}
	$seqres="";
	if(isset($_POST{'seqres'}))
	{
	    if(strlen($_POST{'seqres'})>0)
	    {
		$seqres=htmlspecialchars($_POST{'seqres'});
		$seqres_check=preg_replace('/\n/','',$seqres);
		$seqres_check=preg_replace('/[ACDEFGHIKLMNPQRSTVWY]/','',$seqres_check);
	    }
	}
    }

#  $name=$fasta_info;
#  if(isset($_POST{'name'}))
#    {
#      if(strlen($_POST{'name'})>0)
#	{
#	  
#	  $name=preg_replace('/\n/','',$name);
#	  $name=preg_replace('/\//',' ',$name);
#	  $name=preg_replace('/\s+/','%20',$name);
#          $name=htmlspecialchars($_POST{'name'});
#	}
#    }
 # $name=preg_replace('/\n/','',$name);
 # $name=preg_replace('/\//',' ',$name);
 # $name=preg_replace('/\s+/','%20',$name);
  
  
# $pdb_errors=1;
  if($pdb_errors=="0" && $email_errors=="0")
    {


	exec("mkdir $TMP");
	$folder=shell_exec("mktemp -d $TMP/job.XXXXXXXX");
#	print $TMP."\n";
#	print "folder: $folder";
	$folder=rtrim($folder);
	$temp=preg_split('/\//',$model);
	$folder_base=end($temp);
#	print $folder;
#	print $folder_base;
#	print "<hr>";
	exec("date > $folder/date");
	$fh=fopen("$folder/input.pdb","w");
	fwrite($fh,$pdb);
	fclose($fh);
	$fh=fopen("$folder/email","w");
	fwrite($fh,"$email\n");
	fclose($fh);
	$fh=fopen("$folder/seqres","w");
	fwrite($fh,">seqres\n$seqres\n");
	fclose($fh);
	$target_name="";
	if(isset($_POST{'name'})) {
	    $target_name=htmlspecialchars($_POST{'name'});
	}
	$target_name=preg_replace('/\s+/','',$target_name);
	$fh=fopen("$folder/name","w");
	fwrite($fh,"$target_name\n");
	fclose($fh);
	
#      $str=urlencode("seq=$pdb&email=$email&name=$name&host=$host");
      
#$str2="http://sbcweb.pdc.kth.se/cgi-bin/bjornw/pconsmeta.cgi?seq=$pdb&email=$email&name=$name&host=$host";
      #echo $str2;
       #$str=http_get("http://sbcweb.pdc.kth.se/cgi-bin/bjornw/pcons/pconsmeta.cgi?seq=$pdb&email=$email&name=$name&host=$host");

      #$str=preg_replace('/\s+/','',$str);
      #preg_match('/\-\d+$/',$str,$matches);
    #  print_r($matches);
      #if(isset($matches[0]))
#	{
#	  $str=$matches[0];
#	}
	#print $email;
	#print $pdb;
	$bfactorPDB="";
	if(isset($_POST{'bfactorPDB'})) {
	    $bfactorPDB="-bfactorPDB";
	    #print "b: <br>";
	    #print "/local/www/services/ProQ2/bin/ProQ2_queue.pl $folder/input.pdb $folder/email $folder/seqres $bfactorPDB<br>";
	}
	execBG("/local/www/services/ProQ2/bin/ProQ2_queue.pl $folder/input.pdb $folder/email $folder/seqres $bfactorPDB");
#	execBG("sbatch /local/www/services/ProQ2/apps/ProQ2/bin/ProQ2.pl $folder/input.pdb $folder/email $folder/seqres $bfactorPDB");
	
#	$data=shell_exec("/Library/WebServer/ProQM/bin/ProQM_queue.pl $folder/input.pdb $folder/email");
#	$user_heading="Results";
	$user_heading="Submission successful!";
 
	
#	$user_text="<img src=$folder_base/pred.png>"
#	$pred=shell_exec("cat $folder/pred.all|awk '{print $1}'");q
#	$pred=preg_replace('/\n/','<BR>',$pred);
#	$user_text.=$pred;
	
	$user_text="An email with will be sent to $email when the job is done.\n";
#	$user_text.="/local/www/services/ProQ2/bin/ProQ2_queue.pl $folder/input.pdb $folder/email $folder/seqres $bfactorPDB\n";
    }
  else
    {
      $user_heading="Error";
      $user_text="";
      if($pdb_errors!="0")
	{
	  $user_text="$user_text"."Pdb Error: # $pdb_errors<br />";
	}
      if($email_errors!="0")
	{
	  $user_text="$user_text"."Email Error: # $email_errors<br />";
	}
      
      $user_text="$user_text"."Please go <a href=javascript:history.go(-1)>back</a> and try again.";
    }
  
  
  
  

  $text="<tr><td class=\"contentheading\" width=\"100%\">$user_heading</td>
             </tr><tr> </tr></table><table class=\"contentpaneopen\"><tr><td valign=\"top\" colspan=\"2\">
                               $user_text                   
                        </td></tr></table><span class=\"article_seperator\">&nbsp;</span>";
  
  
}
elseif(isset($_GET{'about'}))
{
  $about=$_GET{'about'};
  if($about=='proqm')
    {
      $heading="Protein Quality Predictors";
      $bulk_text="<div style=width:500>";
    
      $bulk_text.="<P> Computational modeling of three-dimensional structure of protein molecules is
an important research area that has the potential to dramatically accelerate the
determination of protein structures, both in terms of purely predicted structures
and as part of an otherwise experimental pipeline. An increase in the number of
proteins for which high quality structural data is available would greatly enhance
our ability to understand biological function, redesign proteins of interest and
develop new drugs, which might even be peptides themselves 
(<i>ProQ2</i>) . The score presented in the result list is the <i>ProQ2</i> score. 
</P>
<P>
ProQ2 predicts the so called S-score for each individual residue (You will get a plot over this score when you click the link on each score.) 
The S-score is a transformation of the normal RMSD for each residue 
using the following formula: S_i=(1/sqrt(1+RMSD_i^2/9)), where RMSD_i 
is the local RMSD deviation for residue <i>i</i> based on a global superposition 
trying to maximize essentially the sum of S-score over the whole model (in reality the superposition is a trade off between getting a high sum of S-score and the length of the structural alignment).
</p>";
        $bulk_text.="</DIV>";
    }
  
 

  if(!isset($heading))
    {
      $heading=$about;
      $bulk_text="BULK TEXT";
    }
  $text="<tr>
                       <td class=\"contentheading\" width=\"100%\">$heading</td>
                      </tr>
                      <tr> </tr>
                     </table>

                    <table class=\"contentpaneopen\">
                      <tr>
                        <td valign=\"top\" colspan=\"2\">
                           $bulk_text                      
                        </td>
                      </tr></table>
		    <span class=\"article_seperator\">&nbsp;</span>";






}
else
{
  $text=$submission_form;
}

#print $text;
date_default_timezone_set('Europe/Berlin'); # for 'CET/1.0/no DST');
$tplObj->varRef( "pcons", array(
				"DATE"    =>    date("l, F j Y"),
				"TEXT" => "$text"                   
				));
$tplObj->parseDynamic("pcons");





#Produce a 50x50 png of the pdb file
function raytrace($pdb,$outfile)
{
  $molauto="/afs/pdc.kth.se/home/b/bjornw/modules/ia64/molscript-2.1.2/molauto";
  $molscript="/afs/pdc.kth.se/home/b/bjornw/modules/ia64/molscript-2.1.2/molscript";
  $raster3d="/afs/pdc.kth.se/home/b/bjornw/modules/ia64/raster3d/render";
  #print "$molauto -nice $pdb|$molscript -r | $raster3d -aa -size 50x50 -out /scratch/temp.$$.png;mv /scratch/temp.$$.png $pdb.png <br>";
  exec("$molauto -nice $pdb|$molscript -r | $raster3d -aa -size 50x50 -out /scratch/temp.$$.png;mv /scratch/temp.$$.png $outfile");

}


function format_sequence($sequence)
{
  $aa_vec=preg_split('//',$sequence);
  $sequence_nice="";
  $count=0;
  foreach ($aa_vec as $aa)
    {
     # print "$count $aa<br />";
      if(strlen($aa)==1)
	{
	  if($count % 10 == 0 && $count>0)
	    {
	      $sequence_nice="$sequence_nice ";
	    }
	  if($count % 60 == 0 && $count > 0)
	    {
	      $sequence_nice="$sequence_nice\n";
	    }
	  
	  $sequence_nice="$sequence_nice$aa";
	  $count++;
	}
    }
  
  return($sequence_nice);

}

function check_email_address($email) {
   // First, we check that there's one @ symbol, and that the lengths are right
  if (!ereg("^[^@]{1,64}@[^@]{1,255}$", $email)) {
    #print "// Email invalid because wrong number of characters in one section, or wrong number of @ symbols.";
    return "Email (\"$email\") invalid because wrong number of characters in one section, or wrong number of @ symbols.";
  }
  // Split it into sections to make life easier
  $email_array = explode("@", $email);
  $local_array = explode(".", $email_array[0]);
  for ($i = 0; $i < sizeof($local_array); $i++) {
     if (!ereg("^(([A-Za-z0-9!#$%&'*+/=?^_`{|}~-][A-Za-z0-9!#$%&'*+/=?^_`{|}~\.-]{0,63})|(\"[^(\\|\")]{0,62}\"))$", $local_array[$i])) {
    #  return false;
      return "Email (\"$email\") invalid";
    }
  }  
  if (!ereg("^\[?[0-9\.]+\]?$", $email_array[1])) { // Check if domain is IP. If not, it should be valid domain name
    $domain_array = explode(".", $email_array[1]);
    if (sizeof($domain_array) < 2) {
      return "Email (\"$email\") invalid beacuse not enought parts to be a domain";
         // Not enough parts to domain
    }
    for ($i = 0; $i < sizeof($domain_array); $i++) {
      if (!ereg("^(([A-Za-z0-9][A-Za-z0-9-]{0,61}[A-Za-z0-9])|([A-Za-z0-9]+))$", $domain_array[$i])) {
	return "Email (\"$email\") invalid";
      }
    }
  }
  return 0;
}

function http_get( $url ) {
  $request = fopen( $url, "rb" );
  $result = "";
  while( !feof( $request ) ) {
    $result .= fread( $request, 8192 );
  }
  
  fclose( $request );
  
  return $result;
}

function cite($ref)
{
    return "";
  # return "<span class=\"article_seperator\">&nbsp;</span><table class=\"contentpaneopen\"><tr><td class=\"contentheading\" width=\"100%\" valign=\"top\">References</td></tr></table><table class=\"contentpaneopen\"><tr><td valign=\"top\" colspan=\"2\">$ref</td></tr></table>";
}

function execBG($cmd) { 
    if (substr(php_uname(), 0, 7) == "Windows"){ 
        pclose(popen("start /B ". $cmd, "r"));  
    } 
    else { 
        exec($cmd . " > /dev/null &");   
    } 
} 

?> 
    
