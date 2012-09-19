<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
	function CompressFiles(array $input_files, $output_file, $compression_type)
	{
		if((count($input_files) % 2) > 0)
			die("ERROR: Invalid entry could in compressed file list</br>");

		switch($compression_type)
		{
			case 0:
			{
				// TODO: more error checks
				$zip = new ZipArchive;
				$result = $zip->open($output_file, ZipArchive::CREATE);
				if ($result === FALSE)	
					return false;

				// add the files to the archive
				for($i = 0; $i < count($input_files); $i += 2)
					$zip->addFile($input_files[$i], $input_files[$i + 1]);
				$zip->close();
			}
			break;
			default:
				die("ERROR: Invalid compression type</br>");
		}
		return true;
	}
?>
