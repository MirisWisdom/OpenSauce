<!--
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
-->
<?php
	require_once('includes/load_config.php');
	require_once('includes/open_database.php');
	require_once('includes/remove_map_func.php');
	require_once('includes/sql_interface.php');
	require_once('includes/map_part_definition.php');
	require_once('includes/file_compression.php');
	require_once('includes/file_splitter.php');

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
		
		public function MapEntryWriter($database, $sql) { parent::__construct($database, $sql, SQLInterfaceType::SQL_IO_TYPE_WRITE); }
	};
	
	class MapEntryUpdater extends SQLInterface
	{
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
		
		public function MapPartWriter($database, $sql) { parent::__construct($database, $sql, SQLInterfaceType::SQL_IO_TYPE_WRITE); }
	};
	
	// increase script timeout value
	ini_set('max_execution_time', 5000);

	// collect config variables and arguments
	$compressed_file_dir = $config->map_server->map_compressed_dir;
	$http_parts_path = $config->map_server->map_parts_path;
	$http_parts_path_full = $_SERVER['DOCUMENT_ROOT']."/".$http_parts_path;
	$map_file = $_POST["map_file"];

	// check the arguments are valid
	if(!isset($map_file) || empty($map_file))
		die("ERROR: Map file parameter not set.<br/>");
	if(!file_exists($map_file))
		die("ERROR: Provided file does not exist.<br/>");

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
		die("ERROR: Provided file is not valid<br/>");

	// create the server defined locations if they don't already exist
	if(!file_exists($http_parts_path_full))
	{
		if(mkdir($http_parts_path_full, 0757, true))
			print("Created map parts folder <b>".$http_parts_path_full."</b></br>");
		else
			die("ERROR: Failed to create the map parts folder</br>");
	}

	if(!file_exists($compressed_file_dir))
	{
		if(mkdir($compressed_file_dir, 0757, true))
			print("Created compressed map folder <b>".$compressed_file_dir."</b></br>");
		else
			die("ERROR: Failed to create the compressed map output folder</br>");
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
			
			$map_name_info = pathinfo($map_file);
		}
		break;
		case 2:
		{
			// provided file is an xml, set the definition variables by reading it as a map part definition
			$map_definition = LoadDefinition($map_file);
			if($map_definition == null)
				die("ERROR: Failed to load map part definition</br>");
			
			$map_name_info = pathinfo($map_definition->name);
		}
		break;
	}
	
	// open the database
	$database = OpenDatabase($config->map_database->data_source_name,
		$config->map_database->username,
		$config->map_database->password);
	
	// look for the map in the database
	$map_entry = new MapEntryReader($database, "SELECT {0} FROM `map_list` WHERE map_name = ?");
	$map_entry->ExecuteQuery(array($map_name_info["filename"]));

	if($map_entry->MoveNext()) 
	{
		// map already in database
		if($map_entry->map_compression_stage != 1)
		{
			// the map was not added successfully so remove the entry and reprocess the map
			print("WARNING: Reprocessing a map that previously failed, map will be removed and reprocessed.<br/>");

			RemoveMapEntry($database, $map_entry->file_id, $compressed_file_dir);
		}
		else
			die("Map <i>".$map_name_info["filename"]."</i> already exists in the database.</br>");
		
		$map_entry = NULL;
	}
	

	// file not in database so insert a new row in to the map_list table
	$map_writer = new MapEntryWriter($database, "INSERT INTO `map_list` ({0}) VALUES ({1})");
	
	$map_writer->map_name = $map_file_name;
	$map_writer->map_extension = $map_name_info["extension"];
	$map_writer->map_md5 = $map_definition->md5;
	
	$map_writer->ExecuteQuery(NULL);
	
	$file_id = $database -> lastInsertId();
	print("Map entry added to database for <i>".$map_file_name."</i></br>");
	
	// create the parts folder
	$parts_output_path = $http_parts_path_full."/".$map_file_name;
	if(!file_exists($parts_output_path))
	{
		if(mkdir($parts_output_path, 0757, true))
			echo "Created output folder <b>".$parts_output_path."</b></br>";
		else
			die("Failed to create the file parts output folder</br>");
	}
	
	// process the map file or part definition
	switch($file_type)
	{
		case 1:
		{
			// the provided file is a map

			// compress the map
			print("Compressing <b>".$map_file."</b>...</br>");
			
			$output_file = $compressed_file_dir."/".$map_file_name.".".$map_file_extension.".zip";
			
			if(!CompressFiles(array($map_file, $map_file_name.".".$map_file_extension), $output_file, 0))
				die("ERROR: failed to create <b>".$output_file.".zip</b></br>");
			
			print("<b>".$output_file."</b> created</br>");

			// split the archive
			print("Splitting the archive...</br>");
			
			if(($part_manifest = SplitFile($output_file, $parts_output_path, 1048576)) == null)
				die("ERROR: Failed to split map archive</br>");
			
			if(!unlink($output_file))
				die("ERROR: Failed to delete temporary zip archive <b>".$output_file."</b></br>");
	
			// build the parts list
			for($i = 0; $i < count($part_manifest); $i++)
			{
				$part = new PartDefinition();
				
				$part_info = pathinfo($part_manifest[$i]);
				$part->name = $part_info["basename"];
				$part->index = $i;
				$part->md5 = md5_file($part_manifest[$i]);
				
				$map_definition->part[] = $part;
			}
		}
		break;
		case 2:
		{
			// the provided file is a map part definition
			
			print("Copying map parts to web directory...</br>");
			foreach($map_definition->part as $value)
			{
				// verify map part
				if($value->encrypted)
					print("WARNING: Encrypted part encountered. If this is a master server do NOT use encrypted maps.");

				$part_source_path = $map_file_directory."/".$value->name;				
				if(!file_exists($part_source_path))
					die("ERROR: A map part is missing");
				if(strcasecmp(md5_file($part_source_path), $value->md5) != 0)
					die("ERROR: Uploaded part's md5 does not match that in the definition");
				
				// copy part to parts folder
				if(!copy($part_source_path, $parts_output_path."/".$value->name))
					die("ERROR: Failed to copy a part to the parts folder");
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
				
		print("Part: <i>".$part_entry->part_name."</i>	MD5: <i>".$part_entry->part_md5."</i></br>");
				
		$part_entry->ExecuteQuery(NULL);
	}
	print("Part entries added to database</br>");
	
	// update the maps database with the parts path
	$map_entry_update = new MapEntryUpdater($database, "UPDATE `map_list` SET {0} WHERE file_id = ?");
	
	$map_entry_update->map_compression_type = $map_definition->algorithm;
	$map_entry_update->map_parts_path = $http_parts_path."/".$map_file_name;
	$map_entry_update->map_part_count = count($map_definition->part);
	
	$map_entry_update->ExecuteQuery(array($file_id));

	// update map progress to process completed
	$compression_stage = new MapCompressionUpdater($database, "UPDATE `map_list` SET {0} WHERE      file_id = ?");
	$compression_stage->map_compression_stage = 1;
	$compression_stage->ExecuteQuery(array($file_id));

	$database = NULL;
	
	print("Map added to server successfully!</br>");
?>