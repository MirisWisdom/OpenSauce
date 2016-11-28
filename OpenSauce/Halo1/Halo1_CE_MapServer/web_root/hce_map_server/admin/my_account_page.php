<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
?>
		<h2>Change Password</h2>
<?php
	function ProcessPasswordChange()
	{
		global $database;
		global $current_user;
		
		if(isset($_POST["current_password"]) || isset($_POST["new_password"]) || isset($_POST["repeat_password"]))
		{
			// get the password variables
			$current_password = null;
			if(!isset($_POST["current_password"]) || empty($_POST["current_password"]))
				return "Current password not set.<br/>";
			$current_password = $_POST["current_password"];

			$new_password = null;
			if(!isset($_POST["new_password"]) || empty($_POST["new_password"]))
				return "New password not set.<br/>";
			$new_password = $_POST["new_password"];

			$repeat_password = null;
			if(!isset($_POST["repeat_password"]) || empty($_POST["repeat_password"]))
				return "Repeated password not set.<br/>";
			$repeat_password = $_POST["repeat_password"];

			// check that the new password was entered correctly
			if($new_password !== $repeat_password)
				return "New password does not match the repeated password.<br/>";

			// get the current users database entry
			$user_read = new UserRead($database, "SELECT {0} FROM `map_server_users` WHERE username = ?");
			$user_read->ExecuteQuery(array($current_user));
			if(!$user_read->MoveNext())
				return "Failed to get current user account from the database.<br/>";

			// check that the provided password matches the current users password
			$password_hash = new PasswordHash(8, true);
			if(!$password_hash->CheckPassword($current_password, $user_read->password_hash))
				return "Incorrect password provided.<br/>";

			$user_update = new UserUpdate($database, "UPDATE `map_server_users` SET {0} WHERE username = ?");
			$user_update->username = $user_read->username;
			$user_update->password_hash = $password_hash->HashPassword($new_password);
			$user_update->map_database_permissions = $user_read->map_database_permissions;
			$user_update->user_control_permissions = $user_read->user_control_permissions;
			$user_update->ExecuteQuery(array($user_read->username));

			return "Changes saved.<br/>";
		}
		return null;
	}
	
	$result = ProcessPasswordChange();
?>
<?php		if($result != null) { ?>
		<h3>Output</h3>
		<?php print($result); ?><br/>
<?php		} ?>
		<form name="password_change_form" method="post" action="">
			<div>
				<input type="hidden" name="my_account"/>
				Current Password:<br/>
				<input class="form_text" type="password" name="current_password"/><br/>
				<br/>
				New Password:<br/>
				<input class="form_text" type="password" name="new_password"/><br/>
				Repeat Password:<br/>
				<input class="form_text" type="password" name="repeat_password"/><br/>
				<br/>
				<input class="form_button_100px" type="submit" value="Save Changes"/>
			</div>
		</form>
