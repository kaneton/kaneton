#!/usr/bin/perl -w

use Getopt::Long;
use Cwd;

sub mkproto_error ()
{
    print ("no source file to check\n");
    exit (-1);
}

sub usage_error ()
{
    print "Usage: mkp.pl [ -dir ] sourcefile\n";
    exit (-1);
}

sub mkproto_file_to_check ($)
{
    my ($header) = @_;
    my @src;
    my $index = 0;

    open ($handle, $header);

    while ($line = <$handle>)
    {
	last if ($line =~ "---------- prototypes ------------------------------------------------------");
    }

    mkproto_error () if (!defined ($line));
    $line =~ s/---------- prototypes ------------------------------------------------------//g;

    my $end = 0;

    do
    {
	chomp $line;
	if ($line =~ " *[*]{1}\/")
	{
	    $end = 1;
	}
	else
	{
	    if ($line =~ " [*] *\$")
	    {
	    }
	    else
	    {
		if ($line =~ " *[*]{1}[ \t]*([^ ]*)[ \t]*([^ ]*)")
		{
		    $src[$index] = $1;
		    $index++;
		    if ($2 ne "")
		    {
			$src[$index] = $2;
			$index++;
		    }
		}
	    }
	}
    } while (($end == 0) && ($line = <$handle>));

    close ($handle);

    return @src;
}

sub mkproto_parse_src ($*)
{
    my ($filesrc, $tmphandle) = @_;

    print $tmphandle "\n/*\n * $filesrc\n */\n";

    open ($filehandle, $filesrc);
    $do_write = 0;
    while ($line = <$filehandle>)
    {
	chomp $line;
	$do_write = 1 if (!($line =~ "static") && ($line =~ "^[^ \t\n#]+[ \t\n]+[^ ]+[\t\n ]*[(]{1}.*[,)]{1}\$"));
        if (($line =~ "{") && ($do_write))
	{
	    $do_write = 0;
	    print $tmphandle ";\n\n"
	}
	else
	{
	    print $tmphandle "\n$line" if ($do_write);
	}
    }
    close ($filehandle);
}

sub mkproto_create_header ($@)
{
    my ($header, @src) = @_;

    open ($handle, $header);
    open ($tmphandle, ">$header.tmp");

    $proto = 0;
    while ($line = <$handle>)
    {
	print $tmphandle $line;
	$proto = 1 if ($line  =~ "prototypes");
	last if (($line =~ " *[*]{1}\/") && ($proto));
    }

    foreach $filesrc (@src)
    {
	mkproto_parse_src ($filesrc, $tmphandle);
    }

    $do_write = 0;
    while ($line = <$handle>)
    {
	if ($line =~ "eop")
	{
	    print $tmphandle "/*\n";
	    $do_write = 1;
	}
	print $tmphandle $line if ($do_write);
    }

    close ($handle);
    close ($tmphandle);

    rename ("$header.tmp", $header);
}

sub mkproto ()
{
    $curdir = getcwd ();
    foreach $header (@ARGV)
    {
	if ($header =~ "([^ ]*/)([^ ]+)")
	{
	    chdir ($1);
	    $header = $2;
	}

	@src = mkproto_file_to_check ($header);
	mkproto_create_header ($header, @src);
	chdir ($curdir);
    }
}

usage_error () if (@ARGV == 0);
mkproto ();
