--	Yelo: Open Sauce SDK
--		Halo 1 (CE) Edition

--	See license\OpenSauce\Halo1_CE for specific license information

require "HaloCE/1_09_Client/Game_EngineFunctions"
require "HaloCE/1_09_Client/Game_HS"
require "HaloCE/1_09_Client/Game"
require "HaloCE/1_09_Client/Interface"
require "HaloCE/1_09_Client/Networking"
require "HaloCE/1_09_Client/Objects"
require "HaloCE/1_09_Client/OpenSauce"
require "HaloCE/1_09_Client/Rasterizer_DX9"
require "HaloCE/1_09_Client/Rasterizer"
require "HaloCE/1_09_Client/Rasterizer_ShaderDraw"
require "HaloCE/1_09_Client/Rasterizer_ShaderExtension"
require "HaloCE/1_09_Client/Scenario"
require "HaloCE/1_09_Client/TagGroups"

require "HaloCE/1_09_Server/Game_EngineFunctions"
require "HaloCE/1_09_Server/Game_HS"
require "HaloCE/1_09_Server/Game"
require "HaloCE/1_09_Server/Networking"
require "HaloCE/1_09_Server/Objects"
require "HaloCE/1_09_Server/OpenSauce"
require "HaloCE/1_09_Server/Scenario"
require "HaloCE/1_09_Server/TagGroups"

-- Updates all of the Halo CE 1.09 runtime pointers to a different binary version
function update_client_pointers(old, new, version)
	if not old then error("the old binary path is required") end
	if not new then error("the new binary path is required") end
	if not version then error("the version is required") end

	local version_full = "HaloCE_"..version.."_Runtime"

	update_system_pointers("Game.EngineFunctions",       HaloCEClient_Game_EngineFunctions, HaloCEClient_Game_EngineFunctions_Index, old, new, version_full)
	update_system_pointers("Game.HS",                    HaloCEClient_Game_HS, HaloCEClient_Game_HS_Index, old, new, version_full)
	update_system_pointers("Game",                       HaloCEClient_Game, HaloCEClient_Game_Index, old, new, version_full)
	update_system_pointers("Interface",                  HaloCEClient_Interface, HaloCEClient_Interface_Index, old, new, version_full)
	update_system_pointers("Networking",                 HaloCEClient_Networking, HaloCEClient_Networking_Index, old, new, version_full)
	update_system_pointers("Objects",                    HaloCEClient_Objects, HaloCEClient_Objects_Index, old, new, version_full)
	update_system_pointers("OpenSauce",                  HaloCEClient_OpenSauce, HaloCEClient_OpenSauce_Index, old, new, version_full)
	update_system_pointers("Rasterizer.DX9",             HaloCEClient_Rasterizer_DX9, HaloCEClient_Rasterizer_DX9_Index, old, new, version_full)
	update_system_pointers("Rasterizer",                 HaloCEClient_Rasterizer, HaloCEClient_Rasterizer_Index, old, new, version_full)
	update_system_pointers("Rasterizer.ShaderDraw",      HaloCEClient_Rasterizer_ShaderDraw, HaloCEClient_Rasterizer_ShaderDraw_Index, old, new, version_full)
	update_system_pointers("Rasterizer.ShaderExtension", HaloCEClient_Rasterizer_ShaderExtension, HaloCEClient_Rasterizer_ShaderExtension_Index, old, new, version_full)
	update_system_pointers("Scenario",                   HaloCEClient_Scenario, HaloCEClient_Scenario_Index, old, new, version_full)
	update_system_pointers("TagGroups",                  HaloCEClient_TagGroups, HaloCEClient_TagGroups_Index, old, new, version_full)
end

-- Updates all of the Halo CE 1.09 server pointers to a different binary version
function update_server_pointers(old, new, version)
	if not old then error("the old binary path is required") end
	if not new then error("the new binary path is required") end
	if not version then error("the version is required") end

	local version_full = "HaloCE_"..version.."_Dedi"

	update_system_pointers("Game.EngineFunctions", HaloCEServer_Game_EngineFunctions, HaloCEServer_Game_EngineFunctions_Index, old, new, version_full)
	update_system_pointers("Game.HS",              HaloCEServer_Game_HS, HaloCEServer_Game_HS_Index, old, new, version_full)
	update_system_pointers("Game",                 HaloCEServer_Game, HaloCEServer_Game_Index, old, new, version_full)
	update_system_pointers("Networking",           HaloCEServer_Networking, HaloCEServer_Networking_Index, old, new, version_full)
	update_system_pointers("Objects",              HaloCEServer_Objects, HaloCEServer_Objects_Index, old, new, version_full)
	update_system_pointers("OpenSauce",            HaloCEServer_OpenSauce, HaloCEServer_OpenSauce_Index, old, new, version_full)
	update_system_pointers("Scenario",             HaloCEServer_Scenario, HaloCEServer_Scenario_Index, old, new, version_full)
	update_system_pointers("TagGroups",            HaloCEServer_TagGroups, HaloCEServer_TagGroups_Index, old, new, version_full)
