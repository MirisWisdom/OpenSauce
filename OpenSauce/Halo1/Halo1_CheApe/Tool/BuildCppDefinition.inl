/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
typedef std::vector<std::string> string_vector_t;

class c_array_instance;
// base definition object that provides a name and a
// vector of names for duplicate removal
class c_definition_instance
{
	std::string					m_name; 
	string_vector_t				m_used_names;
public:
	void						SetName(std::string& name) 
	{ 
		m_name.clear(); 
		m_name.assign(name); 
	}
	std::string&				GetName()				{ return m_name; }
	string_vector_t&			GetUsedNamesVector()	{ return m_used_names; }

	virtual void				Ctor()
	{
		m_name.clear();
		m_used_names.clear();
	}

	void Clear()
	{
		m_name.clear();
		m_used_names.clear();
	}

	virtual ~c_definition_instance()
	{
		Clear();
	}
};
// specifically for enums and flags, stores tag
// field references for later printing
class c_string_list_instance : public c_definition_instance
{ 
	const string_list*				m_string_list;
	std::vector<const tag_field*>	m_references;
public:
	void						SetList(const string_list* list)		{ m_string_list = list; }
	const string_list*			GetList() const							{ return m_string_list; }
	void						AddReference(const tag_field* field)	{ m_references.push_back(field); }
	std::vector<const tag_field*>&	GetReferencesVector()				{ return m_references; }

	virtual void				Ctor()
	{
		c_definition_instance::Ctor();

		m_string_list = nullptr;
		m_references.clear();
	}

	virtual ~c_string_list_instance()
	{
		m_references.clear();
	}
};
// c_string_list_instance extension providing the type of flags
class c_flags_instance : public c_string_list_instance
{
	Enums::field_type			m_flags_type;
public:
	void						SetFlagsType(Enums::field_type type)	{ m_flags_type = type; }
	const Enums::field_type		GetFlagsType()							{ return m_flags_type; }

	virtual void				Ctor()
	{
		c_string_list_instance::Ctor();

		m_flags_type = Enums::_field_long_flags;
	}
};
// base class for fields that require a struct definition
class c_struct_instance : public c_definition_instance
{
	std::vector<c_flags_instance>	m_flags;
	std::vector<c_array_instance>	m_arrays;
public:
	std::vector<c_flags_instance>&	GetFlagsVector()	{ return m_flags; }
	std::vector<c_array_instance>&	GetArraysVector()	{ return m_arrays; }

	virtual void					Ctor()
	{
		c_definition_instance::Ctor();

		m_flags.clear();
		m_arrays.clear();
	}

	virtual ~c_struct_instance()
	{
		m_flags.clear();
		m_arrays.clear();
	}
};
// provides necessary information about a block definition
class c_block_instance : public c_struct_instance
{
	tag								m_definition_tag;
	bool							m_is_base_definition;
	PAD24;
	const tag_block_definition*		m_definition;

public:
	const bool						IsBase() const			{ return m_is_base_definition; }
	const tag						GetTag() const			{ return m_definition_tag; }
	const tag_block_definition*		GetDefinition() const	{ return m_definition; }
	void							SetDefinition(
		const tag_block_definition* definition, 
		bool is_base_definition,
		const tag definition_tag) 
	{ 
		m_is_base_definition = is_base_definition;
		m_definition = definition;
		m_definition_tag = definition_tag;
	}

	virtual void					Ctor()
	{
		c_struct_instance::Ctor();

		m_definition = nullptr;
		m_is_base_definition = false;
	}
};
// provides the information necessary to build an array struct
class c_array_instance : public c_struct_instance
{
	const tag_field*				m_array_start;
public:
	void						SetArrayStart(const tag_field* start_field)	{ m_array_start = start_field; }
	const tag_field*			GetArrayStart() const						{ return m_array_start; }

	virtual void				Ctor()
	{
		c_struct_instance::Ctor();
		m_array_start = nullptr;
	}
};

//////////////////////////////////////////////////////////////////////
// string editing

