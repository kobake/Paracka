#pragma once

#include "id/menu.h"

#define ICO_MAIN	100
#define WAVE_OK		101
#define WAVE_NG		102
#define WAVE_CLEAR	103

#define IDM_FILE_BEGIN	(IDM_BEGIN+0)
#define IDM_FILE_OPEN	(IDM_FILE_BEGIN+0)
#define IDM_FILE_RELOAD	(IDM_FILE_BEGIN+1)
#define IDM_FILE_QUIT	(IDM_FILE_BEGIN+2)
#define IDM_FILE_END	(IDM_FILE_BEGIN+3)

#define IDM_FUNC_BEGIN	(IDM_FILE_END+0)
#define IDM_FUNC_ENTER	IDOK
#define IDM_FUNC_START	(IDM_FUNC_BEGIN+1)
#define IDM_FUNC_PASS	(IDM_FUNC_BEGIN+2)
#define IDM_FUNC_VIEW	(IDM_FUNC_BEGIN+3)
#define IDM_FUNC_TURN	(IDM_FUNC_BEGIN+4)
#define IDM_FUNC_END	(IDM_FUNC_BEGIN+5)

#define IDM_HELP_BEGIN	(IDM_FUNC_END+0)
#define IDM_HELP_TOPIC	(IDM_HELP_BEGIN+0)
#define IDM_HELP_VER	(IDM_HELP_BEGIN+1)
#define IDM_HELP_END	(IDM_HELP_BEGIN+2)
