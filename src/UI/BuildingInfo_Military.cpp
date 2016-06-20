#include "BuildingInfo.h"

#include "../Calc.h"
#include "../Formation.h"
#include "../Graphics.h"
#include "../Sound.h"
#include "../Widget.h"
#include "../UI/Window.h"

#include "../Data/Building.h"
#include "../Data/CityInfo.h"
#include "../Data/Constants.h"
#include "../Data/Figure.h"
#include "../Data/Formation.h"
#include "../Data/Settings.h"
#include "../Data/State.h"

static void buttonReturnToFort(int param1, int param2);
static void buttonLayout(int param1, int param2);

static CustomButton layoutButtons[] = {
	{19, 139, 103, 223, CustomButton_Immediate, buttonLayout, Widget_Button_doNothing, 0, 0},
	{104, 139, 188, 223, CustomButton_Immediate, buttonLayout, Widget_Button_doNothing, 1, 0},
	{189, 139, 273, 223, CustomButton_Immediate, buttonLayout, Widget_Button_doNothing, 2, 0},
	{274, 139, 358, 223, CustomButton_Immediate, buttonLayout, Widget_Button_doNothing, 3, 0},
	{359, 139, 443, 223, CustomButton_Immediate, buttonLayout, Widget_Button_doNothing, 4, 0}
};

static CustomButton returnButtons[] = {
	{0, 0, 288, 32, CustomButton_Immediate, buttonReturnToFort, Widget_Button_doNothing, 0, 0},
};

static int focusButtonId;
static int returnButtonId;

static BuildingInfoContext *contextForCallback;

void UI_BuildingInfo_drawWall(BuildingInfoContext *c)
{
	c->helpId = 85;
	PLAY_SOUND("wavs/wall.wav");
	Widget_Panel_drawOuterPanel(c->xOffset, c->yOffset, c->widthBlocks, c->heightBlocks);
	Widget_GameText_drawCentered(139, 0, c->xOffset, c->yOffset + 10, 16 * c->widthBlocks, Font_LargeBlack);
	Widget_GameText_drawMultiline(139, 1,
		c->xOffset + 32, c->yOffset + 16 * c->heightBlocks - 158,
		16 * (c->widthBlocks - 4), Font_NormalBlack);
}

void UI_BuildingInfo_drawPrefect(BuildingInfoContext *c)
{
	c->helpId = 86;
	PLAY_SOUND("wavs/prefecture.wav");
	Widget_Panel_drawOuterPanel(c->xOffset, c->yOffset, c->widthBlocks, c->heightBlocks);
	Widget_GameText_drawCentered(88, 0, c->xOffset, c->yOffset + 10, 16 * c->widthBlocks, Font_LargeBlack);
	Widget_GameText_drawCentered(13, 1,
		c->xOffset, c->yOffset + 16 * c->heightBlocks - 24,
		16 * c->widthBlocks, Font_NormalBlack);

	Data_Building *b = &Data_Buildings[c->buildingId];
	if (!c->hasRoadAccess) {
		DRAW_DESC(69, 25);
	} else if (b->numWorkers <= 0) {
		DRAW_DESC(88, 9);
	} else {
		if (b->walkerId) {
			DRAW_DESC(88, 2);
		} else {
			DRAW_DESC(88, 3);
		}
		if (c->workerPercentage >= 100) {
			DRAW_DESC_AT(72, 88, 4);
		} else if (c->workerPercentage >= 75) {
			DRAW_DESC_AT(72, 88, 5);
		} else if (c->workerPercentage >= 50) {
			DRAW_DESC_AT(72, 88, 6);
		} else if (c->workerPercentage >= 25) {
			DRAW_DESC_AT(72, 88, 7);
		} else {
			DRAW_DESC_AT(72, 88, 8);
		}
	}

	Widget_Panel_drawInnerPanel(c->xOffset + 16, c->yOffset + 136, c->widthBlocks - 2, 4);
	UI_BuildingInfo_drawEmploymentInfo(c, c->yOffset + 142);
}

