<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
	// handle the users account
	require_once('user_session.php');
	session_start();
	
	$user_session = null;
	$logged_in = false;	
	if(isset($_SESSION['UserSession']))
	{
		$user_session = $_SESSION['UserSession'];
		$logged_in = $user_session->IsLoggedIn();
	}
	if(!$logged_in) { header("Location:server_login.php"); die(); }
	
	$current_user = "";
	// get the users privileges
	$can_create_map_entries = false;
	$can_delete_map_entries = false;
	$can_edit_map_entries = false;
	$can_create_users = false;
	$can_delete_users = false;
	$can_edit_users = false;
	
	if($logged_in)
	{
		$current_user = $user_session->UserName();
		$can_create_map_entries = $user_session->HasMapDatabasePermission(AccessPermissions::ACCESS_PERMISSIONS_CREATE);
		$can_delete_map_entries = $user_session->HasMapDatabasePermission(AccessPermissions::ACCESS_PERMISSIONS_DELETE);
		$can_edit_map_entries = $user_session->HasMapDatabasePermission(AccessPermissions::ACCESS_PERMISSIONS_EDIT);
		$can_create_users = $user_session->HasUserControlPermission(AccessPermissions::ACCESS_PERMISSIONS_CREATE);
		$can_delete_users = $user_session->HasUserControlPermission(AccessPermissions::ACCESS_PERMISSIONS_DELETE);
		$can_edit_users = $user_session->HasUserControlPermission(AccessPermissions::ACCESS_PERMISSIONS_EDIT);
	}
?>