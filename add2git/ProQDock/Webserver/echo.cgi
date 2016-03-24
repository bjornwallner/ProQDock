#!/usr/local/gnu/bin/perl
use CGI qw(:standard);

$title = 'Echoing submitted form data';
$start = 40;

print <<ENDSTART;
Content-Type: text/html; charset=iso-8859-1

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html lang="en-US">
<head>
<title>$title</title>
</head>
<body>
<h1 style="font-size:1.2em">$title</h1>
ENDSTART

    if(param()) {
	@fields = param();
	print "<table border=\"1\" cellspacing=\"0\">\n";
	for $variable(@fields) {
	  @values = param($variable);
          for $value(@values) {
	    print "<tr><th align=\"left\" nowrap><tt>",
	     show($variable),"</tt></th><td nowrap>";
    	    @filehandles = upload($variable);
	    if(@filehandles) {
            for $fh(@filehandles) {
		print "**",$fh,"**";
		$type = uploadInfo($value)->{'Content-Type'};
		$data = substr(<$fh>,0,$start);
		print "file (<tt>$type</tt>, first line or max. $start octets shown): <tt><br>",
 		 show($data),"</tt>";
	    }}
	    else {
                $value =~ s/ / /g;
		if($value eq '') {
		    print '<i>(empty)</i>'; }
		else {
		    print "<tt>",show($value),"</tt>"; } }
	    print "</td></tr>\n"; }}
	print "</table>\n"; }
else {
    print p(em('No data was submitted')); }
($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) =
    gmtime(time);
$now = sprintf "%4d-%02d-%02dT%02d:%02dZ",
    1900+$year,$mon+1,$mday,$hour,$min;
print p('Processed '.$now."\n");
print end_html;

sub show {
    $_[0] = escapeHTML($_[0]);
    $_[0] =~ s/\r\n/<br>/g;
    $_[0] =~ s/\n/<br>/g;
    return $_[0]; }

