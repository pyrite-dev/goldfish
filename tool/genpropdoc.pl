#!/usr/bin/env perl
my $FLAG_GUI = 0b1;

my %list = {};
my %doc = {};

$doc{"active"} = "Non-zero if component is active.";
$doc{"cancel-drag"} = "Set non-zero to ignore dragging. (internal property)";
$doc{"clicked-x"} = "X coord of mouse on click, relative to screen.";
$doc{"clicked-y"} = "Y coord of mouse on click, relative to screen.";
$doc{"close-parent"} = "Set non-zero to close parent component.";
$doc{"cursor"} = "Cursor position.";
$doc{"diff-x"} = "X coord of mouse on click, relative to component.";
$doc{"diff-y"} = "Y coord of mouse on click, relative to component.";
$doc{"active"} = "Non-zero if component is focused.";
$doc{"focus"} = "Non-zero if component is focused.";
$doc{"frame"} = "Frame of component.";
$doc{"grab"} = "Set non-zero to force outset border.";
$doc{"hide"} = "Set non-zero to hide component and its child components.";
$doc{"ignore-mouse"} = "Set to ignore mouse on component.";
$doc{"max-value"} = "Maximum value.";
$doc{"min-height"} = "Minimum height of component.";
$doc{"min-value"} = "Minimum value.";
$doc{"min-width"} = "Minimum width of component.";
$doc{"no-sound"} = "Set non-zero to disable sound.";
$doc{"old-height"} = "Old height of component before resize.";
$doc{"old-width"} = "Old width of component before resize.";
$doc{"scrollbar"} = "Scrollbar of component.";
$doc{"selected"} = "Selected component. (internal property)";
$doc{"split"} = "How many boxes should be shown in progress bar.";
$doc{"step"} = "Granularity that value must adhere to.";
$doc{"value"} = "Value.";
$doc{"x-base"} = "Set \`1\` to make X coord base right of screen.\n\nDefault is \`-1\`, which makes X coord base left of screen.";
$doc{"y-base"} = "Set \`1\` to make Y coord base bottom of screen.\n\nDefault is \`-1\`, which makes Y coord base top of screen.";

open(my $fh, '-|', 'grep -r prop src');

while(my $l = <$fh>){
	my $name = "";
	my $type = "";
	my $flag = 0;
	my $filename = $l;
	$filename =~ s/^([^:]+):[\S\s]+$/\1/;
	if($l =~ /(?:gf_prop_set|gf_gui_set_prop)_(integer|floating|id)\(([^"]+)"([^"]+)"/){
		my $arg = $2;
		$name = $3;
		$type = $1;

		if(($arg =~ /gf_gui_get_prop/) or ($filename =~ /gui/)){
			$flag |= $FLAG_GUI;
		}
	}else{
		next;
	}
	if(exists($list{$name})){
		next;
	}

	$list{$name} = {
		"type" => $type,
		"flag" => $flag,
		"filename" => $filename
	};
}

close($fh);

print("# Property cheatsheet\n\n");
foreach my $key (sort keys(%list)){
	if("HASH" eq ref $list{$key}){
		my $type = $list{$key}{"type"};
		my $flag = $list{$key}{"flag"};
		my $filename = $list{$key}{"filename"};

		print("## $key\n");
		print("Type: \`$type\` \n\n");

		if($flag & $FLAG_GUI){
			print("Context: GUI\n\n");
		}
		print("\n");
		print("$doc{$key}\n");
		print("\n");
	}
}
