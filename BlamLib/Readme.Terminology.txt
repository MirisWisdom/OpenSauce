In BlamLib, I try to stick to the official engine-speak as much as possible. However, the modding community has created some of it's own names for things over the years. 
The following is a partial list of official names and their modding community counterparts.

Tag Block
	* struct
Tag Block (Element)
	* chunk
Tag Data (I'm actually not sure if the community has ever really given this tag-field a name, but here is what I have heard them referred to as over the years)
	* voids
Tag Index\Handle (actually, this itself is just a DatumIndex. See Blam\DataHandles\DatumIndex.cs)
	* tag id
Group Tag (yeah, that four-character-code stuff)
	* tag id
Tag definition
	* metadata
Tag group definition
	* tag layout
Resource data (vertex/indice data, bitmap data, etc)
	* raw data
Tag header
	* cache index header
Tag Instances
	* cache index
	
	
Note: There is such thing as a 'tag-struct', but this was introduced when the Blam engine had tag versioning added. Because it is really only relative to the 
source-tag file format, I don't think a community name was ever given to it.