<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
require_once("PasswordHash.php");
require_once("config.php");
require_once("sql_database.php");
require_once("sql_interface.php");
	
final class AccessPermissions
{
	const ACCESS_PERMISSIONS_NONE = 0;
	const ACCESS_PERMISSIONS_CREATE = 1;
	const ACCESS_PERMISSIONS_DELETE = 2;
	const ACCESS_PERMISSIONS_EDIT = 4;
}

class User
{
	public $username;
	public $user_control_permissions;	
	public $map_database_permissions;
}

class UserSQLBase extends SQLInterface
{
	public $username;
	public $password_hash;
	
	public $user_control_permissions;	
	public $map_database_permissions;
	
	public function UserSQLBase($database, $sql, $type) { parent::__construct($database, $sql, $type); }	
}

class UserRead extends UserSQLBase
{
	public function UserRead($database, $sql) { parent::__construct($database, $sql, SQLInterfaceType::SQL_IO_TYPE_READ); }	
}

class UserWrite extends UserSQLBase
{	
	public function UserWrite($database, $sql) { parent::__construct($database, $sql, SQLInterfaceType::SQL_IO_TYPE_WRITE); }
}

class UserUpdate extends UserSQLBase
{	
	public function UserUpdate($database, $sql) { parent::__construct($database, $sql, SQLInterfaceType::SQL_IO_TYPE_UPDATE); }
}

class UserSession
{
	private $is_logged_in;
	private $logged_in_user;
	
	public function UserSession()
	{
		$this->is_logged_in = false;
		$this->logged_in_user = null;
	}
	
	function IsLoggedIn()
	{
		return $this->is_logged_in;
	}
	
	function UserName()
	{
		return $this->logged_in_user->username;
	}
	
	function HasUserControlPermission($access_type)
	{
		return ($this->logged_in_user->user_control_permissions & $access_type) == $access_type;
	}
	
	function HasMapDatabasePermission($access_type)
	{
		return ($this->logged_in_user->map_database_permissions & $access_type) == $access_type;
	}
	
	function LogIn($username, $password)
	{
		$config = LoadConfig();
		
		$database = OpenDatabase($config->map_database->data_source_name, $config->map_database->username_readonly, $config->map_database->password_readonly);
		$user_read = new UserRead($database, "SELECT {0} FROM `map_server_users` WHERE username = ?");
		$user_read->ExecuteQuery(array($username));
		if(!$user_read->MoveNext())
			return false;
		
		$password_hash = new PasswordHash(8, true);
		if($password_hash->CheckPassword($password, $user_read->password_hash))
		{
			$this->is_logged_in = true;
			
			$user = new User();
			$user->username = $user_read->username;
			$user->user_control_permissions = $user_read->user_control_permissions;
			$user->map_database_permissions = $user_read->map_database_permissions;
			
			$this->logged_in_user = $user;
		}
		else
		{
			$this->is_logged_in = false;
			$this->logged_in_user = null;
		}			

		$database = null;
		return $this->is_logged_in;
	}
}
?>
