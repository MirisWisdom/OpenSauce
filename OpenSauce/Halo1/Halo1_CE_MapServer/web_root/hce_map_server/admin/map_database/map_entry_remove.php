<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
	include_once("admin/map_database/map_entry_remove_func.php");
	
	if(!$can_delete_map_entries)
	{
		print_line_inset("Your user account does not have sufficient privileges to remove map entries.</br>", 2);
	}
	else
	{
		$file_id_set = isset($_POST['map_entry_remove']) && !empty($_POST['map_entry_remove']);

		if(!$file_id_set)
			print_line_inset("No file id provided", 2);
		else
		{
			$map_entry_remove_result = RemoveMapEntry($database, $_POST['map_entry_remove'], $config->map_server->map_compressed_dir);
			
			foreach($map_entry_remove_result->messages as $value)
				print_line_inset($value."<br/>", 2);
			
			if(!$map_entry_remove_result->success)
				print_line_inset($map_entry_remove_result->error_message, 2);
		}
	}
?>