// format the name to make it usable as a variable name
static void FormatName(std::string& name,
	const bool remove_bracket_description = true,
	const bool remove_pre_underscores = true)
{
	std::string::size_type index = std::string::npos;

	// remove unwanted description in the name
	if(remove_bracket_description)
		if((index = name.find_first_of("[]()")) != std::string::npos)
			name.replace(index, name.size() - index, "");
	if(name.size() == 0) return;

	// remove invalid characters
	while((index = name.find_first_of("()[],.- *'+-&#/<>%%")) != std::string::npos)
		name.replace(index, 1, "_");

	// force to lower case
	for( auto& c : name )
		c = tolower(c);

	std::string::iterator iter;

	// remove preceding underscores
	if(remove_pre_underscores)
	{
		for (iter = name.begin(); iter != name.end(); ++iter )
		{
			if(*iter == '_')
			{
				name.replace(iter, iter + 1, "");
				if(name.size() == 0) return;
				if(iter != name.begin()) --iter;
			}
			else
				break;
		}
	}

	// remove duplicate underscores
	// skip past preceding underscores
	iter = name.begin();
	while(*iter == '_' && (iter != name.end()))
		++iter;
	for (; iter != name.end() - 1;)
	{
		do
		{
			if(iter + 1 == name.end())
				break;
			if(*iter == '_' && *(iter + 1) == '_')
				name.replace(iter, iter + 1, "");
			++iter;
		}
		while(*iter == '_');
	}
	if(name.size() == 0) return;

	// remove trailing underscores
	std::string::reverse_iterator riter;
	for (riter = name.rbegin(); (*riter == '_') && (riter != name.rend()); ++riter )
		name.replace(riter.base() - 1, riter.base(), "");
	if(name.size() == 0) return;

	// prevent names from starting with a digit
	if(name.find_first_of("0123456789") == 0)
		name.insert(0, "_");
}
// formats a string to replace invalid characters
static void FormatString(std::string& string)
{
	static struct {
		cstring m_find;
		cstring m_replace;
	}k_replacements[] = {
		{ "\\", "\\\\" },
		{ "\"", "\\\"" },
		{ "\n", "\\n" },
	};

	std::string::size_type index;
	bool string_edited = false;

	for (auto& replacement : k_replacements)
	{
		index = 0;
		while ((index = string.find(replacement.m_find, index)) != std::string::npos)
		{
			string.replace(index, 1, replacement.m_replace);
			index += 2;
			string_edited = true;
		}
	}
}
// iterates through a string vector ensuring the uniqueness of name
static void GetUniqueName(std::string& name,
	string_vector_t& names_vector)
{
	std::string test_name(name);
	int32 count = 0;
	bool is_unique;

	do
	{
		is_unique = true;
		// see if the name already exists in the vector
		for( auto& name : names_vector )
		{
			if(test_name.compare(name) == 0)
			{
				is_unique = false;
				break;
			}
		}
		// if the name was found, append a number and recheck
		// if the name is unique, add it to the vector
		if(!is_unique)
		{
			char number_str[8];
			number_str[0] = 0;
			sprintf_s(number_str, "_%i", count + 1);
			test_name.assign(name);
			test_name.append(number_str);
			count++;
		}
		else
			names_vector.push_back(test_name);
	}
	while(!is_unique);
	// set the name to the unique name
	name.assign(test_name);
}


