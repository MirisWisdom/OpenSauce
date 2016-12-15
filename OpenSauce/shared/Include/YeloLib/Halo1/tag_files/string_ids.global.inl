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
// mapping functions
__STRING_ID(one)
__STRING_ID(zero)
__STRING_ID(random)
__STRING_ID(function_a_in)
__STRING_ID(function_b_in)
__STRING_ID(function_c_in)
__STRING_ID(function_d_in)
__STRING_ID(function_a_out)
__STRING_ID(function_b_out)
__STRING_ID(function_c_out)
__STRING_ID(function_d_out)

//////////////////////////////////////////////////////////////////////////
// object functions
//__STRING_ID(current_body_damage)
//__STRING_ID(current_shield_damage)
//__STRING_ID(body_vitality)
//__STRING_ID(shield_vitality)
//__STRING_ID(object_overshield_amount)
//__STRING_ID(alive)
//__STRING_ID(compass)
	//////////////////////////////////////////////////////////////////////////
	// device
	//__STRING_ID(power)
	//__STRING_ID(change_in_power)
	//__STRING_ID(position)
	//__STRING_ID(change_in_position)
	//__STRING_ID(locked)
	//__STRING_ID(delay)
	//////////////////////////////////////////////////////////////////////////
	// projectile
	//__STRING_ID(time_remaining)
	//__STRING_ID(range_remaining)
	//__STRING_ID(tracer)
	//__STRING_ID(acceleration_range) // H2
	//__STRING_ID(projectile_attach) // H2
	//////////////////////////////////////////////////////////////////////////
	// weapon
	//__STRING_ID(heat)
	//__STRING_ID(turned_on) // H2
	//__STRING_ID(illumination)
	//__STRING_ID(primary_ammunition)
	//__STRING_ID(primary_ammunition_ones) // H2
	//__STRING_ID(primary_ammunition_tens) // H2
	//__STRING_ID(secondary_ammunition)
	//__STRING_ID(primary_ejection_port)
	//__STRING_ID(secondary_ejection_port)
	//__STRING_ID(primary_rate_of_fire)
	//__STRING_ID(secondary_rate_of_fire)
	//__STRING_ID(primary_firing)
	//__STRING_ID(secondary_firing)
	//__STRING_ID(primary_charged)
	//__STRING_ID(secondary_charged)
	//__STRING_ID(integrated_light)
	//__STRING_ID(primary_trigger_down) // H2
	//__STRING_ID(secondary_trigger_down) // H2
	//////////////////////////////////////////////////////////////////////////
	// unit
	//__STRING_ID(driver_seat_power)
	//__STRING_ID(gunner_seat_power)
	//__STRING_ID(aiming_change)
	//__STRING_ID(horizontal_aiming_change) // H2
	//__STRING_ID(mouth_aperture)
	//__STRING_ID(integrated_light_power)
	//__STRING_ID(can_blink)
	//__STRING_ID(shield_sapping) // not in H2
	//__STRING_ID(driver_seat_occupied) // H2
	//__STRING_ID(gunner_seat_occupied) // H2
	//__STRING_ID(flying_speed) // H2
	//__STRING_ID(engine_power) // H2
	//__STRING_ID(unit_open) // H2
	//__STRING_ID(unit_closed) // H2
	//////////////////////////////////////////////////////////////////////////
	// biped
	//////////////////////////////////////////////////////////////////////////
	// vehicle
	//__STRING_ID(speed_absolute)
	//__STRING_ID(speed_forward)
	//__STRING_ID(speed_backward)
	//__STRING_ID(slide_absolute)
	//__STRING_ID(speed_slide_maximum)
	//__STRING_ID(turn_absolute)
	//__STRING_ID(boost) // H2
	//__STRING_ID(velocity_air)
	//__STRING_ID(velocity_water)
	//__STRING_ID(velocity_ground)
	//__STRING_ID(velocity_forward)
	//__STRING_ID(velocity_left)
	//__STRING_ID(velocity_up)
	//__STRING_ID(left_tread_position)
	//__STRING_ID(right_tread_position)
	//__STRING_ID(left_tread_velocity)
	//__STRING_ID(right_tread_velocity)
	//__STRING_ID(front_left_tire_position)
	//__STRING_ID(front_right_tire_position)
	//__STRING_ID(back_left_tire_position)
	//__STRING_ID(back_right_tire_position)
	//__STRING_ID(front_left_tire_velocity)
	//__STRING_ID(front_right_tire_velocity)
	//__STRING_ID(back_left_tire_velocity)
	//__STRING_ID(back_right_tire_velocity)
	//__STRING_ID(wingtip_contrail)
	//__STRING_ID(thrust)
	//__STRING_ID(hover)
	//__STRING_ID(engine_hack)
	//__STRING_ID(engine_rpm)
	//__STRING_ID(wingtip_contrail_new)
	//__STRING_ID(mean_antigrav) // H2

//////////////////////////////////////////////////////////////////////////
// object marker names
//__STRING_ID(head)
//__STRING_ID(body)
//__STRING_ID(left_hand)
//__STRING_ID(melee)				// unit
//__STRING_ID(ground_point)			// item
//__STRING_ID(front)				// control
//__STRING_ID(primary_trigger)		// weapon
//__STRING_ID(secondary_trigger)	// weapon

//////////////////////////////////////////////////////////////////////////
// effect marker names
//normal
//incident
//__STRING_ID(negative_incident)
//__STRING_ID(reflection)
//__STRING_ID(gravity)

//////////////////////////////////////////////////////////////////////////
// particle effect marker names
//__STRING_ID(velocity)
//gravity

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
// weapon labels (from rename-weapon-labels)
//__STRING_ID(pistol) // hp, pp, ne, b
//__STRING_ID(rifle) // ar, pr, sg, sr, f, amg
//__STRING_ID(missile) // rl, fr
//__STRING_ID(support) // bs, ft
//__STRING_ID(blade) // fb
//__STRING_ID(unarmed) // hfr

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
