<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
	class MapListEntry
	{
		public $map_name;
		public $map_extension;
		public $map_processed;
		public $map_file_id;
	}
	
	class MapEntryReader extends SQLInterface
	{
		public $file_id;
		public $map_name;
		public $map_extension;
		public $map_compression_stage;
		
		public function MapEntryReader($database, $sql) { parent::__construct($database, $sql, SQLInterfaceType::SQL_IO_TYPE_READ); }
	};
	
	function GetMapEntryList($database)
	{		
		// iterate though all of the maps in the database
		$map_entry_reader = new MapEntryReader($database, "SELECT {0} FROM `map_list`");
		$map_entry_reader->ExecuteQuery(NULL);

		$map_entry_list = array();
		while($map_entry_reader->MoveNext())
		{
			$map_entry = new MapListEntry();
			
			// determine whether an error occured when adding the map entry
			$map_entry->map_processed = "No";
			if($map_entry_reader->map_compression_stage == 1)
				$map_entry->map_processed = "Yes";

			$map_entry->map_name = $map_entry_reader->map_name;
			$map_entry->map_extension = $map_entry_reader->map_extension;
			$map_entry->map_file_id = $map_entry_reader->file_id;
			
			$map_entry_list[] = $map_entry;
		}
		return $map_entry_list;
	}
?>
