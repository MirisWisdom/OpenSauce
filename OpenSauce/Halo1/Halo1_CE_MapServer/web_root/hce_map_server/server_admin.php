<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
	// required to force a refresh of the tables when reloaded
	header("Cache-Control: no-store, no-cache, must-revalidate");
	
	set_include_path("*admin include path*");
	
	require_once("admin/common/config.php");	
	require_once("admin/common/print.php");
	require_once("admin/common/user_account.php");
	require_once("admin/common/sql_database.php");
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html style="background-color: #35363D" xmlns="http://www.w3.org/1999/xhtml">
<head>
	<link rel="stylesheet" href="css/mainstyle.css" type="text/css"/>
	<title>OpenSauce Halo CE Map Server</title>
	<script type="text/javascript">
		function DisableNavigation()
		{
			nav_logout.button.disabled = true;
			nav_map_database.button.disabled = true;
<?php		if($can_create_users || $can_delete_users || $can_edit_users) { ?>
			nav_user_control.button.disabled = true;
<?php		} ?>
			nav_my_account.button.disabled = true;
		}
		
		function EnableNavigation()
		{
			nav_logout.button.disabled = false;
			nav_map_database.button.disabled = false;
<?php		if($can_create_users || $can_delete_users || $can_edit_users) { ?>
			nav_user_control.button.disabled = false;
<?php		} ?>
			nav_my_account.button.disabled = false;
		}
		
<?php		
		if(isset($_POST['map_database']))
		{
			if(isset($_POST['map_entry_add']))
			{
				include_once("admin/map_database/map_entry_add_js.php");
			}
		}
?>
	</script>
</head>

<body>
	<div id='title'>
		<div id="main_title">
			<h1>OpenSauce Halo CE Map Server</h1>
		</div>
		<div id="username">
			<h2>Logged in as <?php print($user_session->Username()); ?></h2>
			<form name="nav_logout" method='post' action='server_login.php'>
				<div>
					<input type="hidden" name="logout"/>
					<input name="button" class='form_button_100px' type='submit' value='Log Out'/>
				</div>
			</form>
		</div>
	</div>

	<div id='navigation'>
		<form name="nav_map_database" method="post" action="">
			<div>
				<input type="hidden" name="map_database"/>
				<input class="form_button" name="button" type="submit" value="Map Database"/>
			</div>
		</form>
<?php		if($can_create_users || $can_delete_users || $can_edit_users) { ?>
		<form name="nav_user_control" method="post" action="">
			<div>
				<input type="hidden" name="user_control"/>
				<input class="form_button" name="button" type="submit" value="User Control"/>
			</div>
		</form>
<?php		} ?>
		<form name="nav_my_account" method="post" action="">
			<div>
				<input type="hidden" name="my_account"/>
				<input class="form_button" name="button" type="submit" value="My Account"/>
			</div>
		</form>
	</div>
	
	<div id='contents'>
<?php
			if(isset($_POST['map_database']))
			{
				if(isset($_POST['map_entry_add']))
				{
					include_once("admin/map_database/map_entry_add_async.php");
				}
				else if(isset($_POST['map_entry_remove']))
				{
					include_once("admin/map_database/map_entry_remove.php");					
				}
				else if(isset($_POST['map_file_delete']))
				{
					include_once("admin/map_database/map_file_delete.php");
				}
				else
					include_once("admin/map_database_page.php");
			}
			else if(isset($_POST['user_control']))
			{
				if(isset($_POST['user_add']))
					include_once("admin/user_control/user_add.php");
				else if(isset($_POST['user_edit']))
					include_once("admin/user_control/user_edit.php");
				else if(isset($_POST['user_remove']))
					include_once("admin/user_control/user_remove.php");
				else
					include_once("admin/user_control_page.php");
			}
			else if(isset($_POST['my_account']))
			{
				include_once("admin/my_account_page.php");
			}
			else
			{
				include_once("admin/map_database_page.php");
			}
?>
	</div>
</body>
</html>
<?php
	$database = NULL;
?>