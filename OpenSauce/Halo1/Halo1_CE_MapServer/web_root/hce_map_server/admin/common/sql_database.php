<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
	function OpenDatabase($data_source_name, $username, $password)
	{
		try
		{	
			$dbConnection = new PDO($data_source_name, $username, $password);
			$dbConnection->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
		} 
		catch(PDOException $e) 
		{
			die ("ERROR: Failed to open the SQL database. (".$e.")</br>"); // Exit, displaying an error message
		}
		return $dbConnection;
	}
	
	// open the sql database
	$database = OpenDatabase($config->map_database->data_source_name,
	$config->map_database->username, $config->map_database->password);
?>