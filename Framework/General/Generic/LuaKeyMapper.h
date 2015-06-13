//=========================================================================================================================
//LuaKeyMapper.h
//=========================================================================================================================
#ifndef __LUAKEYMAPPER_H
#define __LUAKEYMAPPER_H
//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
#include <string>
#include <map>
#include "Input.h"
using namespace std;
//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
#pragma region "LuaKeys"
static string luaKey[160] =
{
	"KEY_A",
	"KEY_ADD",
	"KEY_APPS",
	"KEY_ATTN",
	"KEY_B",
	"KEY_BACK",
	"KEY_BROWSERBACK",
	"KEY_BROWSERFAVORITES",
	"KEY_BROWSERFORWARD",
	"KEY_BROWSERHOME",
	"KEY_BROWSERREFRESH",
	"KEY_BROWSERSEARCH",
	"KEY_BROWSERSTOP",
	"KEY_C",
	"KEY_CAPSLOCK",
	"KEY_CHATPADGREEN",
	"KEY_CHATPADORANGE",
	"KEY_CRSEL",
	"KEY_D",
	"KEY_D0",
	"KEY_D1",
	"KEY_D2",
	"KEY_D3",
	"KEY_D4",
	"KEY_D5",
	"KEY_D6",
	"KEY_D7",
	"KEY_D8",
	"KEY_D9",
	"KEY_DECIMAL",
	"KEY_DELETE",
	"KEY_DIVIDE",
	"KEY_DOWN",
	"KEY_E",
	"KEY_END",
	"KEY_ENTER",
	"KEY_ERASEEOF",
	"KEY_ESCAPE",
	"KEY_EXECUTE",
	"KEY_EXSEL",
	"KEY_F",
	"KEY_F1",
	"KEY_F10",
	"KEY_F11",
	"KEY_F12",
	"KEY_F13",
	"KEY_F14",
	"KEY_F15",
	"KEY_F16",
	"KEY_F17",
	"KEY_F18",
	"KEY_F19",
	"KEY_F2",
	"KEY_F20",
	"KEY_F21",
	"KEY_F22",
	"KEY_F23",
	"KEY_F24",
	"KEY_F3",
	"KEY_F4",
	"KEY_F5",
	"KEY_F6",
	"KEY_F7",
	"KEY_F8",
	"KEY_F9",
	"KEY_G",
	"KEY_H",
	"KEY_HELP",
	"KEY_HOME",
	"KEY_I",
	"KEY_IMECONVERT",
	"KEY_IMENOCONVERT",
	"KEY_INSERT",
	"KEY_J",
	"KEY_K",
	"KEY_KANA",
	"KEY_KANJI",
	"KEY_L",
	"KEY_LAUNCHAPPLICATION1",
	"KEY_LAUNCHAPPLICATION2",
	"KEY_LAUCHMAIL",
	"KEY_LEFT",
	"KEY_LEFTALT",
	"KEY_LEFTCONTROL",
	"KEY_LEFTSHIFT",
	"KEY_LEFTWINDOWS",
	"KEY_M",
	"KEY_MEDIANEXTTRACK",
	"KEY_MEDIAPLAYPAUSE",
	"KEY_MEDIAPREVIOUSTRACK",
	"KEY_MEDIASTOP",
	"KEY_MULTIPLY",
	"KEY_N",
	"KEY_NONE",
	"KEY_NUMLOCK",
	"KEY_NUMPAD0",
	"KEY_NUMPAD1",
	"KEY_NUMPAD2",
	"KEY_NUMPAD3",
	"KEY_NUMPAD4",
	"KEY_NUMPAD5",
	"KEY_NUMPAD6",
	"KEY_NUMPAD7",
	"KEY_NUMPAD8",
	"KEY_NUMPAD9",
	"KEY_O",
	"KEY_OEM8",
	"KEY_OEMAUTO",
	"KEY_OEMBACKSLASH",
	"KEY_OEMCLEAR",
	"KEY_OEMCLOSEBRACKETS",
	"KEY_OEMCOMMA",
	"KEY_OEMCOPY",
	"KEY_OEMENLW",
	"KEY_OEMMINUS",
	"KEY_OEMOPENBRACKETS",
	"KEY_OEMPERIOD",
	"KEY_OEMPLUS",
	"KEY_OEMPIPE",
	"KEY_OEMQUESTION",
	"KEY_OEMQUOTES",
	"KEY_OEMSEMICOLON",
	"KEY_OEMTILDE",
	"KEY_P",
	"KEY_PA1",
	"KEY_PAGEDOWN",
	"KEY_PAGEUP",
	"KEY_PAUSE",
	"KEY_PLAY",
	"KEY_PRINT",
	"KEY_PRINTSCREEN",
	"KEY_PROCESSKEY",
	"KEY_Q",
	"KEY_R",
	"KEY_RIGHT",
	"KEY_RIGHTALT",
	"KEY_RIGHTCONTROL",
	"KEY_RIGHTSHIFT",
	"KEY_RIGHTWINDOWS",
	"KEY_S",
	"KEY_SCROLL",
	"KEY_SELECT",
	"KEY_SELECTMEDIA",
	"KEY_SEPARATOR",
	"KEY_SLEEP",
	"KEY_SPACE",
	"KEY_SUBTRACT",
	"KEY_T",
	"KEY_TAB",
	"KEY_U",
	"KEY_UP",
	"KEY_V",
	"KEY_VOLUMEDOWN",
	"KEY_VOLUMEMUTE",
	"KEY_VOLUMEUP",
	"KEY_W",
	"KEY_X",
	"KEY_Y",
	"KEY_Z",
	"KEY_ZOOM"

	/*"KEY_0",
	"KEY_1",
	"KEY_2",
	"KEY_3",
	"KEY_4",
	"KEY_5",
	"KEY_6",
	"KEY_7",
	"KEY_8",
	"KEY_9",

	"KEY_A",
	"KEY_B",
	"KEY_C",
	"KEY_D",
	"KEY_E",
	"KEY_F",
	"KEY_G",
	"KEY_H",
	"KEY_I",
	"KEY_J",
	"KEY_K",
	"KEY_L",
	"KEY_M",
	"KEY_N",
	"KEY_O",
	"KEY_P",
	"KEY_Q",
	"KEY_R",
	"KEY_S",
	"KEY_T",
	"KEY_U",
	"KEY_V",
	"KEY_W",
	"KEY_X",
	"KEY_Y",
	"KEY_Z",

	"KEY_F1",
	"KEY_F2",
	"KEY_F3",
	"KEY_F4",
	"KEY_F5",
	"KEY_F6",
	"KEY_F7",
	"KEY_F8",
	"KEY_F9",
	"KEY_F10",
	"KEY_F11",
	"KEY_F12",

	"KEY_NUMPAD_0",
	"KEY_NUMPAD_1",
	"KEY_NUMPAD_2",
	"KEY_NUMPAD_3",
	"KEY_NUMPAD_4",
	"KEY_NUMPAD_5",
	"KEY_NUMPAD_6",
	"KEY_NUMPAD_7",
	"KEY_NUMPAD_8",
	"KEY_NUMPAD_9",
	"KEY_NUMPAD_ADD",
	"KEY_NUMPAD_DIVIDE",
	"KEY_NUMPAD_ENTER",
	"KEY_NUMPAD_MINUS",
	"KEY_NUMPAD_MULTIPLY",
	"KEY_NUMPAD_NUMLOCK",
	"KEY_NUMPAD_PERIOD",

	"KEY_ADD",
	"KEY_APOSTROPHE",
	"KEY_APPS",
	"KEY_BACKSLASH",
	"KEY_BACKSPACE",
	"KEY_CAPSLOCK",
	"KEY_COMMA",
	"KEY_ENTER",
	"KEY_ESCAPE",
	"KEY_MINUS",
	"KEY_PAUSE",
	"KEY_PERIOD",
	"KEY_PRINTSCREEN",
	"KEY_SCROLLLOCK",
	"KEY_SEMICOLON",
	"KEY_SLASH",
	"KEY_SPACE",
	"KEY_TAB",
	"KEY_TILDE",

	"KEY_LALT",
	"KEY_LBRACKET",
	"KEY_LCONTROL",
	"KEY_LSHIFT",
	"KEY_LWIN",
	"KEY_RALT",
	"KEY_RBRACKET",
	"KEY_RCONTROL",
	"KEY_RSHIFT",
	"KEY_RWIN",

	"KEY_DOWN",
	"KEY_LEFT",
	"KEY_RIGHT",
	"KEY_UP",

	"KEY_DELETE",
	"KEY_END",
	"KEY_HOME",
	"KEY_INSERT",
	"KEY_PAGEDOWN",
	"KEY_PAGEUP",

	"KEY_CALCULATOR",
	"KEY_MAIL",
	"KEY_MEDIASELECT",
	"KEY_MEDIASTOP",
	"KEY_MUTE",
	"KEY_MYCOMPUTER",
	"KEY_NEXTTRACK",
	"KEY_PLAYPAUSE",
	"KEY_POWER",
	"KEY_PREVTRACK",
	"KEY_SLEEP",
	"KEY_VOLUMEDOWN",
	"KEY_VOLUMEUP",
	"KEY_WAKE",
	"KEY_WEBBACK",
	"KEY_WEBFAVORITES",
	"KEY_WEBFORWARD",
	"KEY_WEBHOME",
	"KEY_WEBREFRESH",
	"KEY_WEBSEARCH",
	"KEY_WEBSTOP"*/
};
//-------------------------------------------------------------------------------------------------------------------------
/*static int Key[156] =
{
	0x41,
	0x6b,
	0x5d,
	0xf6,
	0x42,
	8,
	0xa6,
	0xab,
	0xa7,
	0xac,
	0xa8,
	170,
	0xa9,
	0x43,
	20,
	0xca,
	0xcb,
	0xf7,
	0x44,
	0x30,
	0x31,
	50,
	0x33,
	0x34,
	0x35,
	0x36,
	0x37,
	0x38,
	0x39,
	110,
	Delete = 0x2e,
	Divide = 0x6f,
	Down = 40,
	E = 0x45,
	End = 0x23,
	Enter = 13,
	EraseEof = 0xf9,
	Escape = 0x1b,
	Execute = 0x2b,
	Exsel = 0xf8,
	F = 70,
	F1 = 0x70,
	F10 = 0x79,
	F11 = 0x7a,
	F12 = 0x7b,
	F13 = 0x7c,
	F14 = 0x7d,
	F15 = 0x7e,
	F16 = 0x7f,
	F17 = 0x80,
	F18 = 0x81,
	F19 = 130,
	F2 = 0x71,
	F20 = 0x83,
	F21 = 0x84,
	F22 = 0x85,
	F23 = 0x86,
	F24 = 0x87,
	F3 = 0x72,
	F4 = 0x73,
	F5 = 0x74,
	F6 = 0x75,
	F7 = 0x76,
	F8 = 0x77,
	F9 = 120,
	G = 0x47,
	H = 0x48,
	Help = 0x2f,
	Home = 0x24,
	I = 0x49,
	ImeConvert = 0x1c,
	ImeNoConvert = 0x1d,
	Insert = 0x2d,
	J = 0x4a,
	K = 0x4b,
	Kana = 0x15,
	Kanji = 0x19,
	L = 0x4c,
	LaunchApplication1 = 0xb6,
	LaunchApplication2 = 0xb7,
	LaunchMail = 180,
	Left = 0x25,
	LeftAlt = 0xa4,
	LeftControl = 0xa2,
	LeftShift = 160,
	LeftWindows = 0x5b,
	M = 0x4d,
	MediaNextTrack = 0xb0,
	MediaPlayPause = 0xb3,
	MediaPreviousTrack = 0xb1,
	MediaStop = 0xb2,
	Multiply = 0x6a,
	N = 0x4e,
	None = 0,
	NumLock = 0x90,
	NumPad0 = 0x60,
	NumPad1 = 0x61,
	NumPad2 = 0x62,
	NumPad3 = 0x63,
	NumPad4 = 100,
	NumPad5 = 0x65,
	NumPad6 = 0x66,
	NumPad7 = 0x67,
	NumPad8 = 0x68,
	NumPad9 = 0x69,
	O = 0x4f,
	Oem8 = 0xdf,
	OemAuto = 0xf3,
	OemBackslash = 0xe2,
	OemClear = 0xfe,
	OemCloseBrackets = 0xdd,
	OemComma = 0xbc,
	OemCopy = 0xf2,
	OemEnlW = 0xf4,
	OemMinus = 0xbd,
	OemOpenBrackets = 0xdb,
	OemPeriod = 190,
	OemPipe = 220,
	OemPlus = 0xbb,
	OemQuestion = 0xbf,
	OemQuotes = 0xde,
	OemSemicolon = 0xba,
	OemTilde = 0xc0,
	P = 80,
	Pa1 = 0xfd,
	PageDown = 0x22,
	PageUp = 0x21,
	Pause = 0x13,
	Play = 250,
	Print = 0x2a,
	PrintScreen = 0x2c,
	ProcessKey = 0xe5,
	Q = 0x51,
	R = 0x52,
	Right = 0x27,
	RightAlt = 0xa5,
	RightControl = 0xa3,
	RightShift = 0xa1,
	RightWindows = 0x5c,
	S = 0x53,
	Scroll = 0x91,
	Select = 0x29,
	SelectMedia = 0xb5,
	Separator = 0x6c,
	Sleep = 0x5f,
	Space = 0x20,
	Subtract = 0x6d,
	T = 0x54,
	Tab = 9,
	U = 0x55,
	Up = 0x26,
	V = 0x56,
	VolumeDown = 0xae,
	VolumeMute = 0xad,
	VolumeUp = 0xaf,
	W = 0x57,
	X = 0x58,
	Y = 0x59,
	Z = 90,
	Zoom = 0xfb
};*/
#pragma endregion
//-------------------------------------------------------------------------------------------------------------------------
class LuaKeyMapper
{
public:
	LuaKeyMapper();
	LuaKeyMapper(const LuaKeyMapper& o);
	~LuaKeyMapper();

	Keyboard::Key get_key(string key);

	static void NewInstance() { instance = new LuaKeyMapper(); }
	static LuaKeyMapper* GetInstance()
	{
		if (instance == NULL)
			instance = new LuaKeyMapper();
		return instance;
	}

private:
	static LuaKeyMapper* instance;

	map<string, Keyboard::Key> m_keys;
};
//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
#endif//__LUAKEYMAPPER_H