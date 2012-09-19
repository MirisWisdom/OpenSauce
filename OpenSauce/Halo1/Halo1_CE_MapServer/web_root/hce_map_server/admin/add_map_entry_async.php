<!--
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
-->
<?php
	header("Cache-Control: no-store, no-cache, must-revalidate");
	
	$map_file = $_POST["map_file"];
?>
</html>
</head>
	<link rel="stylesheet" href="../css/mainstyle.css" type="text/css" />
	<title>OpenSauce Halo CE Map Server</title>
	<script type="text/javascript">
		<?php
			if(isset($map_file) && !empty($map_file))
				print("document.onload = StartMapCompression();");
		?>
		
		function StartMapCompression()
		{
			var request = new XMLHttpRequest();

			request.onreadystatechange=function()
			{
				if (request.readyState==4 && request.status==200)
				{
					document.getElementById("contents").innerHTML = request.responseText;

					document.getElementById("return_button").disabled = false;
				}
			}

			request.open("POST", "add_map_entry.php", true);
			request.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
			request.send("map_file=" + <?php print("\"".$map_file."\""); ?>);
		}
	</script>
</head>

<body>
	<div id='title'>
		<h1>OpenSauce Halo CE Map Server</h1>
	</div>

	<div id='return_link'>
		<form method='link' action='../server_admin.php'>
			<input id="return_button" type='submit' value='Return to Main Page' disabled="disabled"/>
		</form>
	</div>

	<div id='contents'>
		<?php
			if(isset($map_file) && !empty($map_file))
				print("Adding map entry, please wait...");
			else
				die("ERROR: No map file posted with request.");
		?>
	</div>
</body>
</html>