<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
	function TransferFile($filename)
	{
		// check the file exists
		if(!file_exists($filename))
		{
			header("HTTP/1.0 404 Not Found");
			return 0;
		}
		
		// open the file
		$file_handle = fopen($filename, "rb");
		if(!$file_handle)
		{
			header("HTTP/1.0 500 Internal Server Error");
			return 0;
		}
		
		// lock the file for shared reading
		if(!flock($file_handle, LOCK_SH))
		{
			header("HTTP/1.0 500 Internal Server Error");
			return 0;
		}
		
		$file_size = filesize($filename);
		
		// set the files headers
		header('Content-Description: File Transfer');
		header('Content-disposition: attachment; filename='.basename($filename));
		header('Content-Type: application/octet-stream');
		header('Content-Transfer-Encoding: binary');
		header('Expires: 0');
		header('Cache-Control: must-revalidate');
		header('Content-Length: '.$file_size);
		
		$data_sent = 0;
		while($chunk = fread($file_handle, 1024))
		{
			echo $chunk;
			flush();
			
			$data_sent += 1024;
			
			if(connection_aborted())
				break;
		}
		flock($file_handle, LOCK_UN);
		fclose($file_handle);
		
		if($data_sent > $file_size)
			$data_sent = $file_size;
		
		// return data sent
		return $data_sent;
	}
?>
