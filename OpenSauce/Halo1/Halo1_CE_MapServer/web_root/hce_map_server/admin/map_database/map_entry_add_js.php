<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
		$print_jscript = false;
		$map_file = null;
		if($can_create_map_entries)
		{
			$map_file = $_POST['map_entry_add'];
			if(($map_file != null) && !empty($map_file))
				$print_jscript = true;
		}
?>
<?php		if($print_jscript) { ?>
		function AddMapEntryAsync()
		{
			DisableNavigation();

			var request = new XMLHttpRequest();

			request.onreadystatechange=function()
			{
				if (request.readyState==4 && request.status==200)
				{
					document.getElementById("contents").innerHTML = request.responseText;

					EnableNavigation();
				}
			}

			request.open("POST", "map_database/map_entry_add.php", true);
			request.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
			request.send("map_file=" + <?php print("\"".$map_file."\""); ?>);
		}
		onload = AddMapEntryAsync;
<?php		} ?>