void UI_BuildingInfo_drawFort(BuildingInfoContext *c)
{
	c->helpId = 87;
	PLAY_SOUND("wavs/fort.wav");
	Widget_Panel_drawOuterPanel(c->xOffset, c->yOffset, c->widthBlocks, c->heightBlocks);
	Widget_GameText_drawCentered(89, 0, c->xOffset, c->yOffset + 10, 16 * c->widthBlocks, Font_LargeBlack);

	if (Data_Formations[c->formationId].cursedByMars) {
		Widget_GameText_drawMultiline(89, 1,
			c->xOffset + 32, c->yOffset + 16 * c->heightBlocks - 158,
			16 * (c->widthBlocks - 4), Font_NormalBlack);
	} else {
		Widget_GameText_drawMultiline(89, 2,
			c->xOffset + 32, c->yOffset + 16 * c->heightBlocks - 158,
			16 * (c->widthBlocks - 4), Font_NormalBlack);
	}
}

void UI_BuildingInfo_drawGatehouse(BuildingInfoContext *c)
{
	c->helpId = 85;
	PLAY_SOUND("wavs/gatehouse.wav");
	Widget_Panel_drawOuterPanel(c->xOffset, c->yOffset, c->widthBlocks, c->heightBlocks);
	Widget_GameText_drawCentered(90, 0, c->xOffset, c->yOffset + 10, 16 * c->widthBlocks, Font_LargeBlack);

	Widget_GameText_drawMultiline(90, 1,
		c->xOffset + 32, c->yOffset + 16 * c->heightBlocks - 158,
		16 * (c->widthBlocks - 4), Font_NormalBlack);
}

void UI_BuildingInfo_drawTower(BuildingInfoContext *c)
{
	c->helpId = 85;
	PLAY_SOUND("wavs/tower.wav");
	Widget_Panel_drawOuterPanel(c->xOffset, c->yOffset, c->widthBlocks, c->heightBlocks);
	Widget_GameText_drawCentered(91, 0, c->xOffset, c->yOffset + 10, 16 * c->widthBlocks, Font_LargeBlack);

	if (!c->hasRoadAccess) {
		DRAW_DESC(69, 25);
	} else if (Data_Buildings[c->buildingId].numWorkers <= 0) {
		DRAW_DESC(91, 2);
	} else if (Data_Buildings[c->buildingId].walkerId) {
		DRAW_DESC(91, 3);
	} else {
		DRAW_DESC(91, 4);
	}
	Widget_Panel_drawInnerPanel(c->xOffset + 16, c->yOffset + 136, c->widthBlocks - 2, 4);
	UI_BuildingInfo_drawEmploymentInfo(c, c->yOffset + 142);
}

void UI_BuildingInfo_drawMilitaryAcademy(BuildingInfoContext *c)
{
	c->helpId = 88;
	PLAY_SOUND("wavs/mil_acad.wav");
	Widget_Panel_drawOuterPanel(c->xOffset, c->yOffset, c->widthBlocks, c->heightBlocks);
	Widget_GameText_drawCentered(135, 0, c->xOffset, c->yOffset + 10, 16 * c->widthBlocks, Font_LargeBlack);

	Data_Building *b = &Data_Buildings[c->buildingId];
	if (!c->hasRoadAccess) {
		DRAW_DESC(69, 25);
	} else if (b->numWorkers <= 0) {
		DRAW_DESC(135, 2);
	} else if (c->workerPercentage >= 100) {
		DRAW_DESC(135, 1);
	} else {
		DRAW_DESC(135, 3);
	}
	Widget_Panel_drawInnerPanel(c->xOffset + 16, c->yOffset + 136, c->widthBlocks - 2, 4);
	UI_BuildingInfo_drawEmploymentInfo(c, c->yOffset + 142);
}

