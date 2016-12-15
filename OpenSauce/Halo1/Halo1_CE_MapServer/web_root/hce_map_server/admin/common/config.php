<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
	require_once("INIBlockIO.php");
	
	class MapDatabaseBlock extends INIBlockIO
	{
		public $username_readonly;
		public $password_readonly;
		public $username;
		public $password;
		public $data_source_name;
		
		function __construct() {
			parent::INIBlockIO("map_database");
		}
	};
	
	class MapServerBlock extends INIBlockIO
	{
		public $map_dir;
		public $map_compressed_dir;
		public $map_parts_path;
		public $bandwidth_cap_soft;
		public $bandwidth_cap_hard;
		
		function __construct() {
			parent::INIBlockIO("map_server");
		}
	};
	
	class ServerConfig extends INIBlockIO
	{
		public $map_database;
		public $map_server;
		
		public function ServerConfig()
		{
			$this->map_database = new MapDatabaseBlock();
			$this->map_server = new MapServerBlock();
			
			parent::INIBlockIO("");
		}
	};
	
	function LoadConfig()
	{
		$config_path = "/usr/local/opensauce/config.ini";

		if(empty($config_path))
			die("ERROR: config.ini path not yet set in config.php<br/>");

		$config_file = parse_ini_file($config_path, true);
		if($config_file == FALSE)
			die("ERROR: failed to load config.ini from the defined path. (".$config_path.")<br/>");

		$config = new ServerConfig();
		$config->ReadBlock($config_file);
		
		return $config;
	}
	
	$config = LoadConfig();
?>