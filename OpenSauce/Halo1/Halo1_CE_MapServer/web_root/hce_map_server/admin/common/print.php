<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
function print_line_inset($string, $tab_count)
{
	$output_string = "";
	for($i = 0; $i < $tab_count; $i++)
		$output_string .= "\t";
	$output_string .= $string;
	$output_string .= "\r\n";
	print($output_string);
}
?>
