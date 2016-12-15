<?php

	class INIBlockIO
	{
		private $block_name;
		private $class_reflection;
		
		public function INIBlockIO($name) { $this->block_name = $name; $this->class_reflection = new ReflectionClass($this); }
		
		public function ReadBlock($array)
		{
			foreach($this as $key => &$value)
			{
				// load each property from the object
				if(!$this->class_reflection->hasProperty($key))
					continue;
					
				$property = $this->class_reflection->getProperty($key);
				if(!$property->isPublic())
					continue;
				
				// if the property is an object find out if its base is INIBlockIO
				if(is_object($value))
				{
					$reflection = new ReflectionClass($value);
					do
					{
						if(strcmp($reflection->name, "INIBlockIO") == 0)
							break;
					}while($reflection = $reflection->getParentClass());
					
					if(!$reflection)
						continue;
					
					// read the block from the ini array
					$value->ReadBlock($array[$key]);
				}
				else
				{
					$value = $array[$key];
				}
			}
		}
		
		function WriteBlock($file_handle)
		{
			// write the block name if applicable
			if(isset($this->block_name) && !empty($this->block_name))
				fwrite($file_handle, "[".$this->block_name."]\r\n");
			
			foreach($this as $key => &$value)
			{
				// load each property from the object
				if(!$this->class_reflection->hasProperty($key))
					continue;
					
				$property = $this->class_reflection->getProperty($key);
				if(!$property->isPublic())
					continue;
				
				// if the property is an object find out if its base is INIBlockIO
				if(is_object($value))
				{
					$reflection = new ReflectionClass($value);
					do
					{
						if(strcmp($reflection->name, "INIBlockIO") == 0)
							break;
					}while($reflection = $reflection->getParentClass());
					
					if(!$reflection)
						continue;
					
					// write the block to the file
					$value->WriteBlock($file_handle);
				}
				else
				{
					fwrite($file_handle, $key."=\"".$value."\"\r\n");
				}
			}
		}
	};
?>
