#undef	__STRING_ID_GENERATE_SET_NAME
#define __STRING_ID_GENERATE_SET_NAME	global
__STRING_ID_SET_BEGIN()
__STRING_ID_EMPTY_STRING_HACK()

__STRING_ID(none)
__STRING_ID(default)

//////////////////////////////////////////////////////////////////////////
// generic states
__STRING_ID(unknown)
__STRING_ID(on)
__STRING_ID(off)
__STRING_ID(enabled)
__STRING_ID(disabled)

//////////////////////////////////////////////////////////////////////////
// difficulty
__STRING_ID(easy)
__STRING_ID(normal)
__STRING_ID(heroic)
__STRING_ID(legendary)

//////////////////////////////////////////////////////////////////////////
// date / time
__STRING_ID(monday)
__STRING_ID(tuesday)
__STRING_ID(wednesday)
__STRING_ID(thursday)
__STRING_ID(friday)
__STRING_ID(saturday)
__STRING_ID(sunday)

__STRING_ID(january)
__STRING_ID(february)
__STRING_ID(march)
__STRING_ID(april)
__STRING_ID(may)
__STRING_ID(june)
__STRING_ID(july)
__STRING_ID(august)
__STRING_ID(september)
__STRING_ID(october)
__STRING_ID(november)
__STRING_ID(december)

__STRING_ID(holiday)

__STRING_ID(time_seconds)
__STRING_ID(time_minutes)
__STRING_ID(time_hours)

//////////////////////////////////////////////////////////////////////////
// language
__STRING_ID(english)
__STRING_ID(japanese)
__STRING_ID(german)
__STRING_ID(french)
__STRING_ID(spanish)
__STRING_ID(mexican)
__STRING_ID(italian)
__STRING_ID(korean)
__STRING_ID(chinese)
__STRING_ID(portuguese)
__STRING_ID(polish)

//////////////////////////////////////////////////////////////////////////
// campaign levels
__STRING_ID(a10)
__STRING_ID(a30)
__STRING_ID(a50)
__STRING_ID(b30)
__STRING_ID(b40)
__STRING_ID(c10)
__STRING_ID(c20)
__STRING_ID(c40)
__STRING_ID(d20)
__STRING_ID(d40)

//////////////////////////////////////////////////////////////////////////
// damage reporting types
__STRING_ID(guardians)
__STRING_ID(scripting)
__STRING_ID(falling_damage)
__STRING_ID(collision_damage)
__STRING_ID(explosion)
__STRING_ID(transfer_damage)
__STRING_ID(teleporter)
__STRING_ID(multiplayer_flag)	// eg, ctf flag, bomb, oddball, etc
	//////////////////////////////////////////////////////////////////////////
	// vehicle
	__STRING_ID(vehicle)		// eg, unoccupied vehicle
	__STRING_ID(vehicle_driver)
	__STRING_ID(vehicle_gunner)
	__STRING_ID(vehicle_passenger)
	//////////////////////////////////////////////////////////////////////////
	// weapons
	__STRING_ID(weapon)
	__STRING_ID(weapon_rifle)
		__STRING_ID(weapon_automatic_rifle)
		__STRING_ID(weapon_sniper_rifle)
	__STRING_ID(weapon_pistol)
	__STRING_ID(weapon_shotgun)
	__STRING_ID(weapon_machine_gun)
	__STRING_ID(weapon_support)	// eg, turret, 3rd person flamethrower, etc
	__STRING_ID(weapon_heavy)
	__STRING_ID(weapon_melee)	// eg, gravity hammer, sword
	//////////////////////////////////////////////////////////////////////////
	// equipment
	__STRING_ID(equipment)
	__STRING_ID(equipment_grenade)
	__STRING_ID(equipment_deployable)
	__STRING_ID(equipment_armor_ability)


__STRING_ID_SET_END()
