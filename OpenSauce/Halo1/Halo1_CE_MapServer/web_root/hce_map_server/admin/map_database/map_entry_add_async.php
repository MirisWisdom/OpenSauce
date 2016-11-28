<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
?>
<?php			if(!$can_create_map_entries) { ?>
			Your user account does not have sufficient privileges to add map entries.<br/>
<?php			} else if($map_file == null) { ?>
			No map file name posted with request.<br/>
<?php			} else { ?>
			Adding map to database, please wait...<br/>
<?php			} ?>
