<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
	// required to force a refresh of the tables when reloaded
	header("Cache-Control: no-store, no-cache, must-revalidate");
	
	set_include_path("*admin include path*");
	
	require_once("admin/common/config.php");
	
	// handle the users account
	include_once("admin/common/user_session.php");
	session_start();

	// if the intention is to logout, destroy the current session and start a new one
	if(isset($_POST["logout"]))
	{
		session_destroy();
		session_start();
	}

	$user_session = null;
	$logged_in = false;
	if(isset($_SESSION['UserSession']))
	{
		// if the session is already in-progress see whether a user is logged in
		$user_session = $_SESSION['UserSession'];
		$logged_in = $user_session->IsLoggedIn();
	}
	else
	{
		// if there is no user session and a user and password are provided, attempt to log in
		if(isset($_POST["username"]) && isset($_POST["password"]))
		{
			$user_session = new UserSession();

			if($user_session->LogIn($_POST["username"], $_POST["password"]))
			{
				$logged_in = $user_session->IsLoggedIn();
				$_SESSION['UserSession'] = $user_session;
			}
		}
	}
	
	if($logged_in)
	{
		header("Location:server_admin.php");
		die();
	}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html style="background-color: #35363D" xmlns="http://www.w3.org/1999/xhtml">
<head>
	<link rel="stylesheet" href="css/mainstyle.css" type="text/css"/>
	<title>OpenSauce Halo CE Map Server</title>
</head>

<body>
	<div id='title'>
		<div id='main_title'>
			<h1>OpenSauce Halo CE Map Server</h1>
		</div>
	</div>

	<div id='contents_login'>
<?php		if(!$logged_in) { ?>
		<!-- If we are not logged in, display the log in form -->
		<h1>Please log in</h1><br/>
		<form method='post' action=''>
			<div>
				<h3>Username: </h3><input class='form_text' type='text' name='username'/><br/>
				<h3>Password: </h3><input class='form_text' type='password' name='password'/><br/>
				<br/>
				<input class='form_button_100px' type='submit' value='Log in'/>
			</div>
		</form>
<?php		} ?>
	</div>
</body>
</html>