<!--
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
-->
<?php
	require_once('includes/load_config.php');
	require_once('includes/map_part_definition.php');

	$map_file = $_POST["map_file"];
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
			
			// check the required value was set
			if(!isset($map_file) || empty($map_file))
				die("ERROR: Map file to delete has not been set.<br/>");

			// file must exist for it to be deleted...
			if(!file_exists($map_file))
				die("ERROR: File does not exist. (".$map_file.")<br/>");

			// determine whether it is an xml or map/yelo
			$file_info = pathinfo($map_file);
			
			$file_type = 0;
			if(strcmp("map", $file_info["extension"]) == 0)		$file_type = 1;
			if(strcmp("yelo", $file_info["extension"]) == 0)	$file_type = 1;
			if(strcmp("xml", $file_info["extension"]) == 0)		$file_type = 2;
			
			if($file_type == 0)
				die("ERROR: File is not a map or map part definition</br>");
			else if($file_type == 2)
			{
				$map_definition = LoadDefinition($map_file);
				
				if($map_definition == null)
					die("ERROR: Failed to load map part definition</br>");
				
				foreach($map_definition->part as $value)
				{
					$part_file = $file_info["dirname"]."/".$value->name;
					
					if(file_exists($part_file))
					{						
						if(!unlink($part_file))
							die("ERROR: Failed to delete file. (".$part_file.")<br/>");
						else
							print("File deleted successfully. (".$part_file.")<br/>");
					}
					else
						print("WARNING: File does not exist:".$part_file."</br>");					
				}
			}
			
			// try and delete the file
			if(!unlink($map_file))
				die("ERROR: Failed to delete file. (".$map_file.")<br/>");
			else
				print("File deleted successfully. (".$map_file.")<br/>");
		?>
	</div>
</body>
</html>