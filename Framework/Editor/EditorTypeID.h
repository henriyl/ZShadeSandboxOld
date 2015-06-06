//================================================================================================================
// EditorTypeID.h
//
//================================================================================================================
// History
//
// Created on 5/22/2015 by Dustin Watson
//================================================================================================================
#ifndef __EDITORTYPEID_H
#define __EDITORTYPEID_H
//================================================================================================================
//================================================================================================================

// Contains the IDs for all the components in the editor
namespace EEditorTypeID
{
	enum Type
	{
		//
		// 2D editor ids
		//

		// Editor tab ids
		eEditorTabGamePathLabel,
		eEditorTabGamePathLabel2,
		eEditorTabCreateGameDirectory,
		eEditorTabOpenGameDirectory,
		eEditorTabGameDirectoryNameLabel,
		eEditorTabGameDirectoryNameTextbox,
		eEditorTabGameTypeRadioContainer,
		eEditorTabGameTypeTopdown,
		eEditorTabGameTypePlatformer,
		eEditorTabGameTypeLabel,
		eEditorTabEditModeLabel,
		eEditorTabEditModeRadioContainer,
		eEditorTabEditModeTile,
		eEditorTabEditModeSprite,
		eEditorTabEditModeMusic,
		eEditorTabEditModeHardness,
		eEditorTabEditSubModeTopdownTileLabel,
		eEditorTabEditSubModeTopdownTileRadioContainer,
		eEditorTabEditSubModeTopdownTileRegular,
		eEditorTabEditSubModeTopdownTileMultiStamp,
		eEditorTabEditSubModePlatformerTileLabel,
		eEditorTabEditSubModePlatformerTileRadioContainer,
		eEditorTabEditSubModePlatformerTileRegular,
		eEditorTabEditSubModePlatformerTileMultiStamp,
		eEditorTabEditSubModePlatformerTileAddTile,
		eEditorTabEditSubModePlatformerTileRemoveTile,
		eEditorTabEditSubModePlatformerTileRemoveTexture,
		eEditorTabEditSubModeSpriteLabel,
		eEditorTabEditSubModeSpriteRadioContainer,
		eEditorTabEditSubModeSpriteNone,
		eEditorTabEditSubModeSpriteMove,
		eEditorTabEditSubModeSpriteDelete,
		eEditorTabEditSubModeSpritePlace,
		eEditorTabEditSubModeSpriteSize,
		eEditorTabEditSubModeHardnessLabel,
		eEditorTabEditSubModeHardnessRadioContainer,
		eEditorTabEditSubModeHardnessTiles,
		eEditorTabEditSubModeHardnessSprites,
		eEditorTabMenuButton,
		eEditorTabHUDButton,
		eEditorTabPlay,

		// World tab ids
		eWorldTabWorldCreatedLabel,
		eWorldTabNameLabel,
		eWorldTabNameLabel2,
		eWorldTabCreateButton,
		eWorldTabLoadButton,
		eWorldTabSaveButton,
		eWorldTabMapSizeLabel,
		eWorldTabMapSizeComboBox,
		eWorldTabMapFowEnabledLabel,
		eWorldTabMapFowEnabledRadioContainer,
		eWorldTabMapFowEnabledYes,
		eWorldTabMapFowEnabledNo,
		eWorldTabMapFowRadiusLabel,
		eWorldTabMapFowRadiusComboBox,
		eWorldTabMapIsoStandLabel,
		eWorldTabMapIsoStandRadioContainer,
		eWorldTabMapIsoRadioButton,
		eWorldTabMapStandardRadioButton,
		eWorldTabMapNameLabel,
		eWorldTabMapNameTextbox,
		eWorldTabWorldNameLabel,
		eWorldTabWorldNameComboBox,
		eWorldTabFowFlashlight,
		eWorldTabCreateTopdownMapButton,
		eWorldTabMapWidthLabel,
		eWorldTabMapWidthTextbox,
		eWorldTabMapHeightLabel,
		eWorldTabMapHeightTextbox,
		eWorldTabPlatformerWorldNameLabel,
		eWorldTabPlatformerWorldNameListComboBox,
		eWorldTabPlatformerMapNameLabel,
		eWorldTabPlatformerMapNameTextbox,
		eWorldTabCreatePlatformerMapButton,
		eWorldTabLoadMapButton,
		eWorldTabSaveMapButton,
		eWorldTabWTMapNameLabel,
		eWorldTabWTMapNameComboBox,
		eWorldTabMapRenderedButton,
		eWorldTabMapRemoveButton,

