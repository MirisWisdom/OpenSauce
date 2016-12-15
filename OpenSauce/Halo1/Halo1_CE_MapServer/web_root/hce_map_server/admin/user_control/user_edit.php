<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
?>
		<h2>Edit User</h2>
<?php
	class UserEditResult
	{
		public $success;
		public $error_message;
		
		public $user_name;
		public $user_can_create_map_entry;
		public $user_can_delete_map_entry;
		public $user_can_edit_map_entry;
		public $user_can_create_users;
		public $user_can_delete_users;
		public $user_can_edit_users;		
	}
	
	function ProcessUserEdit()
	{
		global $database;
		global $current_user;
		global $can_edit_users;
		
		$result = new UserEditResult();
		
		// only allow users with the required privileges to delete users
		if(!$can_edit_users)
		{
			$result->success = false;
			$result->error_message = "Your user account does not have sufficient priviledges to edit users.";
			return $result;
		}

		// verify the username has been set
		if(!isset($_POST["user_edit"]) || empty($_POST["user_edit"]))
		{
			$result->success = false;
			$result->error_message = "No username to edit provided.";
			return $result;
		}

		$result->user_name = $_POST["user_edit"];

		// prevent the currently signed in user from being edited
		if($result->user_name === $current_user)
		{
			$result->success = false;
			$result->error_message = "You cannot edit the currently logged in user.";
			return $result;
		}

		$user_read = new UserRead($database, "SELECT {0} FROM `map_server_users` WHERE username = ?");
		$user_read->ExecuteQuery(array($result->user_name));

		if(!$user_read->MoveNext())
		{
			$result->success = false;
			$result->error_message = "Unable to find user in database.";
			return $result;
		}

		$result->user_can_create_map_entry = ($user_read->map_database_permissions & AccessPermissions::ACCESS_PERMISSIONS_CREATE) == AccessPermissions::ACCESS_PERMISSIONS_CREATE;
		$result->user_can_delete_map_entry = ($user_read->map_database_permissions & AccessPermissions::ACCESS_PERMISSIONS_DELETE) == AccessPermissions::ACCESS_PERMISSIONS_DELETE;
		$result->user_can_edit_map_entry = ($user_read->map_database_permissions & AccessPermissions::ACCESS_PERMISSIONS_EDIT) == AccessPermissions::ACCESS_PERMISSIONS_EDIT;
		$result->user_can_create_users = ($user_read->user_control_permissions & AccessPermissions::ACCESS_PERMISSIONS_CREATE) == AccessPermissions::ACCESS_PERMISSIONS_CREATE;
		$result->user_can_delete_users = ($user_read->user_control_permissions & AccessPermissions::ACCESS_PERMISSIONS_DELETE) == AccessPermissions::ACCESS_PERMISSIONS_DELETE;
		$result->user_can_edit_users = ($user_read->user_control_permissions & AccessPermissions::ACCESS_PERMISSIONS_EDIT) == AccessPermissions::ACCESS_PERMISSIONS_EDIT;

		if(isset($_POST['user_edit_save']))
		{
			$result->user_can_create_map_entry = isset($_POST['user_edit_can_create_map_entry']);
			$result->user_can_delete_map_entry = isset($_POST['user_edit_can_delete_map_entry']);
			$result->user_can_edit_map_entry = isset($_POST['user_edit_can_edit_map_entry']);
			$result->user_can_create_users = isset($_POST['user_edit_can_create_users']);
			$result->user_can_delete_users = isset($_POST['user_edit_can_delete_users']);
			$result->user_can_edit_users = isset($_POST['user_edit_can_edit_users']);

			$user_update = new UserUpdate($database, "UPDATE `map_server_users` SET {0} WHERE username = ?");
			$user_update->username = $user_read->username;
			$user_update->password_hash = $user_read->password_hash;
			$user_update->map_database_permissions = AccessPermissions::ACCESS_PERMISSIONS_NONE;
			$user_update->user_control_permissions = AccessPermissions::ACCESS_PERMISSIONS_NONE;

			if($result->user_can_create_map_entry) $user_update->map_database_permissions |= AccessPermissions::ACCESS_PERMISSIONS_CREATE;
			if($result->user_can_delete_map_entry) $user_update->map_database_permissions |= AccessPermissions::ACCESS_PERMISSIONS_DELETE;
			if($result->user_can_edit_map_entry) $user_update->map_database_permissions |= AccessPermissions::ACCESS_PERMISSIONS_EDIT;
			if($result->user_can_create_users) $user_update->user_control_permissions |= AccessPermissions::ACCESS_PERMISSIONS_CREATE;
			if($result->user_can_delete_users) $user_update->user_control_permissions |= AccessPermissions::ACCESS_PERMISSIONS_DELETE;
			if($result->user_can_edit_users) $user_update->user_control_permissions |= AccessPermissions::ACCESS_PERMISSIONS_EDIT;

			$user_update->ExecuteQuery(array($user_read->username));

			print_line_inset("<h3>Output</h3>", 2);
			print_line_inset("Changes saved.<br/><br/>", 2);
		}
		$result->success = true;
		return $result;
	}
	$user_edit_result = ProcessUserEdit();
	
	if(!$user_edit_result->success)
		print_line_inset($user_edit_result->error_message, 2);
?>
<?php		if($user_edit_result->success) { ?>
		Editing <?php print($user_edit_result->user_name); ?><br/>
		<form name="user_add_form" method="post" action="">
			<div>
				<input type='hidden' name='user_control'/>
				<input type='hidden' name='user_edit' value="<?php print($user_edit_result->user_name); ?>"/>
				Map Database Permissions<br/>
				<input type="checkbox" name="user_edit_can_create_map_entry" <?php print($user_edit_result->user_can_create_map_entry ? "checked=\"checked\"" : "");?>/> Create<br/>
				<input type="checkbox" name="user_edit_can_delete_map_entry" <?php print($user_edit_result->user_can_delete_map_entry ? "checked=\"checked\"" : "");?>/> Delete<br/>
				<input type="checkbox" name="user_edit_can_edit_map_entry" <?php print($user_edit_result->user_can_edit_map_entry ? "checked=\"checked\"" : "");?>/> Edit<br/>
				User Control Permissions<br/>
				<input type="checkbox" name="user_edit_can_create_users" <?php print($user_edit_result->user_can_create_users ? "checked=\"checked\"" : "");?>/> Create<br/>
				<input type="checkbox" name="user_edit_can_delete_users" <?php print($user_edit_result->user_can_delete_users ? "checked=\"checked\"" : "");?>/> Delete<br/>
				<input type="checkbox" name="user_edit_can_edit_users" <?php print($user_edit_result->user_can_edit_users ? "checked=\"checked\"" : "");?>/> Edit<br/>
				<input type='hidden' name='user_edit_save'/>
				<input class="form_button_100px" type="submit" value="Save Changes"/>
			</div>
		</form>
<?php		} ?>
