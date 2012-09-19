<!--
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
-->
<?php
	require_once('includes/load_config.php');
	require_once('includes/open_database.php');
	require_once('includes/remove_map_func.php');
	
	header("Cache-Control: no-store, no-cache, must-revalidate");
?>
</html>
</head>
    <link rel="stylesheet" href="../css/mainstyle.css" type="text/css" />
	<title>OpenSauce Halo CE Map Server</title>
</head>

<body>
	<div id='title'>
		<h1>OpenSauce Halo CE Map Server</h1>
	</div>

	<div id='return_link'>
		<form method='link' action='../server_admin.php'>
			<input type='submit' value='Return to Main Page'/>
		</form>
	</div>

	<div id='contents'>
		<?php
			$database = OpenDatabase($config->map_database->data_source_name,
				$config->map_database->username,
				$config->map_database->password);

			$file_id_set = isset($_POST['file_id']) && !empty($_POST['file_id']);

			if(!$file_id_set)
				die("ERROR: No file id provided");

			RemoveMapEntry($database, $_POST['file_id'], $config->map_server->map_compressed_dir);

			$database = NULL;
		?>
	</div>
</body>
</html>