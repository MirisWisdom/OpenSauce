<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
	require_once("admin/common/INIBlockIO.php");
	
	class StateTimeBlock extends INIBlockIO
	{
		public $updated_datetime;
		
		function __construct() {
			parent::INIBlockIO("state_time");
		}
	};
	
	class ServerBandwidthBlock extends INIBlockIO
	{
		public $bandwidth_used;
		
		function __construct() {
			parent::INIBlockIO("server_bandwidth");
		}
	};
	
	class ServerState extends INIBlockIO
	{
		public $state_time;
		public $server_bandwidth;
		
		function __construct()
		{
			$this->state_time = new StateTimeBlock();
			$this->server_bandwidth = new ServerBandwidthBlock();
			
			parent::INIBlockIO("");
		}
	};
	
	$state_path = "";
	
	function LoadState()
	{
		global $state_path;

		if(empty($state_path))
			die("ERROR: state.ini path not yet set in server_state.php<br/>");

		// if the state ini exists, parse it
		$state = new ServerState();
		if(file_exists($state_path))
		{		
			$state_file = parse_ini_file($state_path, true);
			$state->ReadBlock($state_file);
		}
		
		// if the update datetime is set, use it reset the bandwidth count if applicable
		if(!empty($state->state_time->updated_datetime))
		{
			// get the current date and update date
			$current_datetime = getdate();
			$updated_datetime = getdate($state->state_time->updated_datetime);
			
			// if if a day has elapsed, reset the bandwidth usage counter to 0
			if(($current_datetime['year'] > $updated_datetime['year']) ||
				($current_datetime['month'] > $updated_datetime['month']) ||
				($current_datetime['mday'] > $updated_datetime['mday']))
			{
				// reset bandwidth usage for the day
				$state->server_bandwidth->bandwidth_used = 0;
			}
		}
		else
		{
			$state->server_bandwidth->bandwidth_used = 0;
		}
		
		return $state;
	}
	
	function SaveState($state)
	{
		global $state_path;
		
		if(!$state_file = fopen($state_path, "w+"))
			die("ERROR: could not open state.ini for writing.<br/>");
		
		if(!flock($state_file, LOCK_EX))
			die("ERROR: could not lock state.ini for writing.<br/>");
		
		// set the time the state ini was updated
		$state->state_time->updated_datetime = time();
		$state->WriteBlock($state_file);
		
		flock($state_file, LOCK_UN);
		fclose($state_file);		
	}
?>
