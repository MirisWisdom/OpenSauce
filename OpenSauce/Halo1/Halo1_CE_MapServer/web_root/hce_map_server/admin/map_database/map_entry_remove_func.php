<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
	require_once("admin/common/sql_interface.php");
	
	class RemoveMapEntryResult
	{
		public $success;
		public $error_message;
		
		public $messages;
	}
	
	class RemoveMapEntryReader extends SQLInterface
	{
		public $map_name;
		public $map_extension;
		public $map_compression_stage;
		public $map_compression_type;
		public $map_parts_path;
		
		public function RemoveMapEntryReader($database, $sql) { parent::__construct($database, $sql, SQLInterfaceType::SQL_IO_TYPE_READ); }
	};
	
	class RemovePartEntryReader extends SQLInterface
	{
		public $part_name;
		
		public function RemovePartEntryReader($database, $sql) { parent::__construct($database, $sql, SQLInterfaceType::SQL_IO_TYPE_READ); }
	};
	
	function RemoveMapEntry($database, $file_id, $map_compressed_dir)
	{
		$result = new RemoveMapEntryResult();
		
		$result->messages = array();
		
		// search for the map in the database
		$map_entry = new RemoveMapEntryReader($database, "SELECT {0} FROM `map_list` WHERE file_id = ?");
		$map_entry->ExecuteQuery(array($file_id));

		if($map_entry->MoveNext()) 
		{
			// loop through each part in the database associated with this map
			$part_entry = new RemovePartEntryReader($database, "SELECT {0} FROM `map_part_list` WHERE map_file_id = ?");
			$part_entry->ExecuteQuery(array($file_id));

			while($part_entry->MoveNext())
			{
				// delete the part file
				$part_file = $map_entry->map_parts_path."/".$part_entry->part_name;

				if(file_exists($part_file))
				{
					if(unlink($part_file))
						$result->messages[] = "Deleting ".$part_file."...done.";
					else
						$result->messages[] = "Deleting ".$part_file."...failed.";
				}
				else
					$result->messages[] = "File ".$part_file." does not exist...skipped.";
			}

			// delete the part entries in the database
			$sql = "DELETE FROM
						map_part_list
					WHERE
						map_file_id = ?";

			$part_query = $database->prepare($sql);
			if($part_query->execute(array($file_id)))
				$result->messages[] = "Deleting part database entries for ".$map_entry->map_name."...done.";
			else
				$result->messages[] = "Deleting part database entries for ".$map_entry->map_name."...failed.";
			
			if(isset($map_entry->map_parts_path) && !empty($map_entry->map_parts_path))
			{
				// delete the parts folder
				$parts_folder = $map_entry->map_parts_path;

				if(file_exists($parts_folder))
				{
					if(rmdir($parts_folder))
						$result->messages[] = "Deleting ".$parts_folder."...done.";
					else
						$result->messages[] = "Deleting ".$parts_folder."...failed.";
				}
				else
					$result->messages[] = "Folder ".$parts_folder." does not exist...skipped.";
			}
			
			// delete the map archive
			$archive_file = $map_compressed_dir."/".$map_entry->map_name.".".$map_entry->map_extension;

			switch($map_entry->map_compression_type)
			{
			case 1:
				$archive_file = $archive_file.".7z";
				break;
			default:
				$archive_file = $archive_file.".zip";						
			}

			if(file_exists($archive_file))
			{
				if(unlink($archive_file))
					$result->messages[] = "Deleting ".$archive_file."...done.";
				else
					$result->messages[] = "Deleting ".$archive_file."...failed.";
			}
			else
				$result->messages[] = "File ".$archive_file." does not exist...skipped.";
			
			// delete the map database entry
			$sql = "DELETE FROM
						map_list
					WHERE
						file_id = ?";

			$map_delete_query = $database->prepare($sql);
			if($map_delete_query->execute(array($file_id)))
				$result->messages[] = "Deleting the map database entry for ".$map_entry->map_name."...done.";
			else
				$result->messages[] = "Deleting the map database entry for ".$map_entry->map_name."...failed.";
		}
		else
		{
			$result->messages[] = "Map with an ID of ".$file_id." is not present in the map database";
		}
		
		$result->success = true;
		return $result;
	}
?>