void UI_BuildingInfo_drawBarracks(BuildingInfoContext *c)
{
	c->helpId = 37;
	PLAY_SOUND("wavs/barracks.wav");
	Widget_Panel_drawOuterPanel(c->xOffset, c->yOffset, c->widthBlocks, c->heightBlocks);
	Widget_GameText_drawCentered(136, 0, c->xOffset, c->yOffset + 10, 16 * c->widthBlocks, Font_LargeBlack);
	Graphics_drawImage(GraphicId(ID_Graphic_ResourceIcons) + Resource_Weapons,
		c->xOffset + 64, c->yOffset + 38);

	Data_Building *b = &Data_Buildings[c->buildingId];
	if (b->loadsStored < 1) {
		Widget_GameText_drawNumberWithDescription(8, 10, 0,
			c->xOffset + 92, c->yOffset + 44, Font_NormalBlack);
	} else {
		Widget_GameText_drawNumberWithDescription(8, 10, b->loadsStored,
			c->xOffset + 92, c->yOffset + 44, Font_NormalBlack);
	}

	if (!c->hasRoadAccess) {
		DRAW_DESC_AT(70, 69, 25);
	} else if (b->numWorkers <= 0) {
		DRAW_DESC_AT(70, 136, 3);
	} else if (!c->barracksSoldiersRequested) {
		DRAW_DESC_AT(70, 136, 4);
	} else {
		int offset = 0;
		if (b->loadsStored > 0) {
			offset = 4;
		}
		if (c->workerPercentage >= 100) {
			DRAW_DESC_AT(70, 136, 5 + offset);
		} else if (c->workerPercentage >= 66) {
			DRAW_DESC_AT(70, 136, 6 + offset);
		} else if (c->workerPercentage >= 33) {
			DRAW_DESC_AT(70, 136, 7 + offset);
		} else {
			DRAW_DESC_AT(70, 136, 8 + offset);
		}
	}
	Widget_Panel_drawInnerPanel(c->xOffset + 16, c->yOffset + 136, c->widthBlocks - 2, 4);
	UI_BuildingInfo_drawEmploymentInfo(c, c->yOffset + 142);
}

void UI_BuildingInfo_drawLegionInfo(BuildingInfoContext *c)
{
	int textId, groupId;
	struct Data_Formation *m = &Data_Formations[c->formationId];
	c->helpId = 87;
	Widget_Panel_drawOuterPanel(c->xOffset, c->yOffset,
		c->widthBlocks, c->heightBlocks);
	Widget_GameText_drawCentered(138, m->legionId,
		c->xOffset, c->yOffset + 10, 16 * c->widthBlocks, Font_LargeBlack);

	// standard icon at the top
	int graphicId = GraphicId(ID_Graphic_FortStandardIcons) + m->legionId;
	int iconHeight = GraphicHeight(graphicId);
	Graphics_drawImage(graphicId,
		c->xOffset + 16 + (40 - GraphicWidth(graphicId)) / 2,
		c->yOffset + 16);
	// standard flag
	graphicId = GraphicId(ID_Graphic_FortFlags);
	if (m->figureType == Figure_FortJavelin) {
		graphicId += 9;
	} else if (m->figureType == Figure_FortMounted) {
		graphicId += 18;
	}
	if (m->isHalted) {
		graphicId += 8;
	}
	int flagHeight = GraphicHeight(graphicId);
	Graphics_drawImage(graphicId,
		c->xOffset + 16 + (40 - GraphicWidth(graphicId)) / 2,
		c->yOffset + 16 + iconHeight);
	// standard pole and morale ball
	graphicId = GraphicId(ID_Graphic_FortStandardPole) + 20 - m->morale / 5;
	Graphics_drawImage(graphicId,
		c->xOffset + 16 + (40 - GraphicWidth(graphicId)) / 2,
		c->yOffset + 16 + iconHeight + flagHeight);

	// number of soldiers
	Widget_GameText_draw(138, 23, c->xOffset + 100, c->yOffset + 60, Font_NormalBlack);
	Widget_Text_drawNumber(m->numFigures, '@', " ",
		c->xOffset + 294, c->yOffset + 60, Font_NormalBlack);
	// health
	Widget_GameText_draw(138, 24, c->xOffset + 100, c->yOffset + 80, Font_NormalBlack);
	int health = Calc_getPercentage(m->totalDamage, m->maxTotalDamage);
	if (health <= 0) {
		textId = 26;
	} else if (health <= 20) {
		textId = 27;
	} else if (health <= 40) {
		textId = 28;
	} else if (health <= 55) {
		textId = 29;
	} else if (health <= 70) {
		textId = 30;
	} else if (health <= 90) {
		textId = 31;
	} else {
		textId = 32;
	}
	Widget_GameText_draw(138, textId, c->xOffset + 300, c->yOffset + 80, Font_NormalBlack);
	// military training
	Widget_GameText_draw(138, 25, c->xOffset + 100, c->yOffset + 100, Font_NormalBlack);
	Widget_GameText_draw(18, m->hasMilitaryTraining,
		c->xOffset + 300, c->yOffset + 100, Font_NormalBlack);
	// morale
	if (m->cursedByMars) {
		Widget_GameText_draw(138, 59, c->xOffset + 100, c->yOffset + 120, Font_NormalBlack);
	} else {
		Widget_GameText_draw(138, 36, c->xOffset + 100, c->yOffset + 120, Font_NormalBlack);
		Widget_GameText_draw(138, 37 + m->morale / 5,
			c->xOffset + 300, c->yOffset + 120, Font_NormalBlack);
	}
	if (m->numFigures) {
		// layout
		static const int offsetsLegionary[2][5] = {
			{0, 0, 2, 3, 4}, {0, 0, 3, 2, 4},
		};
		static const int offsetsOther[2][5] = {
			{5, 6, 2, 3, 4}, {6, 5, 3, 2, 4},
		};
		const int *offsets;
		int index = 0;
		if (Data_Settings_Map.orientation == Dir_6_Left ||
			Data_Settings_Map.orientation == Dir_2_Right) {
			index = 1;
		}
		if (m->figureType == Figure_FortLegionary) {
			offsets = offsetsLegionary[index];
		} else {
			offsets = offsetsOther[index];
		}
		for (int i = 5 - c->formationTypes; i < 5; i++) {
			Graphics_drawImage(GraphicId(ID_Graphic_FortFormations) + offsets[i],
				c->xOffset + 21 + 85 * i, c->yOffset + 141);
		}
		UI_BuildingInfo_drawLegionInfoForeground(c);
	} else {
		// no soldiers
		if (m->cursedByMars) {
			groupId = 89; textId = 1;
		} else if (Data_CityInfo_Buildings.barracks.working) {
			groupId = 138; textId = 10;
		} else {
			groupId = 138; textId = 11;
		}
		Widget_GameText_drawMultiline(groupId, textId,
			c->xOffset + 32, c->yOffset + 172,
			16 * (c->widthBlocks - 4), Font_NormalBlack);
	}
}

