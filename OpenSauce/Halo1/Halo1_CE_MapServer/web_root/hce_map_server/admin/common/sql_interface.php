<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
	require_once("property_array_util.php");
	
	final class SQLInterfaceType
	{
		const SQL_IO_TYPE_READ = 0;
		const SQL_IO_TYPE_WRITE = 1;
		const SQL_IO_TYPE_UPDATE = 2;
	}
	
	class SQLInterface
	{
		private $query;
		private $query_type;
		
		function SQLInterface($database, $sql, $type) {  $this->query_type = $type; $this->InitializeQuery($database, $sql); }
				
		private function InitializeQuery($database, $sql)
		{
			$variable_list = "";
			$values_list = "";		
			
			$class_reflection = new ReflectionClass($this); 
			
			// iterate through this objects properties, building the list for the SQL query
			foreach($this as $key => $value)
			{
				// check the class contains the property
				if(!$class_reflection->hasProperty($key))
					continue;
				
				// skip the property if it is not public
				$property = $class_reflection->getProperty($key);
				if(!$property->isPublic())
					continue;
				
				// append commas to the variable and values list if necessary
				if(!empty($variable_list))
				{
					$variable_list = $variable_list.", ";
					
					if($this->query_type == SQLInterfaceType::SQL_IO_TYPE_WRITE)
						$values_list = $values_list.", ";
				}
				
				// add the property to the SQL variable list
				switch($this->query_type)
				{
					case SQLInterfaceType::SQL_IO_TYPE_READ:
					$variable_list = $variable_list.$key;
					break;
					case SQLInterfaceType::SQL_IO_TYPE_WRITE:
					$variable_list = $variable_list."`".$key."`";
					$values_list = $values_list."?";
					break;
					case SQLInterfaceType::SQL_IO_TYPE_UPDATE:
					$variable_list = $variable_list.$key." = ?";
					break;
				}
			}
			
			// replace {0} with the properties list
			$sql = str_replace("{0}", $variable_list, $sql);
			// replace {1} with the values list
			$sql = str_replace("{1}", $values_list, $sql);
			
			$this->query = $database->prepare($sql);
		}
		
		public function ExecuteQuery($conditions)
		{
			$array = $conditions;
			
			switch($this->query_type)
			{
				case SQLInterfaceType::SQL_IO_TYPE_READ:
				break;
				case SQLInterfaceType::SQL_IO_TYPE_WRITE:
				$array = PropertyArrayUtil::GetPropertyArray($this);		
				break;
				case SQLInterfaceType::SQL_IO_TYPE_UPDATE:
				$array = array_merge(PropertyArrayUtil::GetPropertyArray($this), $conditions);
				break;
			}
			
			$this->query->execute($array);
		}
		
		public function MoveNext()
		{
			if($this->query_type != SQLInterfaceType::SQL_IO_TYPE_READ)
				die("Attempting to MoveNext on an SQL query that is not for reading");
				
			// reset this objects properties
			PropertyArrayUtil::ResetVariables($this);
			
			// get the next query result
			$result = $this->query->fetch();
			
			if(!$result)
				return false;
				
			PropertyArrayUtil::SetProperties($this, $result);
			
			return true;
		}
	}
?>