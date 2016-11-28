/*
    Yelo: Open Sauce SDK
		Halo 2 (Editing Kit) Edition

	See license\OpenSauce\Halo2_CheApe for specific license information
*/


//////////////////////////////////////////////////////////////////////////
// LostToolCommands.inl

static const s_tool_command_argument import_model_render_arguments[] = {
	_tool_command_argument_type_data_file,
	L"jms file",
	"*.jms|*\\render\\*.jms|"
		"Model jms files must reside in a 'render' sub-directory, for example, 'foo\\render\\bar.jms'",
	"JMS file you wish to create a render model from",
};
static const s_tool_command import_model_render = {
	L"model render",
	import_model_render_proc,
	import_model_render_arguments,	NUMBEROF(import_model_render_arguments),
	false
};


static const s_tool_command_argument import_model_collision_arguments[] = {
	_tool_command_argument_type_data_directory,
	L"jms file",
	"*.jms|*\\collision\\*.jms|"
		"Model jms files must reside in a 'collision' sub-directory, for example, 'foo\\collision\\bar.jms'",
	"JMS file you wish to create a collision model from",
};
static const s_tool_command import_model_collision = {
	L"model collision",
	CAST_PTR(_tool_command_proc, 0x4153F0),
	import_model_collision_arguments,	NUMBEROF(import_model_collision_arguments),
	false
};


static const s_tool_command_argument import_model_physics_arguments[] = {
	_tool_command_argument_type_data_file,
	L"jms file",
	"*.jms|*\\physics\\*.jms|"
		"Model jms files must reside in a 'physics' sub-directory, for example, 'foo\\physics\\jms.ass'",
	"JMS file you wish to create a physics model from",
};
static const s_tool_command import_model_physics = {
	L"model physics",
	CAST_PTR(_tool_command_proc, 0x41AB20),
	import_model_physics_arguments,	NUMBEROF(import_model_physics_arguments),
	false
};


static const s_tool_command_argument import_model_arguments[] = {
	{
		_tool_command_argument_type_tag_name,
		L"object name",
	},
	{
		_tool_command_argument_type_string,
		L"object type",
	}
};
static const s_tool_command import_model = {
	L"model object",
	NULL,
	import_model_arguments,	NUMBEROF(import_model_arguments),
	false
};



//////////////////////////////////////////////////////////////////////////
// LostToolCommands_Animations.inl

static const s_tool_command_argument import_model_animations_arguments[] = {
	_tool_command_argument_type_data_file,
	L"source-directory",
	NULL,
	"Directory containing the JMA files you wish to import",
};
static const s_tool_command import_model_animations = {
	L"model animations",
	import_model_animations_proc,
	import_model_animations_arguments,	NUMBEROF(import_model_animations_arguments),
	false
};



//////////////////////////////////////////////////////////////////////////
// LostToolCommands_Sounds.inl

static const s_tool_command_argument import_sound_arguments[] = {
	_tool_command_argument_type_data_file,
	L"sound file",
	""
	"",
};
static const s_tool_command import_sound = {
	L"import sound",
	reimport_sound_proc,
	import_sound_arguments,	NUMBEROF(import_sound_arguments),
	false
};



//////////////////////////////////////////////////////////////////////////
// BuildCacheFile.inl

static const s_tool_command_argument build_cache_file_ex_definition_arguments[] = {
	{
		_tool_command_argument_type_tag_name,
		L"scenario",
		"*.scenario",
		"",
	},
	{
		_tool_command_argument_type_string,
		L"tags base address",
		"0 or a number in the format of: 0x00000000",
		"virtual address to use for the tag memory"
	}
};
static const s_tool_command build_cache_file_ex_definition = {
	L"build cache file ex",
	build_cache_file_ex_proc,
	build_cache_file_ex_definition_arguments,	NUMBEROF(build_cache_file_ex_definition_arguments),
	false
};



//////////////////////////////////////////////////////////////////////////
// UpgradeTags.inl

static const s_tool_command_argument import_tool_upgrade_all_tags_arguments[] = {
	{}
};
static const s_tool_command import_tool_upgrade_all_tags = {
	L"upgrade all tags",
	tool_upgrade_all_tags_proc,
	import_tool_upgrade_all_tags_arguments,	0,
	false
};