		// Map tab ids
		eMapTabMapNameLabel,
		eMapTabMapNameTextbox,
		eMapTabMapSizeLabel,
		eMapTabMapSizeTextbox,
		eMapTabMapVisionLabel,
		eMapTabMapVisionTextbox,
		eMapTabMapMusicNameLabel,
		eMapTabMapMusicNameTextbox,
		eMapTabToggleHardnessCheckbox,
		eMapTabFowEnabledLabel,
		eMapTabFowEnabledRadioContainer,
		eMapTabFowYesRadioButton,
		eMapTabFowNoRadioButton,
		eMapTabFowFlashlightCheckbox,
		eMapTabFowRadiusLabel,
		eMapTabFowRadiusComboBox,
		eMapTabUpdteAttributesButton,
		eMapTabTileSizeLabel,
		eMapTabTileSizeTextbox,
		eMapTabNumTilesLabel,
		eMapTabNumTilesTextbox,
		eMapTabNumSectionsLabel,
		eMapTabNumSectionsTextbox,
		eMapTabRowSizeLabel,
		eMapTabRowSizeTextbox,
		eMapTabColSizeLabel,
		eMapTabColSizeTextbox,
		eMapTabSectionRowSizeLabel,
		eMapTabSectionRowSizeTextbox,
		eMapTabSectionColSizeLabel,
		eMapTabSectionColSizeTextbox,
		eMapTabSpritesRenderedLabel,
		eMapTabSpritesRenderedTextbox,
		eMapTabTotalSpritesRenderedLabel,
		eMapTabTotalSpritesRenderedTextbox,

		// Tiles tab ids
		eTilesTabTilesComboBox,
		eTilesTabXPosLabel,
		eTilesTabXPosTextbox,
		eTilesTabYPosLabel,
		eTilesTabYPosTextbox,
		eTilesTabBaseTextureLabel,
		eTilesTabBaseTextureTextbox,
		eTilesTabAnimationSpeedLabel,
		eTilesTabAnimationSpeedTextbox,
		eTilesTabAnimationNameLabel,
		eTilesTabAnimationNameTextbox,
		eTilesTabUpdateAnimationButton,
		eTilesTabRevertAnimationButton,
		eTilesTabHardLabel,
		eTilesTabHardRadioContainer,
		eTilesTabHardYesRadioButton,
		eTilesTabHardNoRadioButton,
		eTilesTabUpdateTilePropertiesButton,

		// Sprites tab ids
		eSpritesTabSpritesComboBox,
		eSpritesTabUpdateSpritePropertiesButton,

		// Tile Cache tab ids
		eTileCacheTabCacheList,
		eTileCacheTabSelectTileButton,
		eTileCacheTabUpdateCacheButton,
		eTileCacheTabSelectedTileLabel,
		eTileCacheTabSelectedTileLabel2,

		// Sprite Cache tab ids
		eSpriteCacheTabCacheList,
		eSpriteCacheTabSelectSpriteButton,
		eSpriteCacheTabUpdateCacheButton,
		eSpriteCacheTabSelectedSpriteLabel,
		eSpriteCacheTabSelectedSpriteLabel2,

		//
		// 3D editor ids
		//

		// Editor tab ids
		e3DEditorTabCreateGameDirectory,
		e3DEditorTabOpenGameDirectory,
		e3DEditorTabGameDirectoryNameLabel,
		e3DEditorTabGameDirectoryNameTextbox,
		e3DEditorTabMenuButton,
		e3DEditorTabHUDButton,
		e3DEditorTabPlay,

		// Terrain tab ids


		//
		// Menu editor ids
		//

		eMenuItemListbox,
		eMenuItemSelectedLabel,
		eMenuItemSelectedLabel2,
		eMenuItemSelectButton,
		eMenuItemUpdateButton,
		eMenuItemLabel,
		eMenuItemLabelRadioContainer,
		eMenuItemButton,
		eMenuItemText,
		eMenuItemNone,
		//eMenuCreateButton,
		eMenuLoadButton,
		eMenuSaveButton,
		eMenuRenderedButton,
		eMenuRemoveButton,
		eMenuNameListLabel,
		eMenuNameListComboBox,
		eMenuAddBackground,
		eMenuMapEditorButton,
		eMenuNewNameText,
		eMenuNewNameTextbox,
		eMenuNewSizeText,
		eMenuNewSizeRadioContainer,
		eMenuNewSizeRadio800x600,
		eMenuNewSizeRadio1024x768,
		eMenuNewSizeRadio1366x768,
		eMenuNewSizeRadio1280x1024,
		eMenuNewSizeRadio1920x1080,
		eMenuNewTypeText,
		eMenuNewTypeRadioContainer,
		eMenuNewTypeRadioMainMenu,
		eMenuNewTypeRadioInGameMenu,
		eMenuNewCreateButton,
		
		//
		// HUD editor ids
		//

		eHUDItemListbox,
		eHUDItemSelectedLabel,
		eHUDItemSelectedLabel2,
		eHUDItemSelectButton,
		eHUDItemUpdateButton,
		eHUDItemLabel,
		eHUDItemLabelRadioContainer,
		eHUDItemImage,
		eHUDItemText,
		eHUDItemNone,
		//eHUDCreateButton,
		eHUDLoadButton,
		eHUDSaveButton,
		eHUDRenderedButton,
		eHUDRemoveButton,
		eHUDNameListLabel,
		eHUDNameListComboBox,
		eHUDMapEditorButton,
		eHUDNewSizeText,
		eHUDNewSizeRadioContainer,
		eHUDNewSizeRadio800x600,
		eHUDNewSizeRadio1024x768,
		eHUDNewSizeRadio1366x768,
		eHUDNewSizeRadio1280x1024,
		eHUDNewSizeRadio1920x1080,
		eHUDNewCreateButton,
	};
}

//================================================================================================================
//================================================================================================================
#endif//__EDITORTYPEID_H