<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
?>
		<h2>Remove User</h2>
<?php
	class UserRemoveResult
	{
		public $success;
		public $error_message;
		
		public $user_name;
		public $removal_confirmed;
	}
	
	function ProcessUserRemove()
	{
		global $database;
		global $current_user;
		global $can_delete_users;
		
		$result = new UserRemoveResult();
		
		// only allow users with the required privileges to delete users
		if(!$can_delete_users)
		{
			$result->success = false;
			$result->error_message = "Your user account does not have sufficient priviledges to delete users.";
			return $result;
		}

		// verify the username has been set
		if(!isset($_POST["user_remove"]) || empty($_POST["user_remove"]))
		{
			$result->success = false;
			$result->error_message = "No username to remove provided.";
			return $result;
		}

		$result->user_name = $_POST["user_remove"];

		// prevent the currently signed in user from being removed
		if($result->user_name === $current_user)
		{
			$result->success = false;
			$result->error_message = "You cannot remove the currently logged in user.";
			return $result;
		}

		// if the user removal has been confirmed, remove the user from the database
		$result->removal_confirmed = isset($_POST["user_remove_confirm"]);	
		if($result->removal_confirmed)
		{
			// check that the user account exists in the database
			$user_read = new UserRead($database, "SELECT {0} FROM `map_server_users` WHERE username = ?");
			$user_read->ExecuteQuery(array($result->user_name));

			if(!$user_read->MoveNext())
			{
				$result->success = false;
				$result->error_message = "Matching username not found in the database.";
				return $result;
			}

			// delete the user account from the database
			$sql = "DELETE FROM map_server_users WHERE username = ?";
			$remove_user_query = $database->prepare($sql);
			if($remove_user_query->execute(array($result->user_name)))
				print_line_inset("User ".$result->user_name." removed.<br/>", 2);
			else
				print_line_inset("Failed to remove user ".$result->user_name.".<br/>", 2);
		}
		
		$result->success = true;
		return $result;
	}
	
	$user_remove_result = ProcessUserRemove();
	
	if(!$user_remove_result->success)
		print_line_inset($user_remove_result->error_message, 2);
?>		
<?php		if($user_remove_result->success && !$user_remove_result->removal_confirmed) { ?>
		<!-- The user removal hasn't been confirmed yet, so check with the current user -->
		Are you sure you want to remove <?php print($user_remove_result->user_name); ?> from the users list?<br/>
		<form name="user_remove_confirm_form" method="post" action="">
			<div>
				<input type="hidden" name="user_control"/>
				<input type="hidden" name="user_remove" value='<?php print($user_remove_result->user_name); ?>'/>
				<input type="hidden" name="user_remove_confirm"/>
				<input class='form_button_100px' type='submit' value='Confirm'/>
			</div>
		</form>
		<form name="user_remove_cancel_form" method="post" action="">
			<div>
				<input type="hidden" name="user_control"/>
				<input class='form_button_100px' type='submit' value='Cancel'/>
			</div>
		</form>
<?php		} ?>
