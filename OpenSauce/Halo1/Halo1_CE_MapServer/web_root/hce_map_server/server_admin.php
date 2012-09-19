<!--
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
-->
<?php
	require_once('admin/includes/load_config.php');
	require_once('admin/includes/open_database.php');
	require_once('admin/includes/sql_interface.php');
	require_once('admin/includes/map_part_definition.php');
	require_once('admin/includes/folder_map_list.php');
	
	// required to force a refresh of the tables when reloaded
	header("Cache-Control: no-store, no-cache, must-revalidate");
        
	// open the sql database
        $database = OpenDatabase($config->map_database->data_source_name,
        $config->map_database->username,
        $config->map_database->password);
?>
</html>
</head>
    <link rel="stylesheet" href="css/mainstyle.css" type="text/css" />
	<title>OpenSauce Halo CE Map Server</title>
</head>

<body>
	<div id='title'>
		<h1>OpenSauce Halo CE Map Server</h1>
	</div>

	<div id='contents'>
		<div id='map_file_list'>
			<h2>Map Files</h2>
			<table>
				<tr>
					<th>Map File</th>
					<th>In Database</th>
					<th>Added Successfully</th>
					<th>Add Entry</th>
					<th>Delete File</th>
				</tr>

				<?php
					class MapCompressionReader extends SQLInterface
					{
						public $map_compression_stage;
						
						function MapCompressionReader($database, $sql) { parent::__construct($database, $sql, SQLInterfaceType::SQL_IO_TYPE_READ); }
					};
					
					$map_list = GetFolderMapList($config->map_server->map_dir);					
					
					if($map_list != null)
					{
						foreach($map_list as $value)
						{
							$map_in_database = "No";
							$map_processed = "No";

							// look for the map in the database
							$compression_entry = new MapCompressionReader($database, "SELECT {0} FROM `map_list` WHERE map_name = ?");
							$compression_entry->ExecuteQuery(array($value->map_name));
							
							// if the map was found determine whether it was added successfully
							if($compression_entry->MoveNext()) 
							{
								$map_in_database = "Yes";
								
								if($compression_entry->map_compression_stage == 1)
									$map_processed = "Yes";
							}

							// print the table entries
							print("<tr>");
								print("<td>".$value->map_name."</td>");
								print("<td>".$map_in_database."</td>");
								print("<td>".$map_processed."</td>");
								print("<td>");
									print("<form name='map_addition_form' method='POST' action='admin/add_map_entry_async.php'>");
										print("<input type='hidden' name='map_file' value='".$value->file_name."'>");
										print("<input type='submit' value='Add Entry'/>");
									print("</form>");
								print("</td>");
								print("<td>");
									print("<form name='map_deletion_form' method='POST' action='admin/delete_map_file.php'>");
										print("<input type='hidden' name='map_file' value='".$value->file_name."'>");
										print("<input type='submit' value='Delete File'/>");
									print("</form>");
								print("</td>");
							print("</tr>");
						}
					}
				?>
			</table>
		</div>
		
		<div id='contents_gap'>
		</div>

		<div id='map_database'>
			<h2>Map Database</h2>
			<table>
				<tr>
					<th>Name</th>
					<th>Extension</th>
					<th>Added Successfully</th>
					<th>Remove Entry</th>
				</tr>

				<?php
					class MapEntryReader extends SQLInterface
					{
						public $file_id;
						public $map_name;
						public $map_extension;
						public $map_compression_stage;
						
						function MapEntryReader($database, $sql) { parent::__construct($database, $sql, SQLInterfaceType::SQL_IO_TYPE_READ); }
					};

					// loop through all maps in the database
					$map_entry = new MapEntryReader($database, "SELECT {0} FROM `map_list`");
					$map_entry->ExecuteQuery(NULL);
					
					// iterate though all of the maps in the database
					while($map_entry->MoveNext())
					{
						// determine whether an error occured when adding the map entry
						$map_processed = "No";
						if($map_entry->map_compression_stage == 1)
							$map_processed = "Yes";

						// print the table entry
						print("<tr>");
							print("<td>".$map_entry->map_name."</td>");
							print("<td>".$map_entry->map_extension."</td>");
							print("<td>".$map_processed."</td>");
							print("<td>");
								print("<form name='map_addition_form' method='POST' action='admin/remove_map_entry.php'>");
									print("<input type='hidden' name='file_id' value='".$map_entry->file_id."'>");
									print("<input type='submit' value='Remove Entry'>");
								print("</form>");
							print("</td>");
						print("</tr>");
					}

				?>
			</table>
		</div>
	</div>
</body>
</html>
<?php
    $database = NULL;
?>