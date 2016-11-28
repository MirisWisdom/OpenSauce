<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
?>
		<h2>Users</h2>
		<table>
			<tr>
				<th>Username</th>
				<th>Map Database Permissions</th>
				<th>User Control Permissions</th>
<?php 				if($can_edit_users) {?>
				<th></th>
<?php				} ?>
<?php				if($can_delete_users) { ?>
				<th></th>
<?php				} ?>
			</tr>
<?php
				// iterate through all users in the database
				$user_reader = new UserRead($database, "SELECT {0} FROM `map_server_users`");
				$user_reader->ExecuteQuery(NULL);

				while($user_reader->MoveNext())
				{
					// get the users details
					$username = $user_reader->username;

					$map_database_permissions = "";
					if($user_reader->map_database_permissions == AccessPermissions::ACCESS_PERMISSIONS_NONE)
						$map_database_permissions = "None";
					else
					{
						if(AccessPermissions::ACCESS_PERMISSIONS_CREATE == ($user_reader->map_database_permissions & AccessPermissions::ACCESS_PERMISSIONS_CREATE))
							$map_database_permissions .= "Create ";
						if(AccessPermissions::ACCESS_PERMISSIONS_DELETE == ($user_reader->map_database_permissions & AccessPermissions::ACCESS_PERMISSIONS_DELETE))
							$map_database_permissions .= "Delete ";
						if(AccessPermissions::ACCESS_PERMISSIONS_EDIT == ($user_reader->map_database_permissions & AccessPermissions::ACCESS_PERMISSIONS_EDIT))
							$map_database_permissions .= "Edit";
					}

					$user_control_permissions = "";
					if($user_reader->user_control_permissions == AccessPermissions::ACCESS_PERMISSIONS_NONE)
						$user_control_permissions = "None";
					else
					{
						if(AccessPermissions::ACCESS_PERMISSIONS_CREATE == ($user_reader->user_control_permissions & AccessPermissions::ACCESS_PERMISSIONS_CREATE))
							$user_control_permissions .= "Create ";
						if(AccessPermissions::ACCESS_PERMISSIONS_DELETE == ($user_reader->user_control_permissions & AccessPermissions::ACCESS_PERMISSIONS_DELETE))
							$user_control_permissions .= "Delete ";
						if(AccessPermissions::ACCESS_PERMISSIONS_EDIT == ($user_reader->user_control_permissions & AccessPermissions::ACCESS_PERMISSIONS_EDIT))
							$user_control_permissions .= "Edit";
					}

					// print the users details in a table
					print_line_inset("<tr>", 4);
						print_line_inset("<td>".$username."</td>", 5);
						print_line_inset("<td>".$map_database_permissions."</td>", 5);
						print_line_inset("<td>".$user_control_permissions."</td>", 5);
						print_line_inset("<td>", 5);
						// only display the edit form if the current users permissions allow it
						if($can_edit_users)
						{
							print_line_inset("<form name='edit_user_form' method='post' action=''>", 6);
								print_line_inset("<div>", 7);
									print_line_inset("<input type='hidden' name='user_control'/>", 8);
									print_line_inset("<input type='hidden' name='user_edit' value='".$username."'/>", 8);
									print_line_inset("<input class='form_button_100px' type='submit' value='Edit User'/>", 8);
								print_line_inset("</div>", 7);
							print_line_inset("</form>", 6);
						}
						print_line_inset("</td>", 5);
						print_line_inset("<td>", 5);
						// only display the delete form if the current users permissions allow it
						if($can_delete_users)
						{
							print_line_inset("<form name='delete_user_form' method='post' action=''>", 6);
								print_line_inset("<div>", 7);
									print_line_inset("<input type='hidden' name='user_control'/>", 8);
									print_line_inset("<input type='hidden' name='user_remove' value='".$username."'/>", 8);
									print_line_inset("<input class='form_button_100px' type='submit' value='Remove User'/>", 8);
								print_line_inset("</div>", 7);
							print_line_inset("</form>", 6);
						}
						print_line_inset("</td>", 5);
					print_line_inset("</tr>", 4);
				}
?>
		</table>
			<!-- only display the add user form if the current users permissions allow it -->
<?php		if($can_create_users) { ?>
		<form name='add_user_form' method='post' action=''>
			<div>
				<input type='hidden' name='user_control'/>
				<input type='hidden' name='user_add'/>
				<input class='form_button_100px' type='submit' value='Add User'/>
			</div>
		</form>
<?php		} ?>

