<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
	require_once('admin/common/sql_interface.php');
	require_once('admin/common/map_part_definition.php');
	require_once('admin/common/file_compression.php');
	require_once('admin/common/file_splitter.php');
	require_once('admin/map_database/map_entry_remove_func.php');
	
	class AddMapEntryResult
	{
		public $success;
		public $error_message;
		
		public $messages;
	}
	
	class MapEntryReader extends SQLInterface
	{
		public $file_id;
		public $map_compression_stage;
		
		public function MapEntryReader($database, $sql) { parent::__construct($database, $sql, SQLInterfaceType::SQL_IO_TYPE_READ); }
	};
	
	class MapEntryWriter extends SQLInterface
	{
		public $map_name;
		public $map_extension;
		public $map_md5;
		public $map_uncompressed_size;
		
		public function MapEntryWriter($database, $sql) { parent::__construct($database, $sql, SQLInterfaceType::SQL_IO_TYPE_WRITE); }
	};
	
	class MapEntryUpdater extends SQLInterface
	{
		public $map_compressed_size;
		public $map_compression_type;
		public $map_parts_path;
		public $map_part_count;
		
		public function MapEntryUpdater($database, $sql) { parent::__construct($database, $sql, SQLInterfaceType::SQL_IO_TYPE_UPDATE); }
	};
	
	class MapCompressionUpdater extends SQLInterface
	{
		public $map_compression_stage;
		
		public function MapCompressionUpdater($database, $sql) { parent::__construct($database, $sql, SQLInterfaceType::SQL_IO_TYPE_UPDATE); }
	};
	
	class MapPartWriter extends SQLInterface
	{
		public $map_file_id;
		public $part_id;
		public $part_name;
		public $part_md5;
		public $part_size;
		
		public function MapPartWriter($database, $sql) { parent::__construct($database, $sql, SQLInterfaceType::SQL_IO_TYPE_WRITE); }
	};
	
	function ProcessAddMapEntry()
	{
		global $config;
		
		$result = new AddMapEntryResult();
		
		$result->messages = array();		

		// collect config variables and arguments
		$map_compressed_dir = $config->map_server->map_compressed_dir;
		$map_parts_path = $config->map_server->map_parts_path;
		$map_file = $_POST["map_file"];

		// check the arguments are valid
		if(!isset($map_file) || empty($map_file))
		{
			$result->success = false;
			$result->error_message = "ERROR: Map file parameter not set.<br/>";
			return $result;
		}
		if(!file_exists($map_file))
		{
			$result->success = false;
			$result->error_message = "Provided file does not exist.<br/>";
			return $result;
		}

		$map_file_info = pathinfo($map_file);
		$map_file_name = $map_file_info["filename"];
		$map_file_extension =  $map_file_info["extension"];
		$map_file_directory =  $map_file_info["dirname"];

		// check the extension of the map file (.map, .yelo or .xml)	
		$file_type = 0;
		if(strcmp($map_file_extension, "map") == 0)	$file_type = 1;
		if(strcmp($map_file_extension, "yelo") == 0)	$file_type = 1;
		if(strcmp($map_file_extension, "xml") == 0)	$file_type = 2;

		if($file_type == 0)
		{
			$result->success = false;
			$result->error_message = "Provided file is not valid.<br/>";
			return $result;
		}

		// create the server defined locations if they don't already exist
		if(!file_exists($map_parts_path))
		{
			if(mkdir($map_parts_path, 0760, true))
				$result->messages[] = "Created map parts folder <b>".$map_parts_path."<br/>";
			else
			{
				$result->success = false;
				$result->error_message = "Failed to create the map parts folder.<br/>";
				return $result;
			}
		}

		if(!file_exists($map_compressed_dir))
		{
			if(mkdir($map_compressed_dir, 0760, true))
				$result->messages[] = "Created compressed map folder <b>".$map_compressed_dir."<br/>";
			else
			{
				$result->success = false;
				$result->error_message = "Failed to create the compressed map output folder.<br/>";
				return $result;
			}
		}

		// fill a map definition class, either manually or from an xml
		$map_definition = null;	
		$map_name_info = null;
		switch($file_type)
		{
			case 1:
			{
				// provided file is a map, set the definition variables manually
				$map_definition = new MapDefinition();
				$map_definition->algorithm = 0;
				$map_definition->name = $map_file_name;
				$map_definition->md5 = md5_file($map_file);
				$map_definition->uncompressed_size = filesize($map_file);

				$map_name_info = pathinfo($map_file);
			}
			break;
			case 2:
			{
				// provided file is an xml, set the definition variables by reading it as a map part definition
				$map_definition = LoadDefinition($map_file);
				if($map_definition == null)
				{
					$result->success = false;
					$result->error_message = "Failed to load map part definition.<br/>";
					return $result;
				}

				$map_name_info = pathinfo($map_definition->name);
			}
			break;
		}

		// open the database
		$database = OpenDatabase($config->map_database->data_source_name,
		$config->map_database->username, $config->map_database->password);

		// look for the map in the database
		$map_entry = new MapEntryReader($database, "SELECT {0} FROM `map_list` WHERE map_name = ?");
		$map_entry->ExecuteQuery(array($map_name_info["filename"]));

		if($map_entry->MoveNext()) 
		{
			// map already in database
			if($map_entry->map_compression_stage != 1)
			{
				// the map was not added successfully so remove the entry and reprocess the map
				$result->messages[] = "WARNING: Reprocessing a map that previously failed, map will be removed and reprocessed.";

				$remove_map_entry_result = RemoveMapEntry($database, $map_entry->file_id, $map_compressed_dir);

				if(!$remove_map_entry_result->success)
				{
					$result->success = false;
					$result->error_message = $remove_map_entry_result->error_message;
					return $result;
				}
				
				$result->messages = array_merge($result->messages, $remove_map_entry_result->messages);
			}
			else
			{
				$result->success = false;
				$result->error_message = "Map <i>".$map_name_info["filename"]."</i> already exists in the database.<br/>";
				return $result;
			}

			$map_entry = NULL;
		}


		// file not in database so insert a new row in to the map_list table
		$map_writer = new MapEntryWriter($database, "INSERT INTO `map_list` ({0}) VALUES ({1})");

		$map_writer->map_name = $map_file_name;
		$map_writer->map_extension = $map_name_info["extension"];
		$map_writer->map_md5 = $map_definition->md5;
		$map_writer->map_uncompressed_size = $map_definition->uncompressed_size;

		$map_writer->ExecuteQuery(NULL);

		$file_id = $database -> lastInsertId();
		$result->messages[] = "Map entry added to database for <i>".$map_file_name."</i>";

		$parts_output_path = $map_parts_path."/".$map_file_name;
		// create the parts folder
		if(!file_exists($parts_output_path))
		{
			if(mkdir($parts_output_path, 0760, true))
				$result->messages[] = "Created output folder <b>".$parts_output_path."</b>";
			else
			{
				$result->success = false;
				$result->error_message = "Failed to create the file parts output folder.<br/>";
				return $result;
			}
		}

		// process the map file or part definition
		switch($file_type)
		{
			case 1:
			{				
				// the provided file is a map

				// compress the map
				$result->messages[] = "Compressing <b>".$map_file."</b>...";

				$output_file = $map_compressed_dir."/".$map_file_name.".zip";

				if(!CompressFiles(array($map_file, $map_file_name.".".$map_file_extension), $output_file, 0))
				{
					$result->success = false;
					$result->error_message = "Failed to create <b>".$output_file."</b>";
					return $result;
				}

				$result->messages[] = "<b>".$output_file."</b> created.";

				$map_definition->compressed_size = filesize($output_file);

				// split the archive
				$result->messages[] = "Splitting the archive...";

				if(($part_manifest = SplitFile($output_file, $parts_output_path, 1048576, false)) == null)
				{
					$result->success = false;
					$result->error_message = "Failed to split map archive.<br/>";
					return $result;
				}

				if(!unlink($output_file))
				{
					$result->success = false;
					$result->error_message = "Failed to delete temporary zip archive <b>".$output_file."</b>.<br/>";
					return $result;
				}

				// build the parts list
				for($i = 0; $i < count($part_manifest); $i++)
				{
					$part = new PartDefinition();

					$part_info = pathinfo($part_manifest[$i]);
					$part->name = $part_info["basename"];
					$part->index = $i;
					$part->md5 = md5_file($part_manifest[$i]);
					$part->size = filesize($part_manifest[$i]);

					$map_definition->part[] = $part;
				}
			}
			break;
			case 2:
			{
				$result->messages[] = "Copying map parts to web directory...";
				foreach($map_definition->part as $value)
				{
					// verify map part
					if($value->encrypted)
						$result->messages[] = "WARNING: Encrypted part encountered. If this is a master server do NOT use encrypted maps.";

					$part_source_path = $map_file_directory."/".$value->name;				
					if(!file_exists($part_source_path))
					{
						$result->success = false;
						$result->error_message = "A map part is missing.<br/>";
						return $result;
					}
					
					if(strcasecmp(md5_file($part_source_path), $value->md5) != 0)
					{
						$result->success = false;
						$result->error_message = "Uploaded part's md5 does not match that in the definition.<br/>";
						return $result;
					}

					// copy part to parts folder
					if(!copy($part_source_path, $parts_output_path."/".$value->name))
					{
						$result->success = false;
						$result->error_message = "Failed to copy a part to the parts folder.<br/>";
						return $result;
					}
				}
			}
			break;
		}

		// add parts to database
		$part_entry = new MapPartWriter($database, "INSERT INTO `map_part_list` ({0}) VALUES ({1})");
		for($i = 0; $i < count($map_definition->part); $i++)
		{                
			$part_entry->map_file_id = $file_id;
			$part_entry->part_id = $map_definition->part[$i]->index;
			$part_entry->part_name = $map_definition->part[$i]->name;
			$part_entry->part_md5 = $map_definition->part[$i]->md5;
			$part_entry->part_size = $map_definition->part[$i]->size;

			$result->messages[] = "Part: <i>".$part_entry->part_name."</i>	MD5: <i>".$part_entry->part_md5."</i>";

			$part_entry->ExecuteQuery(NULL);
		}
		$result->messages[] = "Part entries added to database.";

		// update the maps database with the parts path
		$map_entry_update = new MapEntryUpdater($database, "UPDATE `map_list` SET {0} WHERE file_id = ?");

		$map_entry_update->map_compressed_size = $map_definition->compressed_size;
		$map_entry_update->map_compression_type = $map_definition->algorithm;
		$map_entry_update->map_parts_path = $map_parts_path."/".$map_file_name;
		$map_entry_update->map_part_count = count($map_definition->part);

		$map_entry_update->ExecuteQuery(array($file_id));

		// update map progress to process completed
		$compression_stage = new MapCompressionUpdater($database, "UPDATE `map_list` SET {0} WHERE      file_id = ?");
		$compression_stage->map_compression_stage = 1;
		$compression_stage->ExecuteQuery(array($file_id));

		$result->messages[] = "Map added to server successfully!";
		
		$result->success = true;
		return $result;
	}
?>