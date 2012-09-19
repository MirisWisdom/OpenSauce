<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
	class MapListEntry
	{
		public $map_name;
		public $file_name;
		
		function MapListEntry($name, $file) 
		{
			$this->map_name = $name;
			$this->file_name = $file;
		}
	}
	function GetFolderMapList($folder = NULL)
	{
		if($folder == NULL)
			die("Path passed to GetFolderMapList is empty");
				
		// open the maps folder and iterate through its contents
		$handle = opendir($folder);
		
		if(!$handle)
		{
			print("ERROR: Failed to open the maps directory</br>");
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
					$map_list[] = new MapListEntry($file_info["filename"], $file_location, true);
					break;
				case 2:
				{
					if(($map_definition = LoadDefinition($file_location)) != null)
					{
						$map_info = pathinfo($map_definition->name);
						$map_list[] = new MapListEntry($map_info["filename"], $file_location, true);
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
?>
