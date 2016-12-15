<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
?>
		<div id='map_entry_admin'>
			<h2>Map Files</h2>
			<table>
				<tr>
					<th>Map File</th>
					<th>In Database</th>
					<th>Added Successfully</th>
<?php					if($can_create_map_entries) { ?>
					<th></th>
<?php					} ?>
<?php					if($can_delete_map_entries) { ?>
					<th></th>
<?php					} ?>
				</tr>
<?php
					require_once('admin/map_database/map_file_list.php');
					// loop through all maps in the map directory
					$map_file_list = GetMapFileList($config->map_server->map_dir, $database);
					foreach($map_file_list as $value)
					{
						// print the table entries
						print_line_inset("<tr>", 4);
							print_line_inset("<td>".$value->map_name."</td>", 5);
							print_line_inset("<td>".$value->map_in_database."</td>", 5);
							print_line_inset("<td>".$value->map_processed."</td>", 5);
							// only display the add entry form if the users has permissions for it
							if($can_create_map_entries)
							{
							print_line_inset("<td>", 5);
								print_line_inset("<form name='map_addition_form' method='post' action=''>", 6);
									print_line_inset("<div>", 7);
										print_line_inset("<input type='hidden' name='map_database'/>", 8);
										print_line_inset("<input type='hidden' name='map_entry_add' value='".$value->map_file_name."'/>", 8);
										print_line_inset("<input class='form_button_100px' type='submit' value='Add Entry'/>", 8);
									print_line_inset("</div>", 7);
								print_line_inset("</form>", 6);
							print_line_inset("</td>", 5);
							}
							// only display the delete file form if the users has permissions for it
							if($can_delete_map_entries)
							{
							print_line_inset("<td>", 5);
								print_line_inset("<form name='map_deletion_form' method='post' action=''>", 6);
									print_line_inset("<div>", 7);
										print_line_inset("<input type='hidden' name='map_database'/>", 8);
										print_line_inset("<input type='hidden' name='map_file_delete' value='".$value->map_file_name."'/>", 8);
										print_line_inset("<input class='form_button_100px' type='submit' value='Delete File(s)'/>", 8);
									print_line_inset("</div>", 7);
								print_line_inset("</form>", 6);
							print_line_inset("</td>", 5);
							}
						print_line_inset("</tr>", 4);
					}
				?>
			</table>
		</div>

		<div id='map_database'>
			<h2>Map Database</h2>
			<table>
				<tr>
					<th>Name</th>
					<th>Extension</th>
					<th>Added Successfully</th>
<?php					if($can_delete_map_entries) { ?>
					<th></th>
<?php					} ?>
				</tr>

<?php
					require_once('admin/map_database/map_entry_list.php');
					$map_entry_list = GetMapEntryList($database);					
					foreach($map_entry_list as $value)
					{
						print_line_inset("<tr>", 4);
							print_line_inset("<td>".$value->map_name."</td>", 5);
							print_line_inset("<td>".$value->map_extension."</td>", 5);
							print_line_inset("<td>".$value->map_processed."</td>", 5);
							// only preint the delete entry form if the users permissions allow it
							if($can_delete_map_entries)
							{
							print_line_inset("<td>", 5);
								print_line_inset("<form name='map_addition_form' method='post' action=''>", 6);
									print_line_inset("<div>", 7);
										print_line_inset("<input type='hidden' name='map_database'/>", 8);
										print_line_inset("<input type='hidden' name='map_entry_remove' value='".$value->map_file_id."'/>", 8);
										print_line_inset("<input class='form_button_100px' type='submit' value='Remove Entry'/>", 8);
									print_line_inset("</div>", 7);
								print_line_inset("</form>", 6);
							print_line_inset("</td>", 5);
							}
						print_line_inset("</tr>", 4);
					}
?>
			</table>
		</div>
