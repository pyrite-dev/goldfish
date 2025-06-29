#!/usr/bin/env perl

sub line {
	add_buffer(" **" . ("*" x 80) . "\n");
}

my $buffer = "";
sub add_buffer {
	local($line) = @_;
	$buffer = $buffer . $line;
}

my $lines = "";
my $input = "";
my $author = "Nishi";
my $active = 0;
my $found = 0;

my $only_license = 0;

foreach my $arg (@ARGV){
	if($arg eq "--only-license"){
		$only_license = 1;
	}elsif($input eq ""){
		$input = $arg;
	}elsif(!($input eq "")){
		$author = $arg;
	}
}

my $filename = $input;
$filename =~ s/^.+\/([^\/]+)$/\1/;

open(my $f, "<", "$input") or die;
while(my $l = <$f>){
	$lines = $lines . $l;
	$l =~ s/\r?\n$//;
	if($l eq " * \@if SHOW_LICENSE" && !$found && !$active){
		$active = 1;
	}elsif($l eq " * \@endif" && $active){
		$found = 1;
	}
}
close($f);
if($found){
	add_buffer($lines);
}else{
	add_buffer("/**\n");
	add_buffer(" * \@if SHOW_LICENSE\n");

	line();
	add_buffer(" * \n");	
	open(my $f, "<", "LICENSE") or die;
	while(my $l = <$f>){
		add_buffer(" * $l");
	}
	close($f);
	add_buffer(" * \n");
	if(!$only_license){
		line();
		add_buffer(" * File Name:    $filename\n");
		add_buffer(" * Author:       $author\n");
		add_buffer(" * \n");
		add_buffer(" * Functionality:\n");
		add_buffer(" * TODO: Write here\n");
		add_buffer(" * \n");
	}
	line();

	add_buffer(" * \@endif\n");
	add_buffer(" */\n");
	add_buffer("\n" . $lines);
}

open(my $f, ">", "$input") or die;
print $f ($buffer);
close($f);
