<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
	class INIBlockReader
	{		
		private $class_reflection;
		
		public function INIBlockReader() { $this->class_reflection = new ReflectionClass($this); }
		
		public function ReadBlock($array)
		{
			foreach($this as $key => &$value)
			{
				// load each property from the object
				if(!$this->class_reflection->hasProperty($key))
					continue;
					
				$property = $this->class_reflection->getProperty($key);
				if(!$property->isPublic())
					continue;
				
				// if the property is an object find out if its base is INIBlockReader
				if(is_object($value))
				{
					$reflection = new ReflectionClass($value);
					do
					{
						if(strcmp($reflection->name, "INIBlockReader") == 0)
							break;
					}while($reflection = $reflection->getParentClass());
					
					if(!$reflection)
						continue;
					
					// read the block from the ini array
					$value->ReadBlock($array[$key]);
				}
				else
				{
					$value = $array[$key];
				}
			}
		}
	};
		
	class MapDatabaseBlock extends INIBlockReader
	{
		public $username_readonly;
		public $password_readonly;
		public $username;
		public $password;
		public $data_source_name;
	};
	
	class MapServerBlock extends INIBlockReader
	{
		public $server_address;
		public $map_dir;
		public $map_compressed_dir;
		public $map_parts_path;
	};
	
	class ServerConfig extends INIBlockReader
	{
		public $map_database;
		public $map_server;
		
		public function ServerConfig()
		{
			$this->map_database = new MapDatabaseBlock();
			$this->map_server = new MapServerBlock();
			
			parent::__construct();
		}
	};
	
	$config_path = "";
	
	if(empty($config_path))
		die("ERROR: config.ini path not yet set in load_config.php</br>");
		
	$config_file = parse_ini_file($config_path, true);

	if($config_file == FALSE)
		die("ERROR: failed to load config.ini from the defined path. (".$config_path.")</br>");
		
	$config = new ServerConfig();
	$config->ReadBlock($config_file);
	
	$config_file = NULL;
?>