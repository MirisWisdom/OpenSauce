<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
	function SplitFile($input_file, $output_path, $part_size)
	{
		// open the file for reading
		if(!$handle = fopen($input_file, "rb"))
			return null;

		// get the filename without it's directory
		$base_filename = basename($input_file);

		// calculate the number of parts
		$archive_size = filesize($input_file);
		$part_count =  (int)($archive_size / $part_size);
		if(($archive_size % $part_size) > 0)
			$part_count++;

		$part_list = array();
		// create the map parts
		$offset = 0;
		for($i = 0; $i < $part_count; $i++)
		{
			if(feof($handle))
			{
				fclose($handle);
				die("ERROR: Reached the end of the archive before writing all of the parts</br>");
			}

			$part_name = $output_path.'/'.$base_filename.'.'.str_pad($i + 1, 3, "0", STR_PAD_LEFT);

			$write_handle = fopen($part_name, "w");
			// open the output part file
			if(FALSE == $write_handle)
				return null;

			$copy_count = min($archive_size - $offset, $part_size);

			$content = fread($handle, $copy_count);
			if(FALSE == $content)
			{
				// close and delete empty part file
				fclose($write_handle);
				unlink($part_name);

				return null;
			}

			if(FALSE == (fwrite($write_handle, $content)))
			{
				// close and delete empty part file
				fclose($write_handle);
				unlink($part_name);

				return null;
			}

			$offset += $copy_count;

			// close the part file
			fclose($write_handle);

			$part_list[] = $part_name;
		}

		// close the archive
		fclose($handle);

		return $part_list;
	}
?>
