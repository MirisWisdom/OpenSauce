<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
	require_once("admin/common/map_part_definition.php");
	
	class MapFileDeleteResult
	{
		public $success;
		public $error_message;
		
		public $messages;
	}
	
	function ProcessMapFileDelete()
	{
		global $can_delete_map_entries;
		
		$result = new MapFileDeleteResult();
		
		$result->messages = array();
		
		// only allow file deletion by users that have the required permissions
		if(!$can_delete_map_entries)
		{
			$can_delete_map_entries->success = false;
			$can_delete_map_entries->error_message = "Your user account does not have sufficient privileges to delete map files.";
			return $result;
		}
		
		// check the required value was set
		if(!isset($_POST["map_file_delete"]) || empty($_POST["map_file_delete"]))
		{
			$can_delete_map_entries->success = false;
			$can_delete_map_entries->error_message = "Map file to delete has not been set.";
			return $result;
		}

		$map_file = $_POST["map_file_delete"];

		// file must exist for it to be deleted...
		if(!file_exists($map_file))
		{
			$can_delete_map_entries->success = false;
			$can_delete_map_entries->error_message = "File does not exist. (".$map_file.")";
			return $result;
		}

		// determine whether it is an xml or map/yelo
		$file_info = pathinfo($map_file);

		$file_type = 0;
		if(strcmp("map", $file_info["extension"]) == 0)		$file_type = 1;
		if(strcmp("yelo", $file_info["extension"]) == 0)	$file_type = 1;
		if(strcmp("xml", $file_info["extension"]) == 0)		$file_type = 2;

		if($file_type == 0)
		{
			$can_delete_map_entries->success = false;
			$can_delete_map_entries->error_message = "File is not a map or map part definition. (".$map_file.")";
			return $result;
		}
		
		if($file_type == 2)
		{
			// the file is a map part definition
			$map_definition = LoadDefinition($map_file);

			if($map_definition == null)
			{
				$can_delete_map_entries->success = false;
				$can_delete_map_entries->error_message = "Failed to load map part definition.";
				return $result;
			}

			// iterate though all parts in the definition
			foreach($map_definition->part as $value)
			{
				$part_file = $file_info["dirname"]."/".$value->name;

				// delete the part
				if(file_exists($part_file))
				{
					if(!unlink($part_file))
					{
						$can_delete_map_entries->success = false;
						$can_delete_map_entries->error_message = "Failed to delete file. (".$part_file.")";
						return $result;
					}
					else
						$result->messages[] = "File deleted successfully. (".$part_file.")";
				}
				else
					$result->messages[] = "WARNING: File does not exist:".$part_file."";
			}
		}

		// try and delete the file
		if(!unlink($map_file))
			$result->messages[] = "WARNING: Failed to delete file. (".$map_file.")";
		else
			$result->messages[] = "File deleted successfully. (".$map_file.")";
		
		$result->success = true;
		return $result;
	}
	
	$map_file_delete_result = ProcessMapFileDelete();
	
	foreach($map_file_delete_result->messages as $value)
		print_line_inset($value."<br/>", 2);
	
	if(!$map_file_delete_result->success)
		print_line_inset($map_file_delete_result->error_message, 2);
?>