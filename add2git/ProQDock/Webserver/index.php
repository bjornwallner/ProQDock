
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<html>
<head>
    <title>Main page - MolProbity</title>
    <link rel="StyleSheet" href="css/default.css" TYPE="text/css">
    <link rel="shortcut icon" href="favicon.ico">
    <meta name="ROBOTS" content="INDEX, NOFOLLOW">
</head>
<body>
<table border="0" cellpadding="0" cellspacing="0" width="100%">
<tr>
    <td width="150"><img src="img/small-logo5.gif" alt="MolProbity logo"></td>
    <td valign="bottom"><div class="pageheader">
        <h1>Main page</h1>
    </div></td>
</tr>
<tr><td valign="top" width="150">
    <div class="leftnav">
<a href='index.php?MolProbSID=fvd0ice5nemsfhg23k7oipef40&eventID=2'><b>Main page</b></a><br />
<div class='minornav'><a href='index.php?MolProbSID=fvd0ice5nemsfhg23k7oipef40&eventID=3'>About hydrogens</a><br />
</div><div class='minornav'><a href='index.php?MolProbSID=fvd0ice5nemsfhg23k7oipef40&eventID=4'>Evaluate X-ray</a><br />
</div><div class='minornav'><a href='index.php?MolProbSID=fvd0ice5nemsfhg23k7oipef40&eventID=5'>Evaluate NMR</a><br />
</div><div class='minornav'><a href='index.php?MolProbSID=fvd0ice5nemsfhg23k7oipef40&eventID=6'>Fix up structure</a><br />
</div><div class='minornav'><a href='index.php?MolProbSID=fvd0ice5nemsfhg23k7oipef40&eventID=7'>Work with kins</a><br />
</div><br />
<a href='index.php?MolProbSID=fvd0ice5nemsfhg23k7oipef40&eventID=8'>View &amp; download files</a><br />
<a href='index.php?MolProbSID=fvd0ice5nemsfhg23k7oipef40&eventID=9'>Lab notebook</a><br />
<a href='index.php?MolProbSID=fvd0ice5nemsfhg23k7oipef40&eventID=10'>Feedback &amp; bugs</a><br />
<a href='index.php?MolProbSID=fvd0ice5nemsfhg23k7oipef40&eventID=11'>Site map</a><br />
<br />
<a href='index.php?MolProbSID=fvd0ice5nemsfhg23k7oipef40&eventID=12'>Save session</a><br />
<a href='index.php?MolProbSID=fvd0ice5nemsfhg23k7oipef40&eventID=13'>Log out</a><br />
<br />You are using 0% of your 200 Mb of disk space.
    </div>
</td>
<td valign="top">
    <div class="pagecontent">
<div class=feature>Again, a huge thanks to the users who sent emails or letters to support our MolProbity grant renewal resubmission!  With your help, it did very well and has now been funded early, at NIH fiscal-year-end.  So -- MolProbity will still be here for you.</div><form method='post' enctype='multipart/form-data' action='index.php'>
<input type='hidden' name='MolProbSID' value='fvd0ice5nemsfhg23k7oipef40'>
<input type='hidden' name='eventID' value='14'>

<h5 class='welcome'>File Upload/Retrieval (<a href='index.php?MolProbSID=fvd0ice5nemsfhg23k7oipef40&eventID=15' onclick='toggleUploadOptions(); return false' id='upload_options_link'>more options</a>)</h5><script language='JavaScript'>
<!--
function toggleUploadOptions()
{
    var block = document.getElementById('upload_options_block')
    var link = document.getElementById('upload_options_link')
    if(block.style.display == 'none')
    {
        block.style.display = 'block'
        link.innerHTML = 'hide options'
    }
    else
    {
        block.style.display = 'none'
        link.innerHTML = 'more options'
    }
}
// -->
</script>
<div class='indent'><table border='0' width='100%'>
<tr>
    <td>PDB/NDB code: <input type="text" name="pdbCode" size="6" maxlength="10"></td>
    <td>type:
        <select name='fetchType'>
            <option value='pdb'>PDB coords</option>
            <option value='pdb_xray'>PDB coords & xray data</option>
            <option value='biolunit'>Biol. unit (PDB only)</option>
            <option value='biolunit_xray'>Biol. unit & xray data (PDB only)</option>
            <option value='eds_2fofc'>2Fo-Fc map (EDS)</option>
            <option value='eds_fofc'>Fo-Fc map (EDS)</option>
        </select>
    </td>
    <td><input type="submit" name="cmd" value="Fetch &gt;"></td>
</tr><tr><td colspan='3'><hr></td></tr><tr>
    <td><input type="file" name="uploadFile"></td>
    <td>type:
        <select name='uploadType'>
            <option value='pdb'>PDB coords</option>
            <option value='xray'>xray data (mtz format)</option>
            <option value='kin'>kinemage</option>
            <option value='map'>ED map</option>
            <option value='hetdict'>het dict</option>
        </select>
    </td>
    <td><input type="submit" name="cmd" value="Upload &gt;"></td>
</tr>
</table>
    <div class='inline_options' style='display:none' id='upload_options_block'>
        <label><input type="checkbox" name="isCnsFormat" value="1"> File is from CNS refinement</label>
        <br><label><input type="checkbox" name="ignoreSegID" value="1"> Ignore segID field</label>
    </div>
