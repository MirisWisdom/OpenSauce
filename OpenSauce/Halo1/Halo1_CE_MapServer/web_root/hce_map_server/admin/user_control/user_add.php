<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
?>
		<h2>Add User</h2>
<?php
	class UserAddResult
	{
		public $success;
		public $error_message;
		
		public $user_name;
		public $user_password;
		public $user_can_create_map_entry;
		public $user_can_delete_map_entry;
		public $user_can_edit_map_entry;
		public $user_can_create_users;
		public $user_can_delete_users;
		public $user_can_edit_users;	
	}
	
	function ProcessUserAdd()
	{
		global $database;
		global $can_create_users;
		
		$result = new UserAddResult();
		
		if(!$can_create_users)
		{
			$result->success = false;
			$result->error_message = "Your user account does not have sufficient priviledges to add new users.";
			return $result;
		}

		$result->user_name = "";
		$result->user_can_create_map_entry = false;
		$result->user_can_delete_map_entry = false;
		$result->user_can_edit_map_entry = false;
		$result->user_can_create_users = false;
		$result->user_can_delete_users = false;
		$result->user_can_edit_users = false;

		if(isset($_POST['user_add_username']))
		{
			$result->user_name = $_POST['user_add_username'];
			$result->user_can_create_map_entry = isset($_POST['user_add_can_create_map_entry']);
			$result->user_can_delete_map_entry = isset($_POST['user_add_can_delete_map_entry']);
			$result->user_can_edit_map_entry = isset($_POST['user_add_can_edit_map_entry']);
			$result->user_can_create_users = isset($_POST['user_add_can_create_users']);
			$result->user_can_delete_users = isset($_POST['user_add_can_delete_users']);
			$result->user_can_edit_users = isset($_POST['user_add_can_edit_users']);

			print_line_inset("<h3>Output</h3>", 2);

			if(empty($result->user_name))
			{
				print_line_inset("No username set.<br/><br/>", 2);
			}
			else
			{
				$user_read = new UserRead($database, "SELECT {0} FROM `map_server_users` WHERE username = ?");
				$user_read->ExecuteQuery(array($result->user_name));

				if($user_read->MoveNext())
				{
					print_line_inset("An account with that username already exists.<br/><br/>", 2);
				}
				else
				{
					// generate a random password for the new user account
					$result->user_password = substr(str_shuffle("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"), 0, 8);
					$user_write = new UserWrite($database, "INSERT INTO `map_server_users` ({0}) VALUES ({1})");
					$password_hash = new PasswordHash(8, true);

					$user_write->username = $result->user_name;
					$user_write->password_hash = $password_hash->HashPassword($result->user_password);
					if($result->user_can_create_map_entry) $user_write->map_database_permissions |= AccessPermissions::ACCESS_PERMISSIONS_CREATE;
					if($result->user_can_delete_map_entry) $user_write->map_database_permissions |= AccessPermissions::ACCESS_PERMISSIONS_DELETE;
					if($result->user_can_edit_map_entry) $user_write->map_database_permissions |= AccessPermissions::ACCESS_PERMISSIONS_EDIT;
					if($result->user_can_create_users) $user_write->user_control_permissions |= AccessPermissions::ACCESS_PERMISSIONS_CREATE;
					if($result->user_can_delete_users) $user_write->user_control_permissions |= AccessPermissions::ACCESS_PERMISSIONS_DELETE;
					if($result->user_can_edit_users) $user_write->user_control_permissions |= AccessPermissions::ACCESS_PERMISSIONS_EDIT;

					$user_write->ExecuteQuery(NULL);
					print_line_inset("New user added!<br/>", 2);
					print_line_inset("Username: ".$result->user_name."<br/>", 2);
					print_line_inset("Password: ".$result->user_password."<br/>", 2);
					print_line_inset("<br/>", 2);
					print_line_inset("This password is randomly generated and should be changed by the user.<br/>", 2);
					print_line_inset("<br/>", 2);
				}
			}
		}
		$result->success = true;
		return $result;
	}
	
	$user_add_result = ProcessUserAdd();
	
	if(!$user_add_result->success)
		print_line_inset($user_add_result->error_message, 2);
?>
<?php		if($user_add_result->success) { ?>
		<form name="user_add_form" method="post" action="">
			<div>
				<input type='hidden' name='user_control'/>
				<input type='hidden' name='user_add'/>
				Username: <input class="form_text" type="text" name="user_add_username" value=<?php print("\"".$user_add_result->user_name."\"");?>/><br/>
				Map Database Permissions<br/>
				<input type="checkbox" name="user_add_can_create_map_entry" <?php print($user_add_result->user_can_create_map_entry ? "checked=\"checked\"" : "");?>/> Create<br/>
				<input type="checkbox" name="user_add_can_delete_map_entry" <?php print($user_add_result->user_can_delete_map_entry ? "checked=\"checked\"" : "");?>/> Delete<br/>
				<input type="checkbox" name="user_add_can_edit_map_entry" <?php print($user_add_result->user_can_edit_map_entry ? "checked=\"checked\"" : "");?>/> Edit<br/>
				User Control Permissions<br/>
				<input type="checkbox" name="user_add_can_create_users" <?php print($user_add_result->user_can_create_users ? "checked=\"checked\"" : "");?>/> Create<br/>
				<input type="checkbox" name="user_add_can_delete_users" <?php print($user_add_result->user_can_delete_users ? "checked=\"checked\"" : "");?>/> Delete<br/>
				<input type="checkbox" name="user_add_can_edit_users" <?php print($user_add_result->user_can_edit_users ? "checked=\"checked\"" : "");?>/>Edit<br/>
				<input class="form_button_100px" type="submit" value="Add User"/>
			</div>
		</form>
<?php } ?>