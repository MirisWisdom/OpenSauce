<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
	require_once("admin/common/map_part_definition.php");
	
	class MapFileDetails
	{
		public $map_name;
		public $file_name;
		
		function MapFileDetails($name, $file) 
		{
			$this->map_name = $name;
			$this->file_name = $file;
		}
	}
	
	class MapFileEntry
	{
		public $map_name;
		public $map_in_database;
		public $map_processed;
		public $map_file_name;
	}
	
	class MapCompressionReader extends SQLInterface
	{
		public $map_compression_stage;
		
		public function MapCompressionReader($database, $sql) { parent::__construct($database, $sql, SQLInterfaceType::SQL_IO_TYPE_READ); }
	};
	
	function GetMapsInFolder($folder)
	{
		if($folder == NULL)
			die("ERROR: Path passed to GetFolderMapList is empty.<br/>");
				
		// open the maps folder and iterate through its contents
		$handle = opendir($folder);
		
		if(!$handle)
		{
			print_line_inset("Failed to open the maps directory.<br/>", 2);
			return null;
		}
		
		$map_list = array();
		
		while (false !== ($entry = readdir($handle))) 
		{
			if((strcmp($entry, ".") == 0) || (strcmp($entry, "..") == 0))
				continue;
			
			// check the extension of the file (.map, .yelo or .xml)
			$file_info = pathinfo($entry);
			
			$file_type = 0;
			if(strcmp($file_info["extension"], "map") == 0)		$file_type = 1;
			if(strcmp($file_info["extension"], "yelo") == 0)	$file_type = 1;
			if(strcmp($file_info["extension"], "xml") == 0)		$file_type = 2;
			
			$file_location = $folder."/".$entry;
			switch($file_type)
			{
				case 1:
					$map_list[] = new MapFileDetails($file_info["filename"], $file_location, true);
					break;
				case 2:
				{
					if(($map_definition = LoadDefinition($file_location)) != null)
					{
						$map_info = pathinfo($map_definition->name);
						$map_list[] = new MapFileDetails($map_info["filename"], $file_location, true);
					}
					else
						continue;
				}
				break;
					
				default:					
					continue;
			}
		}
		closedir($handle);
		
		return $map_list;
	}
	
	function GetMapFileList($directory, $database)
	{		
		// loop through all maps in the map directory
		$map_files = GetMapsInFolder($directory);

		$map_list = array();
		if($map_files != null)
		{
			foreach($map_files as $value)
			{
				$map_file = new MapFileEntry();
				
				$map_file->map_name = $value->map_name;
				$map_file->map_in_database = "No";
				$map_file->map_processed = "No";
				$map_file->map_file_name = $value->file_name;

				// look for the map in the database
				$compression_entry = new MapCompressionReader($database, "SELECT {0} FROM `map_list` WHERE map_name = ?");
				$compression_entry->ExecuteQuery(array($value->map_name));

				// if the map was found determine whether it was added successfully
				if($compression_entry->MoveNext()) 
				{
					$map_file->map_in_database = "Yes";

					if($compression_entry->map_compression_stage == 1)
						$map_file->map_processed = "Yes";
				}
				$map_list[] = $map_file;
			}
		}
		return $map_list;
	}
?>
