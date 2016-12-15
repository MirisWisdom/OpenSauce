<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
	// required to force a refresh of the tables when reloaded
	header("Cache-Control: no-store, no-cache, must-revalidate");	
	
	set_include_path("*admin include path*");
	
	require_once("admin/common/config.php");
	require_once("admin/common/print.php");
	require_once("admin/common/user_account.php");
	require_once("admin/common/sql_database.php");
	require_once("admin/map_database/map_entry_add_func.php");
	
	if(!$can_create_map_entries)
		die("Your user account does not have sufficient privileges to add map entries.");
	
	// increase script timeout value
	ini_set('max_execution_time', 5000);
	
	$add_map_entry_result = ProcessAddMapEntry();
	
	foreach($add_map_entry_result->messages as $value)
		print_line_inset($value."<br/>", 2);
	
	if(!$add_map_entry_result->success)
		print_line_inset($add_map_entry_result->error_message, 2);
?>