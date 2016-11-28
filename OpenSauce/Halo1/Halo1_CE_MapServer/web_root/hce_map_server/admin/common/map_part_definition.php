<?php
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
	class PartDefinition
	{
		public $name;
		public $index;
		public $md5;
		public $size;
		public $encrypted;
		public $unencrypted_md5;

		function ReadElement($element)
		{
			$this->name = $element->getAttribute("name");
			$this->index = intval($element->getAttribute("index"));
			$this->md5 = strtolower($element->getAttribute("md5"));
			$this->size = intval($element->getAttribute("size"));
			$this->encrypted = ($element->getAttribute("encrypted") === 'true');
			$this->unencrypted_md5 = strtolower($element->getAttribute("unencrypted_md5"));

			if($this->name == "") return false;
			if($this->md5 == "") return false;		
			if($this->encrypted && ($this->unencrypted_md5 == "")) return false;
			return true;
		}
	}

	class MapDefinition
	{
		public $algorithm;
		public $name;
		public $md5;
		public $uncompressed_size;
		public $compressed_size;
		public $part = array();

		function ReadElement(DOMElement $element)
		{
			$algorithm = $element->getAttribute("algorithm");

			if($algorithm == "zip") $this->algorithm = 0;
			if($algorithm == "7zip") $this->algorithm = 1;
			$this->name = $element->getAttribute("name");
			$this->md5 = strtolower($element->getAttribute("md5"));
			$this->uncompressed_size = intval($element->getAttribute("uncompressed_size"));
			$this->compressed_size = intval($element->getAttribute("compressed_size"));

			$part_nodes_list = $element->getElementsByTagName("part");
			foreach($part_nodes_list as $value)
			{
				$part_entry = new PartDefinition();

				if($part_entry->ReadElement($value))
					$this->part[] = $part_entry;
				else
					return false;
			}

			if($this->name == "") return false;
			if($this->md5 == "") return false;
			if($this->uncompressed_size == 0) return false;
			if($this->compressed_size == 0) return false;
			if(count($this->part) == 0) return false;
			return true;
		}
	}

	function LoadDefinition($file = null)
	{
		if($file == null)
			return null;

		// open the xml file
		$document = new DOMDocument();
		if(FALSE == $document->load($file))
			return null;

		//TODO: make a func for this
		// get the root node list
		if(!($root_node_list = $document->getElementsByTagName("osHTTPServer")))
			return null;

		//verify there is only one root node, it is not null, and it is a DOMElement
		if((1 != $root_node_list->length) ||
			!$root_node_list->item(0) ||
			(XML_ELEMENT_NODE != $root_node_list->item(0)->nodeType))
			return null;
		$root_node = $root_node_list->item(0);

		// get the map_download node list
		if(!($map_download_node_list = $root_node->getElementsByTagName("map_download")))
			return null;
		//verify there is only one root node, it is not null, and it is a DOMElement
		if((1 != $map_download_node_list->length) ||
			!$map_download_node_list->item(0) ||
			(XML_ELEMENT_NODE != $map_download_node_list->item(0)->nodeType))
			return null;
		$map_download = $map_download_node_list->item(0);


		// read the definition
		$map_definition = new MapDefinition();
		if(!$map_definition->ReadElement($map_download))
			return null;
		return $map_definition;
	}
?>
