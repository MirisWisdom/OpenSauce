----------------------------------------------------------------------------------------------------
-- Material "Fixer" script
-- This script automates setting all materials as viewed in the viewport,
-- with no alpha source on the diffuse map
----------------------------------------------------------------------------------------------------

fn setDiffuse changeMat = 
(
	if classof changeMat == multimaterial then
	(
		for i = 1 to changeMat.numsubs do
		(
			if changeMat.materialList[i] != undefined then
			(
				setDiffuse changeMat.materialList[i]
			)
		)
	)
	else if classof changeMat == standardmaterial then
	(
		changeMat.showInViewport = true
		changeMat.adLock = true
		changeMat.adTextureLock = true
		if changeMat.diffuseMap != undefined then
		(
			setDiffuse changeMat.diffuseMap
		)
	)
	else if classof changeMat == bitmaptexture then
	(		
		changeMat.alphasource = 2
	)
)

Utility MaterialFixScript "Material Fix"
(
	rollout MaterialFixRollout "Material Fix Script" width:160 height:36
	(	
		button FixBtn "Fix Materials" pos:[8,8] width:144 height:20		
	
		on FixBtn pressed do
		(
			for i=1 to selection.count do
			(
				s = selection[i]
				if s.material != unassigned do
				(
					setDiffuse s.material
				)					
			)	
		)
	)
	
	on MaterialFixScript Open do
	(
		AddRollout MaterialFixRollout 
	)

	on MaterialFixScript Close do
	(
		RemoveRollout MaterialFixRollout 
	)
)