void UI_BuildingInfo_drawLegionInfoForeground(BuildingInfoContext *c)
{
	struct Data_Formation *m = &Data_Formations[c->formationId];
	if (!m->numFigures) {
		return;
	}
	for (int i = 5 - c->formationTypes; i < 5; i++) {
		int hasFocus = 0;
		if (focusButtonId) {
			if (focusButtonId - 1 == i) {
				hasFocus = 1;
			}
		} else if (m->figureType == Figure_FortLegionary) {
			if (i == 0 && m->layout == 5) {
				hasFocus = 1;
			} else if (i == 1 && m->layout == 0) {
				hasFocus = 1;
			} else if (i == 2 && m->layout == 1) {
				hasFocus = 1;
			} else if (i == 3 && m->layout == 2) {
				hasFocus = 1;
			} else if (i == 4 && m->layout == 6) {
				hasFocus = 1;
			}
		} else { // mounted/javelin
			if (i == 0 && m->layout == 3) {
				hasFocus = 1;
			} else if (i == 1 && m->layout == 4) {
				hasFocus = 1;
			} else if (i == 2 && m->layout == 1) {
				hasFocus = 1;
			} else if (i == 3 && m->layout == 2) {
				hasFocus = 1;
			} else if (i == 4 && m->layout == 6) {
				hasFocus = 0;
			}
		}
		Widget_Panel_drawButtonBorder(c->xOffset + 19 + 85 * i,
			c->yOffset + 139, 84, 84, hasFocus);
	}
	Widget_Panel_drawInnerPanel(c->xOffset + 16, c->yOffset + 230,
		c->widthBlocks - 2, 4);

	int titleId;
	int textId;
	switch (focusButtonId) {
		// single line or testudo
		case 1:
			if (m->figureType == Figure_FortLegionary) {
				titleId = 12;
				textId = m->hasMilitaryTraining ? 18 : 17;
			} else {
				titleId = 16;
				textId = 22;
			}
			break;
		case 2:
			if (m->figureType == Figure_FortLegionary) {
				titleId = 13;
				textId = m->hasMilitaryTraining ? 19 : 17;
			} else {
				titleId = 16;
				textId = 22;
			}
			break;
		// double line
		case 3:
		case 4:
			titleId = 14;
			textId = 20;
			break;
		case 5: // mop up
			titleId = 15;
			textId = 21;
			break;
		default:
			// no button selected: go for formation layout
			switch (m->layout) {
				case FormationLayout_SingleLine1:
				case FormationLayout_SingleLine2:
					titleId = 16;
					textId = 22;
					break;
				case FormationLayout_DoubleLine1:
				case FormationLayout_DoubleLine2:
					titleId = 14;
					textId = 20;
					break;
				case FormationLayout_Tortoise:
					titleId = 12;
					textId = 18;
					break;
				case FormationLayout_MopUp:
					titleId = 15;
					textId = 21;
					break;
				case FormationLayout_Column:
					titleId = 13;
					textId = 19;
					break;
			}
			break;
	}
	Widget_GameText_draw(138, titleId,
		c->xOffset + 24, c->yOffset + 236, Font_NormalWhite);
	Widget_GameText_drawMultiline(138, textId,
		c->xOffset + 24, c->yOffset + 252,
		16 * (c->widthBlocks - 4), Font_NormalGreen);

	if (!m->isAtFort) {
		Widget_Panel_drawButtonBorder(
			c->xOffset + 16 * (c->widthBlocks - 18) / 2,
			c->yOffset + 16 * c->heightBlocks - 48,
			288, 32, returnButtonId == 1);
		Widget_GameText_drawCentered(138, 58,
			c->xOffset + 16 * (c->widthBlocks - 18) / 2,
			c->yOffset + 16 * c->heightBlocks - 39,
			288, Font_NormalBlack);
	}
}