end

-- Updates a single pointer value from an old binary file to a new one
function update_pointer(old, new, old_value)
	if not old then error("the old binary path is required") end
	if not new then error("the new binary path is required") end
	if not old_value then error("the pointer to find is required") end

	-- Find a matching pointer
	local result = asmdiff.search(old_value, new, old)

	-- Print the result
	if result[1] == nil then
		io:write("\tPointer not found :( ")
	else
		local new_value = result[1]
		local offset = new_value - old_value

		if offset > 0 then
			print(string.format("Pointer Found\t0x%04X -> 0x%04X\t+0x%04X", old_value, new_value, math.abs(offset)))
		elseif offset < 0 then
			print(string.format("Pointer Found\t0x%04X -> 0x%04X\t-0x%04X", old_value, new_value, math.abs(offset)))
		else
			print(string.format("Pointer Found\t0x%04X -> 0x%04X\t==", old_value, new_value))
		end
	end
end

-- Updates a table of pointers for a single game system
function update_system_pointers(system, pointers, pointer_indices, old, new, version)
	if not system then error("system was not defined") end
	if not pointers then error("pointers was not defined") end
	if not pointer_indices then error("pointer_indices was not defined") end
	if not old then error("old was not defined") end
	if not new then error("new was not defined") end
	if not version then error("version was not defined") end

	io.write(string.format("Updating %s...", system))

	-- Find all matching pointers
	local result = asmdiff.search(pointers, new, old)

	-- Save the results to file
	save_pointers(result, pointer_indices, string.format("HaloCE/Result/%s.%s.inl", version, system))
	save_difference(pointers, result, pointer_indices, version, system)

	io.write("done\r\n")
end

-- Search a table of pointers for duplicate values
function find_duplicates(pointers)
	if not pointers then error("pointers was not defined") end

	local duplicates_list = {}

	for key,value in pairs(pointers) do
		for test_key,test_value in pairs(pointers) do
			if key ~= test_key and value == test_value then
				duplicates_list[key] = value
			end
		end
	end

	return duplicates_list
end

-- Saves a table of pointers to a C++ compatible .inl file
function save_pointers(pointers, pointer_indices, output_file_path)
	if not pointers then error("pointers was not defined") end
	if not output_file_path then error("output_file_path was not defined") end
	if not pointer_indices then error("pointer_indices was not defined") end

	-- Determine whether any keys have duplicate pointers
	local duplicates_list = find_duplicates(pointers)

	-- Print all non-duplicate pointers to file
	local output_file = io.open(output_file_path, "w")

	if output_file == nil then
		error("Failed to open the output file when saving pointers. Does the \"HaloCE/Result\" directory exist?")
	end

	output_file:write(
[[
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

]])

	-- For each pointer, print the key and value
	for i = 1, table.getn(pointer_indices) do
		local key = pointer_indices[i]
		local value = pointers[key]

		-- If the pointer is not nil and is not a duplicate, write it to file
		if duplicates_list[key] == nil and value ~= nil then
			output_file:write(string.format("#define %s 0x%06X\n", key, value))
		end
	end

	-- Finish writing to the file and close it
	output_file:flush();
	output_file:close();
end

-- Compares two pointer tables and prints the differences between the two
function save_difference(old_pointers, new_pointers, pointer_indices, version, system)
	if not old_pointers then error("old_pointers was not defined") end
	if not new_pointers then error("new_pointers was not defined") end
	if not pointer_indices then error("pointer_indices was not defined") end
	if not version then error("version was not defined") end
	if not system then error("system was not defined") end

	-- Determine whether any keys have duplicate pointers
	local duplicates_list = find_duplicates(new_pointers)

	-- Open the file to write to
	local output_file = io.open("HaloCE/Result/"..version.."."..system..".Diff.txt", "w")

	if output_file == nil then
		error("Failed to open the output file when saving the diff file. Does the \"HaloCE/Result\" directory exist?")
	end

	for i = 1, table.getn(pointer_indices) do
		local key = pointer_indices[i]
		local old_value = old_pointers[key]
		local new_value = new_pointers[key]

		-- Write whether the pointer was found, not found or is a duplicate
		if duplicates_list[key] ~= nil then
			output_file:write(string.format("%s\t0x%04X\tDUPE\n", key, old_value))
		elseif new_pointers[key] == nil then
			output_file:write(string.format("%s\t0x%04X\tNOT_FOUND\n", key, old_value))
		else
			local offset = new_value - old_value

			if offset > 0 then
				output_file:write(string.format("%s\t0x%04X -> 0x%04X\t+0x%04X\n", key, old_value, new_value, math.abs(offset)))
			elseif offset < 0 then
				output_file:write(string.format("%s\t0x%04X -> 0x%04X\t-0x%04X\n", key, old_value, new_value, math.abs(offset)))
			else
				output_file:write(string.format("%s\t0x%04X -> 0x%04X\t==\n", key, old_value, new_value))
			end
		end
	end

	-- Finish writing to the file and close it
	output_file:flush();
	output_file:close();
end
