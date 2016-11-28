<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
	function ZipStatusString( $status )
	{
		switch( (int) $status )
		{
			case ZipArchive::ER_OK           : return 'No error';
			case ZipArchive::ER_MULTIDISK    : return 'Multi-disk zip archives not supported';
			case ZipArchive::ER_RENAME       : return 'Renaming temporary file failed';
			case ZipArchive::ER_CLOSE        : return 'Closing zip archive failed';
			case ZipArchive::ER_SEEK         : return 'Seek error';
			case ZipArchive::ER_READ         : return 'Read error';
			case ZipArchive::ER_WRITE        : return 'Write error';
			case ZipArchive::ER_CRC          : return 'CRC error';
			case ZipArchive::ER_ZIPCLOSED    : return 'Containing zip archive was closed';
			case ZipArchive::ER_NOENT        : return 'No such file';
			case ZipArchive::ER_EXISTS       : return 'File already exists';
			case ZipArchive::ER_OPEN         : return 'Can\'t open file';
			case ZipArchive::ER_TMPOPEN      : return 'Failure to create temporary file';
			case ZipArchive::ER_ZLIB         : return 'Zlib error';
			case ZipArchive::ER_MEMORY       : return 'Malloc failure';
			case ZipArchive::ER_CHANGED      : return 'Entry has been changed';
			case ZipArchive::ER_COMPNOTSUPP  : return 'Compression method not supported';
			case ZipArchive::ER_EOF          : return 'Premature EOF';
			case ZipArchive::ER_INVAL        : return 'Invalid argument';
			case ZipArchive::ER_NOZIP        : return 'Not a zip archive';
			case ZipArchive::ER_INTERNAL     : return 'Internal error';
			case ZipArchive::ER_INCONS       : return 'Zip archive inconsistent';
			case ZipArchive::ER_REMOVE       : return 'Can\'t remove file';
			case ZipArchive::ER_DELETED      : return 'Entry has been deleted';

			default: return "Unknown zip status".$status;
		}
	}
	
	function CompressFiles(array $input_files, $output_file, $compression_type)
	{
		if((count($input_files) % 2) > 0)
			die("ERROR: Invalid entry count in compressed file list</br>");

		$success = true;
		switch($compression_type)
		{
			case 0:
			{
				// check that zip compression is available
				if(!class_exists('ZipArchive'))
				{
					print("ERROR: Zip compression is not available.</br>");
					return false;
				}

				// create a new archive
				$zip = new ZipArchive();
				$result = $zip->open($output_file, ZipArchive::CREATE);
				if ($result !== TRUE)
				{
					print("ERROR: ".ZipStatusString($result)."</br>");
					return false;
				}

				// add the files to the archive
				for($i = 0; $success && ($i < count($input_files)); $i += 2)
				{
					if(!file_exists($input_files[$i]))
					{
						print("ERROR: A file to compress does not exist.</br>");
						$success = false;
					}
					else
					{
						$success &= $zip->addFile($input_files[$i], $input_files[$i + 1]);
					}
				}
				if(!$success)
					print("ERROR: Failed to add all files to archive.</br>");
				
				// close the archive
				$result = $zip->close();
				if ($result !== TRUE)
				{
					print("ERROR: ".ZipStatusString($result)."</br>");
					return false;
				}
			}
			break;
			default:
			{
				print("ERROR: Invalid compression type</br>");
				return false;
			}
		}
		return $success;
	}
?>