// gets a unique field name, returns true if the field had its own name, false if no_name was used
static bool GetName(std::string& name,
	cstring raw_name,
	string_vector_t& names_vector,
	const bool remove_bracket_description = true,
	const bool remove_pre_underscores = true,
	const bool use_default = true,
	const bool add_to_vector = true,
	const bool dont_format = false,
	cstring default_string = "no_name")
{			
	if(raw_name)
		name.assign(raw_name);

	bool has_name = StringEditing::GetStringSegment(name, name, nullptr, "^:#*");			
	
	// the field has no name defined so assign a default one
	if(((name.size() == 0) || !has_name) && use_default)
	{
		name.assign(default_string);
		has_name = true;
	}
	if(has_name && !dont_format)
	{
		// Format the name and ensure its unique
		FormatName(name, remove_bracket_description, remove_pre_underscores);
		if(add_to_vector)
			GetUniqueName(name, names_vector);
	}

	return has_name;
}
static bool GetUnits(std::string& units,
	cstring raw_name)
{
	if(raw_name)
		units.assign(raw_name);
	else
		return false;

	if(!StringEditing::GetStringSegment(units, units, ":", "*^#"))
		units.clear();
	else
		FormatString(units);

	return units.size() != 0;
}		
static bool GetDescription(std::string& description,
	cstring raw_name)
{
	if(raw_name)
		description.assign(raw_name);
	else
		return false;

	if(!StringEditing::GetStringSegment(description, description, "#"))
		description.clear();
	else
		FormatString(description);

	return description.size() != 0;
}
// creates a struct name from a name variable, optionally removing "_block" from block names
static void GetStructName(std::string& name,
	string_vector_t& names_vector,
	cstring prepend = "s_", 
	cstring append = nullptr,
	cstring remove = "_block")
{
	std::string::size_type index = std::string::npos;

	// remove "_block" from the block name
	if((index = name.find(remove)) != std::string::npos)
		name.replace(index, name.size() - index, "");

	if(!StringEditing::GetStringSegment(name, name, nullptr, "^:#*"))
		name.assign("no_name");

	FormatName(name);

	// append/prepend custom strings
	if(prepend)	name.insert(0, prepend);
	if(append)	name.insert(name.size(), append);		

	// Format the name and ensure its unique
	GetUniqueName(name, names_vector);
}
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// globals
// TODO: these are causing dynamic initializer funcs to be created by the compiler
// Not bad, just not preferred. Possibly move these to a context object used for building cpp shit
static std::vector<c_string_list_instance>		g_enum_list;
static std::vector<c_array_instance>			g_array_list;
static std::vector<c_block_instance>			g_block_list;
static c_definition_instance					g_enums_namespace;
static c_definition_instance					g_taggroups_namespace;
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// field preprocessing/data collection