void UI_BuildingInfo_handleMouseLegionInfo(BuildingInfoContext *c)
{
	contextForCallback = c;
	if (Widget_Button_handleCustomButtons(
			c->xOffset, c->yOffset, layoutButtons, 5, &focusButtonId)) {
		struct Data_Formation *m = &Data_Formations[c->formationId];
		if (m->figureType == Figure_FortLegionary) {
			if (focusButtonId == 1 || (focusButtonId == 2 && c->formationTypes == 3)) {
				focusButtonId = 0;
			}
		}
	} else {
		Widget_Button_handleCustomButtons(
			c->xOffset + 16 * (c->widthBlocks - 18) / 2,
			c->yOffset + 16 * c->heightBlocks - 48,
			returnButtons, 1, &returnButtonId);
	}
	contextForCallback = 0;
}

int UI_BuildingInfo_getTooltipLegionInfo(BuildingInfoContext *c)
{
	return focusButtonId ? 147 : 0;
}

static void buttonReturnToFort(int param1, int param2)
{
	int formationId = contextForCallback->formationId;
	struct Data_Formation *m = &Data_Formations[formationId];
	if (!m->inDistantBattle && m->isAtFort != 1) {
		Formation_legionReturnHome(formationId);
		UI_Window_goTo(Window_City);
	}
}

static void buttonLayout(int index, int param2)
{
	struct Data_Formation *m = &Data_Formations[contextForCallback->formationId];
	if (m->inDistantBattle) {
		return;
	}
	if (index == 0 && contextForCallback->formationTypes < 5) {
		return;
	}
	if (index == 1 && contextForCallback->formationTypes < 4) {
		return;
	}
	// store layout in case of mop up
	if (index == 4 && m->layout != FormationLayout_MopUp) {
		m->layoutBeforeMopUp = m->layout;
	}
	if (m->figureType == Figure_FortLegionary) {
		switch (index) {
			case 0: m->layout = FormationLayout_Tortoise; break;
			case 1: m->layout = FormationLayout_Column; break;
			case 2: m->layout = FormationLayout_DoubleLine1; break;
			case 3: m->layout = FormationLayout_DoubleLine2; break;
			case 4: m->layout = FormationLayout_MopUp; break;
		}
	} else {
		switch (index) {
			case 0: m->layout = FormationLayout_SingleLine1; break;
			case 1: m->layout = FormationLayout_SingleLine2; break;
			case 2: m->layout = FormationLayout_DoubleLine1; break;
			case 3: m->layout = FormationLayout_DoubleLine2; break;
			case 4: m->layout = FormationLayout_MopUp; break;
		}
	}
	switch (index) {
		case 0: Sound_Speech_playFile("wavs/cohort1.wav"); break;
		case 1: Sound_Speech_playFile("wavs/cohort2.wav"); break;
		case 2: Sound_Speech_playFile("wavs/cohort3.wav"); break;
		case 3: Sound_Speech_playFile("wavs/cohort4.wav"); break;
		case 4: Sound_Speech_playFile("wavs/cohort5.wav"); break;
	}
	Data_State.selectedLegionFormationId = contextForCallback->formationId;
	UI_Window_goTo(Window_CityMilitary);
}
