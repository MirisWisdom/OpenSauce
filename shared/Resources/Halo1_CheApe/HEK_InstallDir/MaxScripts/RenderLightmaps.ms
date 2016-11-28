----------------------------------------------------------------------------------------------------
-- Directional Lightmap rendering script
-- This script automates the setup and render of directional lightmaps
----------------------------------------------------------------------------------------------------

macroScript LightmapDialog
enabledIn:#("max", "viz")
ButtonText:"Render Lightmaps..."
category:"Render"
internalCategory: "Render" 
toolTip:"Render Lightmaps"
icon:#("Lights", 3)
(
	
--------------------------------------------------------------------------------------------------------------------------------------------
-- Structs
	
struct objectDataStore
(
	castShadows,
	receiveshadows,
	applyAtmospherics,
	secondaryVisibility,
	
	bakeEnabled,
	bakeEnabledElements
)

--------------------------------------------------------------------------------------------------------------------------------------------
-- Variables

local bitmapTypeOptions = #("tif", "bmp", "png", "jpg", "tga")
local bitmapType = 1
local bitmapTypeExtension = bitmapTypeOptions[bitmapType]

local lightmapsOutputFolder = ""
local lightmapsOutputFolderValid = false
local platesOutputFolder = ""
local platesOutputFolderValid = false
local platePrefix = ""
	
local renderOptionsRenderStd = true
local renderOptionsRenderDlm1 = true
local renderOptionsRenderDlm2 = true
local renderOptionsRenderDlm3 = true
local renderSetCount = 4

local buildLightmapPlates = true
local showRenderWindow = false
	
local objectBakeList = #()
local objectBakeListValid = false

local selectedObjectBakeProperties = undefined
local selectedObjectLightmapElement = undefined

local canRenderLightmaps = false

---- Bake variables
local bakeInProgress = false

local lightmapTypeSuffixes = #("std", "dlm1", "dlm2", "dlm3")

local setLightmapsCount = 0
local setLightmapsCompleted = 0
local totalLightmapsCount = 0
local totalLightmapsCompleted = 0

local lightmapBitmapPaths = #()

--------------------------------------------------------------------------------------------------------------------------------------------
-- Forward declarations

---- UI update functions

local 	UpdateSetCount,
			UpdateLightmapsOutputFolder,
			UpdatePlatesOutputFolder,
			UpdateBitmapType,
			UpdateRenderableState,
			UpdateObjectBakeList,
			UpdateForSelectedBakeObject,
			UpdateBakeObjectList,
			UpdateForNewSelection,
			UpdateForNoSelection

---- Object setup functions

local		SetupBakeObjects
			ImportBitmapSizes

---- Bake functions

local		BakeObjects,
			BuildLightmapPlate,
			ValidateBakeListObjects

--------------------------------------------------------------------------------------------------------------------------------------------
-- Rollouts

rollout BakeProgressDialog "Baking Lightmaps..." width:376 height:128
(
	label setTypeLbl "Rendering standard set" pos:[8,8] width:360 height:16
	GroupBox progressGrp "" pos:[8,24] width:360 height:96
	label setProgressLbl "Set Progress" pos:[16,40] width:168 height:16
	label setLightmapCountLbl "Label" pos:[192,40] width:168 height:16 align:#right
	progressBar setProgressPbr "" pos:[16,56] width:344 height:17
	label totalProgressLbl "Total Progress" pos:[16,80] width:168 height:16
	label totalLightmapCountLbl "Label" pos:[192,80] width:168 height:16 align:#right
	progressBar totalProgressPbr "" pos:[16,96] width:344 height:17
	
	function setSetType type =
	(
		setTypeLbl.text = type
	)
	
	function setCancelled =
	(
		setProgressPbr.color = color 100 10 10
		totalProgressPbr.color = color 100 10 10
	)
	
	function UpdateProgress =
	(
		setProgressPbr.value = 100 * ((setLightmapsCompleted as float) / setLightmapsCount)
		setLightmapCountLbl.text = (setLightmapsCompleted as string) + "/" + (setLightmapsCount as string)
		totalProgressPbr.value = 100 * ((totalLightmapsCompleted as float) / totalLightmapsCount)
		totalLightmapCountLbl.text = (totalLightmapsCompleted as string) + "/" + (totalLightmapsCount as string)
	)
)

global LightmapRollout
rollout LightmapRollout "Lightmap Renderer" width:576 height:512
(
	GroupBox outputOptionsGrp "Output Options" pos:[8,296] width:560 height:88
	label fileTypeLbl "Lightmap Ext.:" pos:[32,360] width:72 height:16
	button lightmapOutputBrowseBtn "Browse" pos:[488,336] width:72 height:16
	edittext lightmapOutputTxt "Lightmap Output:" pos:[16,336] width:464 height:16
	dropdownList bitmapTypeDdl "" pos:[104,360] width:72 height:21 items:#("tif", "bmp", "png", "jpg", "tga")
		
	GroupBox lightmapOptionsGrp "Lightmap Options" pos:[8,8] width:560 height:280
	multiListBox lightmapObjectLst "Lightmap Objects" pos:[16,32] width:544 height:10
	button setupObjectsBtn "Setup Objects" pos:[336,32] width:114 height:16
	button updateSelectionBtn "Update Selection" pos:[448,32] width:114 height:16
		
	GroupBox renderOptionsGrp "Render Options" pos:[8,392] width:432 height:112
	label lightmapsToRenderLbl "Lightmaps to Render:" pos:[16,408] width:144 height:16
	checkbox stdLightmapChk "Standard" pos:[16,424] width:144 height:16 checked:true	
	checkbox dlm1LightmapChk "Directional Lightmap 1" pos:[16,448] width:144 height:16 checked:true
	checkbox dlm2LightmapChk "Directional Lightmap 2" pos:[16,464] width:144 height:16 checked:true
	checkbox dlm3LightmapChk "Directional Lightmap 3" pos:[16,480] width:144 height:16 checked:true
	checkbox buildLightmapPlateChk "Build Lightmap Plates" pos:[176,424] width:120 height:16 checked:true
	button renderLightmapsBtn "Render" pos:[448,420] width:121 height:84
	
	GroupBox perObjectGrp "Per-Object" pos:[16,216] width:544 height:64
	spinner uvChannelSpn "UV Channel:" pos:[90,256] width:60 height:16 range:[2,10,2] type:#integer scale:1
	spinner paddingAmountSpn "Padding:" pos:[90,232] width:60 height:16 range:[1,100,5] type:#integer scale:1
	spinner bakeWidthSpn "Width" pos:[192,232] width:80 height:16 range:[1,8096,256] type:#integer scale:1
	spinner bakeHeightSpn "Height" pos:[296,232] width:80 height:16 range:[1,8096,256] type:#integer scale:1
	button importSizesBtn "Import Bitmap Sizes" pos:[197,256] width:180 height:16
	label lightmapListFeedbackLbl "" pos:[16,192] width:544 height:16
	
	edittext platesOutputTxt "     Plates Output:" pos:[16,312] width:464 height:16
	button platesOutputBrowseBtn "Browse" pos:[488,312] width:72 height:16
	label lbl10 "Options:" pos:[176,408] width:144 height:16
	checkbox showRenderWindowChk "Show Render Window" pos:[176,440] width:136 height:16 checked:false
	radiobuttons renderChoiceRadio "" pos:[448,400] width:65 height:32 labels:#("All", "Selected")
	edittext platePrefixTxt "Plate Prefix:" pos:[192,360] width:288 height:16
	
	function SetListItems newItems missingBakeCount =
	(
		lightmapObjectLst.items = newItems
		
		if (missingBakeCount != 0) then
		(
			lightmapListFeedbackLbl.text = (missingBakeCount as string) + " of the selected objects need to be set up"
		)
		else
		(
			lightmapListFeedbackLbl.text = ""
		)
	
		-- If there is 1 or more items, select the first item
		if (objectBakeList.count > 0) then
		(
			lightmapObjectLst.selection = #{1}
			UpdateForSelectedBakeObject 1
		)
		else
		(
			UpdateForSelectedBakeObject 0
		)
	)
	
	function SetBakeOptionsDisabled =
	(		
		perObjectGrp.enabled = false
		uvChannelSpn.enabled = false
		paddingAmountSpn.enabled = false
		bakeWidthSpn.enabled = false
		bakeHeightSpn.enabled = false
	)
	
	function SetBakeOptionsEnabled =
	(		
		perObjectGrp.enabled = true
		uvChannelSpn.enabled = true
		paddingAmountSpn.enabled = true
		bakeWidthSpn.enabled = true
		bakeHeightSpn.enabled = true
	)
	
	function SetBakeProperties =
	(		
		uvChannelSpn.value = selectedObjectBakeProperties.bakeChannel
		paddingAmountSpn.value = selectedObjectBakeProperties.nDilations
		bakeWidthSpn.value = selectedObjectLightmapElement.outputSzX
		bakeHeightSpn.value = selectedObjectLightmapElement.outputSzY
	)
	
	function SetCurrentSelection =
	(
		UpdateForNewSelection()
		UpdateRenderableState()
		
		renderLightmapsBtn.enabled = canRenderLightmaps
	)
	
	function ClearCurrentSelection =
	(
		UpdateForNoSelection()
		UpdateRenderableState()
		
		renderLightmapsBtn.enabled = canRenderLightmaps
	)
	
	on LightmapRollout open do
	(		
		UpdateForNewSelection()
		UpdateRenderableState()
		
		renderLightmapsBtn.enabled = canRenderLightmaps
	)
	on lightmapOutputBrowseBtn pressed do
	(	
		outputPath = getSavePath caption:"Lightmap Output Folder" initialDir:lightmapsOutputFolder
		if outputPath != undefined do
		(
			lightmapOutputTxt.text = outputPath
			UpdateLightmapsOutputFolder outputPath
		)
		
		UpdateRenderableState()
		
		renderLightmapsBtn.enabled = canRenderLightmaps
	)
	on lightmapOutputTxt changed txt do
	(
		lightmapsOutputFolder = txt
		
		UpdateLightmapsOutputFolder txt
		UpdateRenderableState()
		
		renderLightmapsBtn.enabled = canRenderLightmaps
	)
	on bitmapTypeDdl selected sel do
	(
		UpdateBitmapType sel
	)
	on lightmapObjectLst selected sel do
	(
		if lightmapObjectLst.selection.numberSet != 1 then
		(
			UpdateForSelectedBakeObject 0
		)
		else
		(
			selectedIndex = (lightmapObjectLst.selection as Array)[1]
			UpdateForSelectedBakeObject selectedIndex
		)
	)
	on setupObjectsBtn pressed do
	(
		SetupBakeObjects()
		UpdateRenderableState()
		
		renderLightmapsBtn.enabled = canRenderLightmaps
	)
	on updateSelectionBtn pressed do
	(
		UpdateForNewSelection()
		UpdateRenderableState()
		
		renderLightmapsBtn.enabled = canRenderLightmaps
	)
	on stdLightmapChk changed state do
	(
		renderOptionsRenderStd = state
		
		UpdateSetCount()
		UpdateRenderableState()
		
		renderLightmapsBtn.enabled = canRenderLightmaps
	)
	on dlm1LightmapChk changed state do
	(
		renderOptionsRenderDlm1 = state
		
		UpdateSetCount()
		UpdateRenderableState()
		
		renderLightmapsBtn.enabled = canRenderLightmaps
	)
	on dlm2LightmapChk changed state do
	(
		renderOptionsRenderDlm2 = state
		
		UpdateSetCount()
		UpdateRenderableState()
		
		renderLightmapsBtn.enabled = canRenderLightmaps
	)
	on dlm3LightmapChk changed state do
	(
		renderOptionsRenderDlm3 = state
		
		UpdateSetCount()
		UpdateRenderableState()
		
		renderLightmapsBtn.enabled = canRenderLightmaps
	)
	on buildLightmapPlateChk changed state do
	(
		buildLightmapPlates = state
		UpdateRenderableState()
		
		renderLightmapsBtn.enabled = canRenderLightmaps
	)
	on renderLightmapsBtn pressed do
	(
		if (ValidateBakeListObjects() == false) then
		(
			messageBox "The objects listed failed validation. Make sure the list is up to date and all objects have been setup." title:"Lightmap validation failed"
			return 0
		)
		
		objectIndices = #{}
		if (renderChoiceRadio.state == 2) and (lightmapObjectLst.selection.count != 0) do
		(
			objectIndices = lightmapObjectLst.selection
		)
		
		-- Initialise progress dialog values
		setLightmapsCount = objectBakeList.Count
		totalLightmapsCount = objectBakeList.Count * renderSetCount
		totalLightmapsCompleted = 0
		
		createdialog BakeProgressDialog
		
		bakeInProgress = true
		cancelled = false
		
		-- Render the standard lightmap set
		if (renderOptionsRenderStd) then
		(
			setLightmapsCompleted = 0
			BakeProgressDialog.UpdateProgress()
			
			-- Reset the image paths array used for building the lightmap plate
			lightmapBitmapPaths = #()
			
			if (cancelled == false) then
			(
				BakeProgressDialog.setSetType "Rendering standard set"
				BakeObjects &cancelled 1 objectIndices
				
				if((cancelled == false) and buildLightmapPlates) then
				(
					BuildLightmapPlate platePrefix lightmapTypeSuffixes[1]
				)
			)
		)
		-- Render the first directional lightmap component set
		if (renderOptionsRenderDlm1) then
		(
			setLightmapsCompleted = 0
			BakeProgressDialog.UpdateProgress()
			
			-- Reset the image paths array used for building the lightmap plate
			lightmapBitmapPaths = #()
			
			if (cancelled == false) then
			(
				BakeProgressDialog.setSetType "Rendering directional component 1 set"
				BakeObjects &cancelled 2 objectIndices
				
				if((cancelled == false) and buildLightmapPlates) then
				(
					BuildLightmapPlate platePrefix lightmapTypeSuffixes[2]
				)
			)
		)
		-- Render the second directional lightmap component set
		if (renderOptionsRenderDlm2) then
		(
			setLightmapsCompleted = 0
			BakeProgressDialog.UpdateProgress()
			
			-- Reset the image paths array used for building the lightmap plate
			lightmapBitmapPaths = #()
			
			if (cancelled == false) then
			(
				BakeProgressDialog.setSetType "Rendering directional component 2 set"
				BakeObjects &cancelled 3 objectIndices
				
				if((cancelled == false) and buildLightmapPlates) then
				(
					BuildLightmapPlate platePrefix lightmapTypeSuffixes[3]
				)
			)
		)
		-- Render the third directional lightmap component set
		if renderOptionsRenderDlm3 then
		(
			setLightmapsCompleted = 0
			BakeProgressDialog.UpdateProgress()
			
			-- Reset the image paths array used for building the lightmap plate
			lightmapBitmapPaths = #()
			
			if (cancelled == false) then
			(
				BakeProgressDialog.setSetType "Rendering directional component 3 set"
				BakeObjects &cancelled 4 objectIndices
				
				if((cancelled == false) and buildLightmapPlates) then
				(
					BuildLightmapPlate platePrefix lightmapTypeSuffixes[4]
				)
			)
		)
		bakeInProgress = false
		lightmapBitmapPaths = #()
		
		destroydialog BakeProgressDialog
	)
	on uvChannelSpn changed val do
	(
		selectedObjectBakeProperties.bakeChannel = val
	)
	on paddingAmountSpn changed val do
	(
		selectedObjectBakeProperties.nDilations = val
	)
	on bakeWidthSpn changed val do
	(
		selectedObjectLightmapElement.outputSzX = val
	)
	on bakeHeightSpn changed val do
	(
		selectedObjectLightmapElement.outputSzY = val	
	)
	on importSizesBtn pressed do
	(
		ImportBitmapSizes()
		
		lightmapObjectLst.selection = #{1}
	)
	on platesOutputTxt changed txt do
	(
		platesOutputFolder = txt
		
		UpdatePlatesOutputFolder txt
		UpdateRenderableState()
		
		renderLightmapsBtn.enabled = canRenderLightmaps
	)
	on platePrefixTxt changed txt do
	(
		platePrefix = txt
	)	
	on platesOutputBrowseBtn pressed do
	(	
		outputPath = getSavePath caption:"Plates Output Folder" initialDir:platesOutputFolder
		if (outputPath != undefined) do
		(
			platesOutputTxt.text = outputPath
			UpdatePlatesOutputFolder outputPath
		)
		
		UpdateRenderableState()
		
		renderLightmapsBtn.enabled = canRenderLightmaps
	)
	on showRenderWindowChk changed state do
	(
		showRenderWindow = state
	)
)

--------------------------------------------------------------------------------------------------------------------------------------------
-- Functions

---- Search functions

function FindObjectInBakeList objName =
(
	-- Return the index of an item in the bake list with a matching name
	for i = 1 to objectBakeList.count do
	(
		if objectBakeList[i] == objName do
		(
			return i
		)
	)
	
	return 0
)

function FindLightmapBakeElement obj &elementOut =
(
	elementCount = obj.INodeBakeProperties.numBakeElements()
	
	-- Find a lightmap element in the objects bake properties
	for i = 1 to elementCount do
	(
		element = obj.INodeBakeProperties.getBakeElement i
		
		if element.elementName == "DLMLightingMap" then
		(
			elementOut = element
			return true
		)
	)
	
	elementOut = undefined
	return false
)

---- UI update functions

function UpdateSetCount =
(
	renderSetCount = 0
	
	-- Increase the count for each enabled lightmap set
	if (renderOptionsRenderStd) then (renderSetCount += 1)
	if (renderOptionsRenderDlm1) then (renderSetCount += 1)
	if (renderOptionsRenderDlm2) then (renderSetCount += 1)
	if (renderOptionsRenderDlm3) then (renderSetCount += 1)
)

function UpdateLightmapsOutputFolder folder =
(
	-- Set the lightmaps folder and determine whether the path exists
	lightmapsOutputFolder = folder
	
	lightmapsOutputFolderValid = doesFileExist lightmapsOutputFolder
)

function UpdatePlatesOutputFolder folder =
(
	-- Set the lightmap plates folder and determine whether the path exists
	platesOutputFolder = folder
	
	platesOutputFolderValid = doesFileExist platesOutputFolder
)

function UpdateBitmapType index =
(
	bitmapType = index
	
	bitmapTypeExtension = bitmapTypeOptions[bitmapType]
)

function UpdateRenderableState = 
(
	-- Verify that the required variables for a render are set
	canRenderLightmaps = true	
	
	canRenderLightmaps = canRenderLightmaps and lightmapsOutputFolderValid
	canRenderLightmaps = canRenderLightmaps and (renderSetCount != 0)
	canRenderLightmaps = canRenderLightmaps and objectBakeListValid
	if (buildLightmapPlates) do
	(
		canRenderLightmaps = canRenderLightmaps and platesOutputFolderValid	
	)
	canRenderLightmaps = canRenderLightmaps and ValidateBakeListObjects()
)

function UpdateForSelectedBakeObject index =
(
	selectedObjectBakeProperties = undefined
	selectedObjectLightmapElement = undefined
	
	-- Default to disabled UI
	LightmapRollout.SetBakeOptionsDisabled()
	
	LightmapRollout.importSizesBtn.enabled = not (objectBakeList.count == 0)
	
	-- Find the selected object
	if (index == 0) then
	(
		return 0
	)
	
	obj = getNodeByName objectBakeList[index]	
	if (obj == undefined) then
	(
		return 0
	)
	
	-- If the object has no lightmap element, leave the UI disabled
	selectedObjectLightmapElement = undefined
	if not (FindLightmapBakeElement obj &selectedObjectLightmapElement) then
	(
		return 0
	)
	
	-- Set the current lightmap element to edit and update the UI
	selectedObjectBakeProperties = obj.INodeBakeProperties	
	LightmapRollout.SetBakeProperties()
	
	LightmapRollout.SetBakeOptionsEnabled()
)

function UpdateBakeObjectList =
(
	-- Clear the listbox items then add all of the objects in the bake list
	newListItems = #()
	
	missingBakeElementCount = 0
	for i = 1 to objectBakeList.count do
	(
		objectName = objectBakeList[i] as string
		
		obj = getNodeByName objectName
		
		-- If the obejct does not have a lighting bake element point it out to the user
		lmapBake = undefined
		if (not (FindLightmapBakeElement obj &lmapBake)) then
		(
			missingBakeElementCount += 1
			objectName += "*"
		)
		
		newListItems = append newListItems objectName
	)
	
	LightmapRollout.SetListItems newListItems missingBakeElementCount
	
	UpdateRenderableState()
)

function UpdateForNewSelection =
(
	if (bakeInProgress == true) do ( return 0 )
	
	-- Create an new bake data list containing all currenly selected geometry
	newList = #()
	for i = 1 to selection.count do
	(
		-- Filter non-geometry objects and those that aren't bakeable
		if (isKindOf selection[i] GeometryClass) == false then ( continue )
		if (selection[i].INodeBakeProperties == undefined) then ( continue )
		
		append newList selection[i].name
	)
	objectBakeList = newList
	
	objectBakeListValid = (objectBakeList != undefined) and (objectBakeList.count > 0)
	
	UpdateBakeObjectList()
)

function UpdateForNoSelection =
(
	objectBakeList = #()
	
	UpdateBakeObjectList()
)

---- Object setup functions

function SetupBakeObjects =
(
	for i = 1 to objectBakeList.count do
	(
		-- If the object could not be found, return
		obj = getNodeByName objectBakeList[i]	
		if (obj == undefined) then
		(
			return 0
		)
		
		-- If the object already has a lighting bake element, continue
		lightmapElement = undefined
		if (FindLightmapBakeElement obj &lightmapElement) then
		(
			continue
		)
		
		-- Add a new lighting bake element to the object
		lmapElement = LightingMap()
		lmapElement.elementName = "DLMLightingMap"
		lmapElement.outputSzX = 256
		lmapElement.outputSzY = 256
		lmapElement.enabled = true
		
		obj.INodeBakeProperties.nDilations = 5
		obj.INodeBakeProperties.bakeChannel = 2
		
		obj.INodeBakeProperties.addBakeElement lmapElement
		obj.INodeBakeProperties.bakeEnabled = true
	)
	
	UpdateForNewSelection()
)

function ImportBitmapSizes =
(
	struct LightmapSizeEntry
	(
		meshName,
		bitmapWidth,
		bitmapHeight
	)
	
	entryArray = #()
	
	-- Open File
	sizesFile = getOpenFileName caption:"Select the lightmap bitmaps size file:" types:"Lightmap Size File (*.lmsz)|*.lmsz"
	if sizesFile != unassigned do
	(
		openedFile = openFile sizesFile mode:"rt"
		if openedFile != unassigned do
		(		
			-- Read Sizes
			while (eof openedFile != true) do
			(
				textLine =  (readLine openedFile)
				
				splitString = filterstring textLine "\t"					
				splitSize = filterstring splitString[2] "[,]"
				
				entry = LightmapSizeEntry()
				entry.meshName = splitString[1]
				entry.bitmapWidth = splitSize[1] as integer
				entry.bitmapHeight = splitSize[2] as integer
				
				append entryArray entry
			)
		)
		close openedFile
	)
	
	if entryArray.count == 0 then
	(
		return 0
	)
	
	-- Foreach selected model
	matchedModels = 0
	for i = 1 to objectBakeList.count do
	(
		objectName = objectBakeList[i] as string
		
		-- Find matching entry
		sizeEntry = undefined
		for j = 1 to entryArray.count do
		(
			if entryArray[j].meshName == objectName do
			(
				sizeEntry = entryArray[j]
			)
		)
		
		if sizeEntry == undefined then
		(
			continue
		)			
		
		matchedModels += 1
		
		-- Set lightmap size
		obj = getNodeByName objectName
		
		lmapBake = undefined
		if (FindLightmapBakeElement obj &lmapBake) then
		(
			lmapBake.outputSzX = sizeEntry.bitmapWidth
			lmapBake.outputSzY = sizeEntry.bitmapHeight
		)
	)
	
	if matchedModels != objectBakeList.count then
	(
		displayString = "Matched " + (matchedModels as string) + " models out of " + (objectBakeList.count as string)
		messageBox displayString
	)
)
	
---- Bake functions

function SetRenderProperties obj =
(
	-- Set up the object to accumulate light but not cast shadows
	obj.applyAtmospherics = off
	obj.secondaryVisibility = off
	
	obj.INodeBakeProperties.bakeEnabled = true
	
	-- Disable all of the objects bake elements
	elementCount = obj.INodeBakeProperties.numBakeElements()
	for i = 1 to elementCount do
	(
		element = obj.INodeBakeProperties.getBakeElement i
		element.enabled = false
	)
)

function StoreObjectOptions obj =
(
	-- Save the objects options to restore them later
	storedOptions = objectDataStore()
	
	storedOptions.applyAtmospherics = obj.applyAtmospherics
	storedOptions.secondaryVisibility = obj.secondaryVisibility
	
	storedOptions.bakeEnabled = obj.INodeBakeProperties.bakeEnabled
	storedOptions.bakeEnabledElements = #()
	
	elementCount = obj.INodeBakeProperties.numBakeElements()
	
	for i = 1 to elementCount do
	(
		element = obj.INodeBakeProperties.getBakeElement i
		storedOptions.bakeEnabledElements = append storedOptions.bakeEnabledElements element.enabled
	)
	
	return storedOptions
)

function RestoreObjectOptions obj storedOptions =
(
	-- Restore the objects options from the saved copy
	obj.applyAtmospherics = storedOptions.applyAtmospherics
	obj.secondaryVisibility = storedOptions.secondaryVisibility
	
	obj.INodeBakeProperties.bakeEnabled = storedOptions.bakeEnabled
		
	for i = 1 to storedOptions.bakeEnabledElements.count do
	(
		element = obj.INodeBakeProperties.getBakeElement i
		element.enabled = storedOptions.bakeEnabledElements[i]
	)
)

function BakeNode obj filePath &cancelled =
(	
	-- Store and changed object options
	storedOptions = StoreObjectOptions obj
		
	-- Set up the lightmap RTT
	SetRenderProperties obj
	
	-- Use preexisting lighting bake element	
	lmapBake
	FindLightmapBakeElement obj &lmapBake
	lmapBake.filetype = filePath
	lmapBake.enabled = true
	
	-- Bake the lightmap using the resolution of the largest side
	bitmapSize = lmapBake.outputSzX
	if (lmapBake.outputSzY > bitmapSize) then
	(
		bitmapSize = lmapBake.outputSzY
	)
	
	-- Render the lightmap
	select obj
	render rendertype:#bakeSelected vfb:showRenderWindow progressBar:false outputSize:[bitmapSize,bitmapSize] cancelled:&cancelled
	
	if (cancelled == false) then
	(
		-- Create the final bitmap, resizing as necessary before saving
		resizedBitmap = bitmap lmapBake.outputSzX lmapBake.outputSzY filename:filePath
		copy lmapBake.bitmap resizedBitmap
		save resizedBitmap
	)
	
	lmapBake.enabled = false
	
	-- Restore object options to their original values
	RestoreObjectOptions obj storedOptions
	
	gc light:true
)

-----------------------------------------------------------------------------------------------------
-- computeTangentSpace function taken from HalfVector on cgsociety
-- The face position has been removed from the resulting matrices
-- http://forums.cgsociety.org/showpost.php?p=3881243&postcount=11
function computeTangentSpace obj =
(
	local theMesh = snapshotAsMesh obj
	
	local tSpace = #()

	-- Do we have to flip faces?
	local flip = false
	local indices = #(1, 2, 3)
	if dot (cross obj.transform.row1 obj.transform.row2) obj.transform.row3 <= 0 do (
		indices[2] = 3
		indices[3] = 2
		flip = true
	)

	for nFace = 1 to theMesh.numFaces do (
		local face = getFace theMesh nFace
		local tface = getTVFace theMesh nFace
		
		local v1 = getVert theMesh face[indices[1]]
		local v2 = getVert theMesh face[indices[2]]
		local v3 = getVert theMesh face[indices[3]]
		
		local uv1 = getTVert theMesh tface[indices[1]]
		local uv2 = getTVert theMesh tface[indices[2]]
		local uv3 = getTVert theMesh tface[indices[3]]
		
		local dV1 = v1 - v2
		local dV2 = v1 - v3

		local dUV1 = uv1 - uv2
		local dUV2 = uv1 - uv3
		
		local area = dUV1.x * dUV2.y - dUV1.y * dUV2.x
		local sign = if area < 0 then -1 else 1
		
		local tangent = [0,0,1]

		tangent.x = dV1.x * dUV2.y - dUV1.y * dV2.x
		tangent.y = dV1.y * dUV2.y - dUV1.y * dV2.y
		tangent.z = dV1.z * dUV2.y - dUV1.y * dV2.z

		tangent = (normalize tangent) * sign
		
		local normal = normalize (getFaceNormal theMesh nFace)
		if flip do normal = -normal

		local binormal = (normalize (cross normal tangent)) * sign
		
		append tSpace (Matrix3 tangent binormal normal [0,0,0])
	)
	
	delete theMesh
	
	return tSpace
)

function SetNormalsModifier nodeList &editNormalsMod lightmapType =
(
	normalBasis = [0, 0, 1]
	if (lightmapType == 1) then
	(
		return 0
	)
	else if (lightmapType == 2) then
	( 
		normalBasis = [0.796875, 0.0, 0.570313]
	)
	else if (lightmapType == 3) then
	(
		normalBasis = [-0.24, 0.710938, 0.570313]
	)
	else if (lightmapType == 4) then
	(
		normalBasis = [-0.40625, -0.710938, 0.570313]
	)
	
	for j = 1 to nodeList.count do
	(
		obj = nodeList[j]
		
		-- Get the face TBN's
		meshFaceTBNs = computeTangentSpace obj
		
		normalFaceCount = editNormalsMod.getNumFaces node:obj
		normalVertexCount = editNormalsMod.getNumNormals node:obj
		
		-- Create an array of arrays to contain the vertex normals for averaging later
		normalsListArray = #()
		
		for i = 1 to normalVertexCount do
		(
			append normalsListArray #()
		)
		
		for i = 1 to normalFaceCount do
		(
			faceSelectionArray = #{i}
			normalSelection = #{}
			
			-- Get the normal incices of the selected face
			editNormalsMod.ConvertFaceSelection &faceSelectionArray &normalSelection node:obj
			
			-- Get the face TBN
			TBN = meshFaceTBNs[i]		
			
			for k in normalSelection do
			(
				-- Transform the normal basis into tangent space?
				normal = normalize(normalBasis * TBN)
				
				-- Append the new normal to a list for this vertex
				append normalsListArray[k] normal
			)
		)
		
		for i = 1 to normalVertexCount do
		(
			normal = [0, 0, 0]
			
			-- Average the normals from the list for this vertex
			for k = 1 to normalsListArray[i].count do
			(
				normal += normalsListArray[i][k]
			)
			
			normal /= normalsListArray[i].count
			
			-- Set the vertex normal
			editNormalsMod.SetNormal i (normalize normal) node:obj
		)
	)
)

function BakeObjects &cancelled lightmapType objectIndices =
(
	max modify mode
	
	BakeProgressDialog.UpdateProgress()
	
	-- Select all nodes
	clearNodeSelection()
	for i=1 to objectBakeList.count while (cancelled != true) do
	(
		if (objectIndices.numberSet == 0) or (findItem objectIndices i != 0) do
		(
			objectToBake = getNodeByName objectBakeList[i]	
			selectMore objectToBake
		)
	)
	
	-- Add an edit normals modifier
	editNormalsMod = Edit_Normals()
	
	modPanel.addModToSelection editNormalsMod
	
	-- Modify the nodes' normals
	SetNormalsModifier selection &editNormalsMod lightmapType
	
	-- Bake single object and build list of filepaths
	for i=1 to objectBakeList.count while (cancelled != true) do
	(		
		-- Update the progress dialog
		setLightmapsCompleted += 1
		totalLightmapsCompleted += 1
		
		BakeProgressDialog.UpdateProgress()
		
		-- Get the object to bake
		objectToBake = getNodeByName objectBakeList[i]	
		
		-- Build the output bitmap path and bake the lightmap
		filePath = lightmapsOutputFolder + "\\" + objectToBake.name + "_" + lightmapTypeSuffixes[lightmapType] + "." + bitmapTypeExtension
		
		if (objectIndices.numberSet == 0) or (findItem objectIndices i != 0) do
		(
			BakeNode objectToBake filePath &cancelled
		)
		
		-- If the lightmap bake was successful add the path to the bitmap list
		if (cancelled == false) then
		(
			append lightmapBitmapPaths filePath
		)
	)
	
	if (cancelled == true) then
	(
		BakeProgressDialog.setCancelled()
	)
	
	-- Delete edit normals modifiers
	for i=1 to objectBakeList.count do
	(
		if (objectIndices.numberSet == 0) or (findItem objectIndices i != 0) do
		(
			objectToBake = getNodeByName objectBakeList[i]			
			deleteModifier objectToBake editNormalsMod
		)
	)
)

function BuildLightmapPlate lightmapPrefix lightmapSuffix =
(
	bitmapWidth = 0
	bitmapHeight = 0
	
	-- Calculate the width and height of the output bitmap
	for i = 1 to lightmapBitmapPaths.count do
	(
		if (doesFileExist lightmapBitmapPaths[i]) == false do
		(
			tempBitmap = bitmap 256 256 filename:lightmapBitmapPaths[i] color:(color 255 0 0)
			save tempBitmap
		)
		
		lightmapBitmap = openBitmap lightmapBitmapPaths[i]
		
		if (lightmapBitmap.width > bitmapWidth) then
		(
			bitmapWidth = lightmapBitmap.width
		)
		
		bitmapHeight += lightmapBitmap.height
		
		close lightmapBitmap
	)
	
	-- Add the required padding for the blue border
	bitmapWidth += 2	
	bitmapHeight += lightmapBitmapPaths.count + 1
	
	-- Create the bitmap
	filePath = ""
	if (lightmapPrefix != "") then
	(
		filePath = platesOutputFolder + "\\" + lightmapPrefix + "_" + lightmapSuffix + ".tif"
	)
	else
	(		
		filePath = platesOutputFolder + "\\" + lightmapSuffix + ".tif"
	)
	outputBitmap = bitmap bitmapWidth bitmapHeight filename:filePath color:(color 0 0 255)
	
	-- Copy the lightmaps on to the plate
	heightOffset = 1
	for i = 1 to lightmapBitmapPaths.count do
	(
		lightmapBitmap = openBitmap lightmapBitmapPaths[i]
		
		pasteBitmap lightmapBitmap outputBitmap [0, 0] [1, heightOffset]
		heightOffset += lightmapBitmap.height + 1
		
		close lightmapBitmap
	)
	
	-- Save the output bitmap
	save outputBitmap
)

function ValidateBakeListObjects =
(
	-- If the list is empty return false
	if ((objectBakeList == undefined) or (objectBakeList.Count == 0)) then
	(
		return false
	)
	
	for i = 1 to objectBakeList.Count do
	(
		objectName = objectBakeList[i] as string
		
		obj = getNodeByName objectName
		
		-- If an object could not be found return false
		if (obj == undefined) then
		(
			return false
		)
		
		-- If an object does not have a lightmap bake element return false
		lmapBake = undefined
		if (not (FindLightmapBakeElement obj &lmapBake)) then
		(
			return false
		)
	)
	return true
)

on execute do with undo off
(
	local cls = classof LightmapRollout
	if (cls != RolloutClass) or LightmapRollout.isDisplayed do return false
	
	CreateDialog LightmapRollout lockWidth:true lockHeight:true style:#(#style_titlebar, #style_border, #style_sysmenu, #style_minimizebox)
)

on isChecked return
(
	local cls = classof LightmapRollout
	(cls == RolloutClass) and LightmapRollout.isDisplayed
)

on isEnabled return
(
	local cls = classof LightmapRollout
	(cls == RolloutClass)
)

on closeDialogs do with undo off
(
	local cls = classof LightmapRollout
	if (cls == RolloutClass) and LightmapRollout.isDisplayed do destroyDialog DLMRollout
)

) -- end, macroscript LightmapDialog