// adds an enum to the global enums list
static void AddEnum(const tag_field* field)
{
	// look for a duplicate entry in the enums vector, if found add a reference
	for( auto& e : g_enum_list )
	{
		if(e.GetList() == field->Definition<string_list>())
		{
			e.AddReference(field);
			return;
		}
	}

	// this enum has not been added yet, so add it
	c_string_list_instance enum_instance;

	enum_instance.Ctor();
	enum_instance.SetList(field->Definition<string_list>());
	enum_instance.AddReference(field);

	g_enum_list.push_back(enum_instance); 
}
// adds a flags instance to a blocks flags vector
static void AddFlags(const tag_field* field, 
	c_struct_instance& parent_struct)
{
	// look for a duplicate entry in the blocks flags vector, if found add a reference
	std::vector<c_flags_instance>::iterator  iter; 
	std::vector<c_flags_instance>&   flags_vector(parent_struct.GetFlagsVector());
	for(iter = flags_vector.begin(); iter != flags_vector.end(); ++iter)
	{
		if((*iter).GetList() == field->Definition<string_list>())
		{
			(*iter).AddReference(field);
			return;
		}
	}

	// this flag has not been added yet, so add it
	c_flags_instance flags_instance;

	flags_instance.Ctor();
	flags_instance.SetList(field->Definition<string_list>());
	flags_instance.SetFlagsType(CAST(Enums::field_type, field->type));
	flags_instance.AddReference(field);

	flags_vector.push_back(flags_instance);
}
// adds an array instance to a blocks arrays vector
static void AddArray(const tag_field* field, 
	c_struct_instance& parent_struct)
{
	// look for a duplicate entry in the blocks arrays vector, if found do nothing else...
	std::vector<c_array_instance>::iterator  iter; 
	std::vector<c_array_instance>&   arrays_vector(parent_struct.GetArraysVector());
	for(iter = arrays_vector.begin(); iter != arrays_vector.end(); ++iter)
		if((*iter).GetArrayStart() == field)
			return;

	// this array was not found, so add it
	c_array_instance array_instance;

	array_instance.Ctor();
	array_instance.SetArrayStart(field);

	arrays_vector.push_back(array_instance);
}
// adds a block instance to the global blocks vector
static void AddBlock(const tag_block_definition* block_definition, 
	const bool is_definition = false,
	const tag definition_tag = NONE)
{
	// iterate through the blocks fields
	const tag_field* field_pointer;
	// add blocks
	field_pointer = block_definition->fields;
	while(field_pointer && field_pointer->type != Enums::_field_terminator)
	{
		if(field_pointer->type == Enums::_field_block)
			AddBlock(field_pointer->Definition<tag_block_definition>());
		field_pointer++;
	}

	// look for a duplicate entry in the blocks vector, if found do nothing else...
	std::vector<c_block_instance>::iterator iter;
	for(iter = g_block_list.begin(); iter != g_block_list.end(); ++iter)
		if((*iter).GetDefinition() == block_definition)
			return;

	c_block_instance block_instance;
	block_instance.Ctor();
	block_instance.SetDefinition(block_definition, is_definition, definition_tag);


	// add enums first to keep them in order
	field_pointer = block_definition->fields;
	while(field_pointer && field_pointer->type != Enums::_field_terminator)
	{
		if(field_pointer->type == Enums::_field_enum)
			AddEnum(field_pointer);
		field_pointer++;
	}

	// then blocks
	field_pointer = block_definition->fields;
	while(field_pointer && field_pointer->type != Enums::_field_terminator)
	{
		switch(field_pointer->type)
		{
			case Enums::_field_block:
				AddBlock(field_pointer->Definition<tag_block_definition>());
				break;
			case Enums::_field_byte_flags:
			case Enums::_field_word_flags:
			case Enums::_field_long_flags:
				AddFlags(field_pointer, block_instance);
				break;
			case Enums::_field_array_start:
				AddArray(field_pointer, block_instance);
				break;
		}
		field_pointer++;
	}		
	g_block_list.push_back(block_instance);
}
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// field text output
static void WriteExplanation(FILE* file, 
	const tag_field* field)
{
	// iterate through the definition string, writing each line
	fputs("\t\t\t////////////////////////////////////////////////////////////////\n", file);
	fprintf_s(file, "\t\t\t// %s\n", field->name);

	std::string definition_string(field->Definition<char>());
	std::string definition_line;

	while(StringEditing::GetStringSegment(definition_string, definition_line, nullptr, "\n"))
	{
		fprintf_s(file, "\t\t\t// %s\n", definition_line.c_str());
		
		StringEditing::RemoveStringSegment(definition_string, nullptr, "\n");
	}
}
static void WritePad(FILE* file, 
	const tag_field* field)
{
	int pad_count = CAST_PTR(int, field->Definition<int>());

	// print the pad in its simplest form
	if(pad_count == 2)		fputs("\t\t\tPAD16;\n", file);
	else if(pad_count == 3)	fputs("\t\t\tPAD24;\n", file);
	else if(pad_count == 4)	fputs("\t\t\tPAD32;\n", file);
	else
	{
		cstring type = "byte";
		if(pad_count % 4 == 0)
		{
			type = "int32";
			pad_count /= 4;
		}
		else if(pad_count % 2 == 0)
		{
			type = "int16";
			pad_count /= 2;
		}
		fprintf_s(file, "\t\t\tTAG_PAD(%s, %i);\n", type, pad_count);
	}
}
static void WriteTagReference(FILE* file, 
	const tag_field* field, 
	cstring name)
{
	Yelo::tag_reference_definition* def = field->Definition<Yelo::tag_reference_definition>();

	// write the start of the tag reference entry
	fprintf_s(file, "\t\t\tTAG_FIELD(%s, %s", 
		TagGroups::k_tag_field_definitions[field->type].code_name,
		name);

	// union/struct hack to use a tag as a string
	TagGroups::group_tag_to_string gt; gt.Terminate();

	// tag_reference's can have multiple tag types
	if(def->group_tags && (*def->group_tags != NONE))
	{
		// the field has multiple tag groups
		// loop through the tag group list appending them to the file
		tag* current_tag = def->group_tags;
		fputs(", '", file);
		while(current_tag && ((gt.group = *current_tag) != NONE))
		{
			// swap the endian of the tag group and write it
			gt.TagSwap();
			fprintf_s(file, "%s", gt.str);
			current_tag++;

			//if the next group isn't the terminator put the divider
			if(*current_tag != NONE)
				fputs("', '", file);
		}
		fputs("'", file);
	}
	else if(def->group_tag != NONE)
	{
		// the field only has one tag group
		fputs(", '", file);
		gt.group = def->group_tag;
		gt.TagSwap();
		fputs(gt.str, file);
		fputs("'", file);	
	}
	fputs(");\n", file);
}
static void WriteFlags(FILE* file, 
	const tag_field* field,
	cstring name,
	cstring description,
	c_struct_instance& parent_struct)
{
	// look for the flags string_list in the blocks flags vector
	c_flags_instance* flags_instance = nullptr;
	std::vector<c_flags_instance>::iterator iter;
	std::vector<c_flags_instance>& flags_vector(parent_struct.GetFlagsVector());
	for(iter = flags_vector.begin(); iter != flags_vector.end(); ++iter)
	{
		if((*iter).GetList() == field->Definition<string_list>())
		{
			flags_instance = &(*iter);
			break;
		}
	}
	//once found, print it to file
	fprintf_s(file, "\t\t\tTAG_FIELD(%s, %s", flags_instance->GetName().c_str(), name);
	if(description)
		fprintf_s(file, ", \"%s\"", description);
	fputs(");\n", file);

}
static void WriteEnum(FILE* file, 
	const tag_field* field, 
	cstring name, 
	cstring description)
{
	// look for the enums string_list in the global enums vector
	c_string_list_instance* enum_instance = nullptr;
	std::vector<c_string_list_instance>::iterator iter;
	for(iter = g_enum_list.begin(); iter != g_enum_list.end(); ++iter)
	{
		if((*iter).GetList() == field->Definition<string_list>())
		{
			enum_instance = &(*iter);
			break;
		}
	}
	//once found, print it to file
	fprintf_s(file, "\t\t\tTAG_ENUM(%s", name);
	if(enum_instance)
		fprintf_s(file, ", Enums::%s", enum_instance->GetName().c_str());
	if(description)
		fprintf_s(file, ", \"%s\"", description);

	fprintf_s(file, ");\n");
}		
static void WriteArray(FILE* file, 
	const tag_field* field,
	cstring name,
	c_struct_instance& parent_struct)
{
	// look for the arrays tag_field start in the blocks arrays vector
	c_array_instance* array_instance = nullptr;
	std::vector<c_array_instance>& arrays_vector(parent_struct.GetArraysVector());
	std::vector<c_array_instance>::iterator iter;
	for(iter = arrays_vector.begin(); iter != arrays_vector.end(); ++iter)
	{
		if((*iter).GetArrayStart() == field)
		{
			array_instance = &(*iter);
			break;
		}
	}
	//once found, print it to file
	fprintf_s(file, "\t\t\tTAG_ARRAY(%s, %s, %i);\n", 
		array_instance->GetName().c_str(), name, CAST_PTR(int, field->Definition<int>()));
}
static void WriteBlock(FILE* file, 
	const tag_field* field, 
	cstring name)
{
	// look for the block definition in the global blocks vector
	c_block_instance* block_instance = nullptr;
	std::vector<c_block_instance>::iterator  iter; 
	for(iter = g_block_list.begin(); iter != g_block_list.end(); ++iter)
	{
		if((*iter).GetDefinition() == field->Definition<tag_block_definition>())
		{
			block_instance = &(*iter);
			break;
		}
	}
	//once found, print it to file
	fprintf_s(file, "\t\t\tTAG_TBLOCK(%s, %s);\n",
		name, block_instance->GetName().c_str());
}		
static void WriteField(FILE* file, 
	const tag_field* field, 
	cstring name, 
	cstring units, 
	cstring description)
{
	// default field output with optional units and description
	// NOTE: code_name is NULL for non-codeable fields. printf will thus print "(null)"
	fprintf_s(file, "\t\t\tTAG_FIELD(%s, %s", TagGroups::k_tag_field_definitions[field->type].code_name, name);
	if(units || description)
		fprintf_s(file, ", \"%s\"", (units ? units : ""));
	if(description)
		fprintf_s(file, ", \"%s\"", description);
	fputs(");\n", file);
}
static void WriteTagField(FILE* file,
	const tag_field* field,
	c_struct_instance& parent_block)
{
	std::string field_name;
	std::string field_units;
	std::string field_description;

	cstring name = nullptr;
	cstring units = nullptr;
	cstring description = nullptr;

	bool use_default = (
		(field->type != Enums::_field_skip) && 
		(field->type != Enums::_field_pad) && 
		(field->type != Enums::_field_custom));
	bool add_to_vector = (field->type != Enums::_field_explanation);

	if(GetName(field_name, field->name, parent_block.GetUsedNamesVector(), true, true, use_default, add_to_vector))
		name = field_name.c_str();

	if(GetUnits(field_units, field->name))
		units = field_units.c_str();

	if((field->type != Enums::_field_explanation) && GetDescription(field_description, field->name))
		description = field_description.c_str();

	switch(field->type)
	{
	case Enums::_field_explanation:
		WriteExplanation(file, field);
		break;
	case Enums::_field_tag_reference:
		WriteTagReference(file, field, name);
		break;
	case Enums::_field_pad:
	case Enums::_field_skip:
		WritePad(file, field);
		break;
	case Enums::_field_enum:
		WriteEnum(file, field, name, description);
		break;
	case Enums::_field_block:
		WriteBlock(file, field, name);
		break;
	case Enums::_field_array_start:
		WriteArray(file, field, name, parent_block);
		break;
	case Enums::_field_long_flags:
	case Enums::_field_word_flags:
	case Enums::_field_byte_flags:
		WriteFlags(file, field, name, description, parent_block);
		break;
	default:
		WriteField(file, field, name, units, description);
		break;
	case Enums::_field_array_end:
	case Enums::_field_custom:
		break;
	}

	// extra newline prior to an explanation field
	if(field[1].type == Enums::_field_explanation)
		fputs("\n", file);
}
static void WriteEnumDefinition(FILE* file,
	c_string_list_instance& instance)
{
	auto& references = instance.GetReferencesVector();

	std::string field_name;			

	// print the enum entries
	fputs("\n", stdout);
	if(references.size() == 1)
	{
		GetName(field_name, references[0]->name, instance.GetUsedNamesVector(), false, false, false, false, true);
		printf_s("an enum referenced by field \"%s\" requires an identifier\n", field_name.c_str());
	}
	else
	{
		puts("an enum with multiple references requires an identifier");
		puts("references:");

		std::vector<const tag_field*>::iterator ref_iter;
		for(ref_iter = references.begin(); ref_iter != references.end(); ++ref_iter)
		{
			GetName(field_name, (*ref_iter)->name, instance.GetUsedNamesVector(), false, false, false, false, true);
			printf_s("\t%s\n", field_name.c_str());			
		}
	}

	const string_list* list = instance.GetList();
	puts("entries:");

	cstring* element = list->strings;
	for(int i = 0; i < list->count; i++, element++)
		printf_s("\t%s\n", *element);

	// ask the user what they want to call the enum
	puts("what name should be used for this enumeration type?");
	fputs(": ", stdout);
	char input[512];
	input[0] = '\0';
	if(gets_s(input) != nullptr)
	{
		if(input[0] != '\0')
		{
			input[NUMBEROF(input)-1] = '\0';
			field_name.assign(input);
		}
		else
			field_name.assign(references[0]->name);
	}
	GetName(field_name, field_name.c_str(), g_enums_namespace.GetUsedNamesVector());
	instance.SetName(field_name);

	//write the enum to file
	fprintf(file, "\t\tenum %s : _enum\n", field_name.c_str());
	fputs("\t\t{\n", file);

	element = list->strings;
	for(int32 i = 0; i < list->count; i++, element++)
	{
		std::string element_formatted;

		GetName(element_formatted, *element, instance.GetUsedNamesVector(), 
			false, true, true, true, false, "unused");;

		fprintf(file, "\t\t\t_%s_%s,\n", field_name.c_str(), element_formatted.c_str());
	}

	fputs("\n", file);
	fprintf(file, "\t\t\t_%s,\n", field_name.c_str());

	fputs("\t\t};\n", file);
	fputs("\n", stdout);
}
static void WriteFlagsDefinition(FILE* file,
	c_flags_instance& instance,
	c_struct_instance& parent_block,
	const bool add_boost_asserts)
{
	std::string field_name;
	// if there is only one reference, use its name
	auto& references = instance.GetReferencesVector();
	if(references.size() == 1)			
		field_name.assign(references[0]->name);
	else
	{	
		// if the flags struct has multiple references, the user 
		// will probably want to specify a name for it
		puts("a flags struct with multiple references requires an identifier");
		puts("parent block:");
			printf_s("\t%s\n", parent_block.GetName().c_str());
		puts("references:");

		// print the reference fields names to help the user decide on a name
		std::vector<const tag_field*>::iterator ref_iter;
		for(ref_iter = references.begin(); ref_iter != references.end(); ++ref_iter)
		{
			GetName(field_name, (*ref_iter)->name, instance.GetUsedNamesVector(),
				false, false, false, false, true);	
			printf_s("\t%s\n", field_name.c_str());			
		}			

		// print the flag entries names to help the user decide on a name
		const string_list* list = instance.GetList();
		puts("entries:");

		cstring* element = list->strings;
		for(int i = 0; i < list->count; i++, element++)
			printf_s("\t%s\n", *element);	

		// ask the user what they want to call the enum
		puts("what name should be used for the flags struct?");
		fputs(": ", stdout);
		char input[512];
		input[0] = '\0';
		if(gets_s(input) != nullptr)
		{
			//if they do not specify a name, use the first references field name
			if(input[0] != '\0')
			{
				input[NUMBEROF(input)-1] = '\0';
				field_name.assign(input);
			}
			else
				field_name.assign(references[0]->name);
		}
		fputs("\n", stdout);
	}				
	// format the name, create the struct and make sure its unique
	GetStructName(field_name, parent_block.GetUsedNamesVector(), "__");
	instance.SetName(field_name);

	fprintf_s(file, "\t\t\tstruct %s\n\t\t\t{\n", field_name.c_str());

	// defines the size of the flag value
	char* flag_type = nullptr;
	char* type = nullptr;
	switch(references[0]->type)
	{
	case Enums::_field_byte_flags:
		flag_type = "TAG_FLAG8";
		type = "byte_flags";
		break;
	case Enums::_field_word_flags:
		flag_type = "TAG_FLAG16";
		type = "word_flags";
		break;
	case Enums::_field_long_flags:
		flag_type = "TAG_FLAG";
		type = "long_flags";
		break;
	};

	// loop through the string lists element
	std::string element_name;
	std::string element_description;

	const string_list* enum_names_table = instance.GetList();
	cstring* element = enum_names_table->strings;
	for (int32 i = 0; i < enum_names_table->count; i++, element++)
	{
		// names need to be unique withing the scope of the flags struct
		GetName(element_name, *element, instance.GetUsedNamesVector(), false);
		// elements can have a name and a description so get both
		GetDescription(element_description, *element);

		if(element_description.size())
			fprintf(file, "\t\t\t\t%s(%s, \"%s\");\n", flag_type, element_name.c_str(), element_description.c_str());
		else
			fprintf(file, "\t\t\t\t%s(%s);\n", flag_type, element_name.c_str());
	}
	// it is possible that a flags value is defined but has no entries
	// so in that case, add a single entry to ensure the size of the struct is correct
	if (enum_names_table->count == 0)
		fprintf(file, "\t\t\t\t%s(unused);\n", flag_type);

	fputs("\t\t\t};", file);

	if(add_boost_asserts)
	{
		fprintf(file, 
			" BOOST_STATIC_ASSERT( sizeof(%s) == sizeof(%s) );",
			field_name.c_str(),
			type);
	}

	fputs("\n\n", file);
}
static void WriteArrayDefinition(FILE* file,
	c_array_instance& instance,
	c_struct_instance& parent_block,
	const bool add_boost_asserts)
{
	// get the array name
	std::string field_name(instance.GetArrayStart()->name);

	// array structs are defined in the TagGroups namespace
	GetStructName(field_name, g_taggroups_namespace.GetUsedNamesVector(), "s_array_");
	instance.SetName(field_name);

	// write the array struct
	fprintf_s(file, "\t\tstruct %s\n\t\t{\n", field_name.c_str());

	const tag_field* field = instance.GetArrayStart();
	while(field++ && (field->type != Enums::_field_array_end))
		WriteTagField(file, field, instance);

	fputs("\t\t};\n", file);
}
static void WriteBlockDefinition(FILE* file,
	c_block_instance& instance,
	const bool add_boost_asserts)
{			
	const tag_field* current_field = nullptr;
	const Yelo::tag_block_definition* block_definition = instance.GetDefinition();

	// get the blocks name
	std::string block_name(block_definition->name);

	// block structs are defined in the TagGroups namespace
	GetStructName(block_name, g_taggroups_namespace.GetUsedNamesVector(), 
		"s_", instance.IsBase() ? "_definition" : nullptr);
	instance.SetName(block_name);

	// write the array structs before starting the block itself	
	std::vector<c_array_instance>& arrays_vector(instance.GetArraysVector());
	std::vector<c_array_instance>::iterator array_iter;
	for(array_iter = arrays_vector.begin(); array_iter != arrays_vector.end(); ++array_iter)
		WriteArrayDefinition(file, *array_iter, instance, add_boost_asserts);

	// write the start of the struct
	fprintf_s(file, "\t\tstruct %s\n\t\t{\n", block_name.c_str());

	if(instance.IsBase())		
	{
		fprintf_s(file, "\t\t\tenum { k_group_tag = '%s' };\n\n",
			TagGroups::group_tag_to_string{ instance.GetTag() }.ToString());
	}
	// write the flag structs
	std::vector<c_flags_instance>& flags_vector(instance.GetFlagsVector());
	std::vector<c_flags_instance>::iterator flags_iter;
	for(flags_iter = flags_vector.begin(); flags_iter != flags_vector.end(); ++flags_iter)
		WriteFlagsDefinition(file, *flags_iter, instance, add_boost_asserts);

	// write all of the structs fields
	current_field = block_definition->fields;
	while(current_field && (current_field->type != Enums::_field_terminator))
	{
		WriteTagField(file, current_field, instance);	

		// skip over fields that are inside an array
		// there are in the array structs written previously
		if(current_field->type == Enums::_field_array_start)
			while(current_field->type != Enums::_field_array_end)
				current_field++;

		current_field++;
	}

	// write the end of the struct
	// if the user want the boost assertion added...add it :)
	if(add_boost_asserts)
		fprintf_s(file, "\t\t}; BOOST_STATIC_ASSERT( sizeof(%s) == 0x%X ); // max count: %i\n",
			block_name.c_str(), block_definition->element_size, block_definition->maximum_element_count);
	else
		fprintf_s(file, "\t\t}; // size: %u bytes, max count: %i\n",
			block_definition->element_size, block_definition->maximum_element_count);
	// if this is the base definition struct, it's at the end of the file
	// so we don't want a lines space
	if(!instance.IsBase())
		fputs("\n", file);
}
static void WriteEnumList(FILE* file)
{
	// if a tag has no enums, don't write the Enums namespace
	if(g_enum_list.size() == 0)
		return;

	fputs(
		"\tnamespace Enums\n"
		"\t{\n",
		file);
	
	std::vector<c_string_list_instance>::iterator iter;
	for(iter = g_enum_list.begin(); iter != g_enum_list.end(); ++iter)
		WriteEnumDefinition(file, *iter);

	fputs(
		"\t};\n",
		file);
}
static void WriteBlockList(FILE* file, 
	const bool add_boost_asserts)
{
	fputs(
		"\tnamespace TagGroups\n"
		"\t{\n",
		file);
	
	std::vector<c_block_instance>::iterator iter;
	for(iter = g_block_list.begin(); iter != g_block_list.end(); ++iter)
		WriteBlockDefinition(file, *iter, add_boost_asserts);

	fputs(
		"\t};\n",
		file);
}
static void WriteCppDefinition(
	FILE* file,
	const tag_block_definition* base_block,
	tag definition_tag,
	const bool add_boost_asserts)
{
	AddBlock(base_block, true, definition_tag);

	fputs(
		"namespace Yelo\n"
		"{\n",
		file);
	
	WriteEnumList(file);
	WriteBlockList(file, add_boost_asserts);

	fputs(
		"};\n",
		file);

	g_enum_list.clear();
	g_array_list.clear();
	g_block_list.clear();
	g_enums_namespace.Clear();
	g_taggroups_namespace.Clear();
}
//////////////////////////////////////////////////////////////////////