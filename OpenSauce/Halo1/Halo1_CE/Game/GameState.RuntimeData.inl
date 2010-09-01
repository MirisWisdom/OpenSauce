/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


namespace RuntimeData
{
	struct s_runtime_data {

		struct s_integers {
			bool initialized;
			PAD24;

			int32 values[Enums::k_runtime_data_max_values_count];

			void Clear()
			{
				memset(values, 0, sizeof(values));

				initialized = true;
			}
		}integers;

	};
	// Allocated in the game state
	s_runtime_data* runtime_data;


	// Validate that an operation can be performed at [value_index]
	static bool IntegerOperationValidate(int16 value_index)
	{
		return	runtime_data != NULL && runtime_data->integers.initialized &&
				value_index >= 0 && value_index < Enums::k_runtime_data_max_values_count;
	}
	// Reset all integers back to zero
	static void IntegersReset()
	{
		if(runtime_data != NULL)
			runtime_data->integers.Clear();
	}
	// Get the integer value at [value_index].
	// Returns NONE if this operation fails.
	static int32 IntegerGet(int16 value_index)
	{
		if(IntegerOperationValidate(value_index))
			return runtime_data->integers.values[value_index];

		return NONE;
	}
	// Get the integer value at [value_index], then set it equal [value].
	// Returns NONE if this operation fails.
	static int32 IntegerGetAndSet(int16 value_index, int32 value)
	{
		if(IntegerOperationValidate(value_index))
		{
			// swap the original with the new and return the original
			std::swap(value, runtime_data->integers.values[value_index]);

			return value;
		}

		return NONE;
	}
	// Get the integer value at [value_index], then post-increment it.
	// Returns NONE if this operation fails.
	static int32 IntegerGetAndIncrement(int16 value_index)
	{
		if(IntegerOperationValidate(value_index))
			return runtime_data->integers.values[value_index]++;

		return NONE;
	}
	// Get the integer value at [value_index], then post-decrement it.
	// Returns NONE if this operation fails.
	static int32 IntegerGetAndDecrement(int16 value_index)
	{
		if(IntegerOperationValidate(value_index))
			return runtime_data->integers.values[value_index]--;

		return NONE;
	}


	void InitializeForNewGameState()
	{
		runtime_data = GameState::GameStateGlobals()->Malloc<s_runtime_data>();
	}
	void InitializeForNewMap()
	{
		IntegersReset();
	}
};