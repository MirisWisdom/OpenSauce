<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
	class PropertyArrayUtil
	{
		public static function ResetVariables($object)
		{
			foreach($object as $key => &$value)
				$value = NULL;
		}
		
		public static function GetPropertyArray($object)
		{		
			$array = array();
			
			$index = 0;
			foreach($object as $key => $value)
			{
				$array[$index] = $value;
				$index++;
			}
			
			return $array;
		}
		
		public static function SetProperties($object, $property_array)
		{
			// set this objects properties using the property name as the array index
			foreach($object as $key => &$value)
				$value = $property_array[$key];
		}
	}
?>