</div></form>
<div class=feature>CCTBX, which powers this version of MolProbity (4.2), requires stricter adherence to PDB format. If you are having trouble with version 4.2 try using MolProbity4 legacy version 4.02 at <a href='http://rutile.biochem.duke.edu/' target='_blank'>http://rutile.biochem.duke.edu</a>.  Note that not all features are supported on legacy 4.02 and we suggest repairing your PDB format instead.</div></div>
<br>
<div class='pagecontent'>
<table border='0' width='100%'><tr valign='top'><td width='45%'>
<h3>Walkthroughs, tutorials, and usage FAQs:</h3>
<p><b><a href='index.php?MolProbSID=fvd0ice5nemsfhg23k7oipef40&eventID=16'>Evaluate X-ray structure</a>:</b>
Typical steps for a published X-ray crystal structure
or one still undergoing refinement.</p>
<p><b><a href='index.php?MolProbSID=fvd0ice5nemsfhg23k7oipef40&eventID=17'>Evaluate NMR structure</a>:</b>
Typical steps for a published NMR ensemble
or one still undergoing refinement.</p>
<p><b><a href='index.php?MolProbSID=fvd0ice5nemsfhg23k7oipef40&eventID=18'>Fix up structure</a>:</b>
Rebuild the model to remove outliers
as part of the refinement cycle.</p>
<p><b><a href='index.php?MolProbSID=fvd0ice5nemsfhg23k7oipef40&eventID=19'>Work with kinemages</a>:</b>
Create and view interactive 3-D graphics
from your web browser.</p>
<p><b><a target="_blank" href="help/validation_options/validation_options.html">Guide to validation options</a>:</b>
Choose validations appropriate to a structure.</p>


<h3>What's new in 4.2:</h3><ul>
      <li>Cis-peptide identification, markup and statistics now available.</li>
      <li>CaBLAM C&alpha;-based validation of protein backbone and secondary structure now available.</li>
      <li>Rotamer validation now uses <a href='http://kinemage.biochem.duke.edu/databases/top8000.php' target='_blank'>Top8000</a> rotamer distributions. Favored vs Allowed distinction introduced for rotamers.</li>
      <li>New coloring scheme in multicriterion chart encodes outlier severity at a glance.</li>
      <li><a target='_blank' href='help/validation_options/validation_options.html'>Online tutorial</a> explaining markup and methods</li>
      <li>We now use a conformation-dependent library (CDL) from Dunbrack and Karplus for geometry analysis if requested.
      </ul>
      <h3>What's new in 4.1:</h3><ul>
      <li>Validation analysis now powered by CCTBX. Also adds geometry regularization for N/Q/H flip corrections. Read more about this change <a href='index.php?MolProbSID=fvd0ice5nemsfhg23k7oipef40&eventID=20'>here</a>.</li>
      <li>Alternate conformations now handled for validation analysis.</li>
      </ul>
      </td><td width='10%'><!-- horizontal spacer --></td><td width=='45%'>
<h3>Citations, science, and technical FAQs:</h3>
<p><b><a href='help/about.html' target='_blank'>Cite MolProbity</a></b>:
    <small>Chen et al. (2010)
    <a href="http://kinemage.biochem.duke.edu/lab/papers.php" target="_blank">MolProbity:
    all-atom structure validation for macromolecular crystallography.</a>
    Acta Crystallographica D66:12-21.
    </p>
    <center>and/or</center>
<p>Davis et al. (2007)
    <a href="http://kinemage.biochem.duke.edu/lab/papers.php" target="_blank">MolProbity:
    all-atom contacts and structure validation for proteins and nucleic acids.</a>
    Nucleic Acids Research 35:W375-W383.
    </small></p>
<p><b><a href='help/about.html' target='_blank'>Cite KiNG</a></b>:
    <small>Chen et al. (2009)
    <a href="http://kinemage.biochem.duke.edu/lab/papers.php" target="_blank">KiNG (Kinemage, Next Generation):
    A versatile interactive molecular and scientific visualization program.</a>
    Protein Science 18:2403-2409.
    </small></p>
<p><b><a href='help/about.html' target='_blank'>Cite CCTBX</a></b>:
    <small>Grosse-Kunstleve et al. (2002)
    <a href="http://scripts.iucr.org/cgi-bin/paper?ks0118" target="_blank">The Computational Crystallography Toolbox:
    crystallographic algorithms in a reusable software framework.</a>
    J. Appl. Cryst. 35:126-136.
    </small></p>
<p><b><a href='index.php?MolProbSID=fvd0ice5nemsfhg23k7oipef40&eventID=21'>About hydrogens</a>:</b>
Why have the hydrogen bondlengths changed?</p>
<p><b><a href='help/java.html' target='_blank'>Installing Java</a></b>: how to make kinemage graphics work in your browser.</p>
<p><b><a href='index.php?MolProbSID=fvd0ice5nemsfhg23k7oipef40&eventID=22'>Download MolProbity</a></b>: how can I run a private MolProbity server, or run from the command line?</p>
<p><small><i>NB: the back button doesn't work inside MolProbity</i></small></p><!-- by request of DCR -->
</td></tr></table>
<script language='JavaScript'>
<!--
    if(!navigator.javaEnabled())
    {
        document.writeln("<br><div class='alert'>");
        document.writeln("It appears that Java is not enabled in your browser, so you may not be able to use KiNG interactive graphics.");
        document.writeln("<br><a href='help/java.html' target='_blank'>Click here for help.</a>");
        document.writeln("</div>");
    }
// -->
</script>

    </div>
</td></tr>
<tr><td colspan="2">
    <div class="pagefooter">
About <a href="help/about.html" target="_blank">MolProbity</a>
| Website for <a href="http://kinemage.biochem.duke.edu" target="_blank">the Richardson Lab</a> | Using ecloud x-H
| Internal reference 4.2
    </div>
</td></tr>
</table>

</body>
</html>

