/**
 * Defines keyboard scancodes.
 * Mirrors the values in SDL_scancode.h, please refer to SDL documentation.
 */

#ifndef SCANCODE_H
#define SCANCODE_H

typedef enum {
    ASL_SCANCODE_UNKNOWN = 0,

    ASL_SCANCODE_A = 4,
    ASL_SCANCODE_B = 5,
    ASL_SCANCODE_C = 6,
    ASL_SCANCODE_D = 7,
    ASL_SCANCODE_E = 8,
    ASL_SCANCODE_F = 9,
    ASL_SCANCODE_G = 10,
    ASL_SCANCODE_H = 11,
    ASL_SCANCODE_I = 12,
    ASL_SCANCODE_J = 13,
    ASL_SCANCODE_K = 14,
    ASL_SCANCODE_L = 15,
    ASL_SCANCODE_M = 16,
    ASL_SCANCODE_N = 17,
    ASL_SCANCODE_O = 18,
    ASL_SCANCODE_P = 19,
    ASL_SCANCODE_Q = 20,
    ASL_SCANCODE_R = 21,
    ASL_SCANCODE_S = 22,
    ASL_SCANCODE_T = 23,
    ASL_SCANCODE_U = 24,
    ASL_SCANCODE_V = 25,
    ASL_SCANCODE_W = 26,
    ASL_SCANCODE_X = 27,
    ASL_SCANCODE_Y = 28,
    ASL_SCANCODE_Z = 29,

    ASL_SCANCODE_1 = 30,
    ASL_SCANCODE_2 = 31,
    ASL_SCANCODE_3 = 32,
    ASL_SCANCODE_4 = 33,
    ASL_SCANCODE_5 = 34,
    ASL_SCANCODE_6 = 35,
    ASL_SCANCODE_7 = 36,
    ASL_SCANCODE_8 = 37,
    ASL_SCANCODE_9 = 38,
    ASL_SCANCODE_0 = 39,

    ASL_SCANCODE_RETURN = 40,
    ASL_SCANCODE_ESCAPE = 41,
    ASL_SCANCODE_BACKSPACE = 42,
    ASL_SCANCODE_TAB = 43,
    ASL_SCANCODE_SPACE = 44,

    ASL_SCANCODE_MINUS = 45,
    ASL_SCANCODE_EQUALS = 46,
    ASL_SCANCODE_LEFTBRACKET = 47,
    ASL_SCANCODE_RIGHTBRACKET = 48,

    /* Located at the lower left of the return
     * key on ISO keyboards and at the right end
     * of the QWERTY row on ANSI keyboards.
     * Produces REVERSE SOLIDUS (backslash) and
     * VERTICAL LINE in a US layout, REVERSE
     * SOLIDUS and VERTICAL LINE in a UK Mac
     * layout, NUMBER SIGN and TILDE in a UK
     * Windows layout, DOLLAR SIGN and POUND SIGN
     * in a Swiss German layout, NUMBER SIGN and
     * APOSTROPHE in a German layout, GRAVE
     * ACCENT and POUND SIGN in a French Mac
     * layout, and ASTERISK and MICRO SIGN in a
     * French Windows layout.
     */
    ASL_SCANCODE_BACKSLASH = 49,

    /* ISO USB keyboards actually use this code
     * instead of 49 for the same key, but all
     * OSes I've seen treat the two codes
     * identically. So, as an implementor, unless
     * your keyboard generates both of those
     * codes and your OS treats them differently,
     * you should generate ASL_SCANCODE_BACKSLASH
     * instead of this code. As a user, you
     * should not rely on this code because ASL
     * will never generate it with most (all?)
     * keyboards.
     */
    ASL_SCANCODE_NONUSHASH = 50,

    ASL_SCANCODE_SEMICOLON = 51,
    ASL_SCANCODE_APOSTROPHE = 52,

    /* Located in the top left corner (on both ANSI
     * and ISO keyboards). Produces GRAVE ACCENT and
     * TILDE in a US Windows layout and in US and UK
     * Mac layouts on ANSI keyboards, GRAVE ACCENT
     * and NOT SIGN in a UK Windows layout, SECTION
     * SIGN and PLUS-MINUS SIGN in US and UK Mac
     * layouts on ISO keyboards, SECTION SIGN and
     * DEGREE SIGN in a Swiss German layout (Mac:
     * only on ISO keyboards), CIRCUMFLEX ACCENT and
     * DEGREE SIGN in a German layout (Mac: only on
     * ISO keyboards), SUPERSCRIPT TWO and TILDE in a
     * French Windows layout, COMMERCIAL AT and
     * NUMBER SIGN in a French Mac layout on ISO
     * keyboards, and LESS-THAN SIGN and GREATER-THAN
     * SIGN in a Swiss German, German, or French Mac
     * layout on ANSI keyboards.
     */
    ASL_SCANCODE_GRAVE = 53,

    ASL_SCANCODE_COMMA = 54,
    ASL_SCANCODE_PERIOD = 55,
    ASL_SCANCODE_SLASH = 56,

    ASL_SCANCODE_CAPSLOCK = 57,

    ASL_SCANCODE_F1 = 58,
    ASL_SCANCODE_F2 = 59,
    ASL_SCANCODE_F3 = 60,
    ASL_SCANCODE_F4 = 61,
    ASL_SCANCODE_F5 = 62,
    ASL_SCANCODE_F6 = 63,
    ASL_SCANCODE_F7 = 64,
    ASL_SCANCODE_F8 = 65,
    ASL_SCANCODE_F9 = 66,
    ASL_SCANCODE_F10 = 67,
    ASL_SCANCODE_F11 = 68,
    ASL_SCANCODE_F12 = 69,

    ASL_SCANCODE_PRINTSCREEN = 70,
    ASL_SCANCODE_SCROLLLOCK = 71,
    ASL_SCANCODE_PAUSE = 72,

    /* insert on PC, help on some Mac keyboards (but does send code 73, not 117) */
    ASL_SCANCODE_INSERT = 73,

    ASL_SCANCODE_HOME = 74,
    ASL_SCANCODE_PAGEUP = 75,
    ASL_SCANCODE_DELETE = 76,
    ASL_SCANCODE_END = 77,
    ASL_SCANCODE_PAGEDOWN = 78,
    ASL_SCANCODE_RIGHT = 79,
    ASL_SCANCODE_LEFT = 80,
    ASL_SCANCODE_DOWN = 81,
    ASL_SCANCODE_UP = 82,

    /* num lock on PC, clear on Mac keyboards */
    ASL_SCANCODE_NUMLOCKCLEAR = 83,

    ASL_SCANCODE_KP_DIVIDE = 84,
    ASL_SCANCODE_KP_MULTIPLY = 85,
    ASL_SCANCODE_KP_MINUS = 86,
    ASL_SCANCODE_KP_PLUS = 87,
    ASL_SCANCODE_KP_ENTER = 88,
    ASL_SCANCODE_KP_1 = 89,
    ASL_SCANCODE_KP_2 = 90,
    ASL_SCANCODE_KP_3 = 91,
    ASL_SCANCODE_KP_4 = 92,
    ASL_SCANCODE_KP_5 = 93,
    ASL_SCANCODE_KP_6 = 94,
    ASL_SCANCODE_KP_7 = 95,
    ASL_SCANCODE_KP_8 = 96,
    ASL_SCANCODE_KP_9 = 97,
    ASL_SCANCODE_KP_0 = 98,
    ASL_SCANCODE_KP_PERIOD = 99,

    /* This is the additional key that ISO
     * keyboards have over ANSI ones,
     * located between left shift and Y.
     * Produces GRAVE ACCENT and TILDE in a
     * US or UK Mac layout, REVERSE SOLIDUS
     * (backslash) and VERTICAL LINE in a
     * US or UK Windows layout, and
     * LESS-THAN SIGN and GREATER-THAN SIGN
     * in a Swiss German, German, or French
     * layout. */
    ASL_SCANCODE_NONUSBACKSLASH = 100,

    /* windows contextual menu, compose */
    ASL_SCANCODE_APPLICATION = 101,

    /* The USB document says this is a status flag,
     * not a physical key - but some Mac keyboards
     * do have a power key. */
    ASL_SCANCODE_POWER = 102,

    ASL_SCANCODE_KP_EQUALS = 103,
    ASL_SCANCODE_F13 = 104,
    ASL_SCANCODE_F14 = 105,
    ASL_SCANCODE_F15 = 106,
    ASL_SCANCODE_F16 = 107,
    ASL_SCANCODE_F17 = 108,
    ASL_SCANCODE_F18 = 109,
    ASL_SCANCODE_F19 = 110,
    ASL_SCANCODE_F20 = 111,
    ASL_SCANCODE_F21 = 112,
    ASL_SCANCODE_F22 = 113,
    ASL_SCANCODE_F23 = 114,
    ASL_SCANCODE_F24 = 115,
    ASL_SCANCODE_EXECUTE = 116,
    ASL_SCANCODE_HELP = 117,
    ASL_SCANCODE_MENU = 118,
    ASL_SCANCODE_SELECT = 119,
    ASL_SCANCODE_STOP = 120,
    ASL_SCANCODE_AGAIN = 121, /* redo */
    ASL_SCANCODE_UNDO = 122,
    ASL_SCANCODE_CUT = 123,
    ASL_SCANCODE_COPY = 124,
    ASL_SCANCODE_PASTE = 125,
    ASL_SCANCODE_FIND = 126,
    ASL_SCANCODE_MUTE = 127,
    ASL_SCANCODE_VOLUMEUP = 128,
    ASL_SCANCODE_VOLUMEDOWN = 129,

    /* not sure whether there's a reason to enable these */
    /*     ASL_SCANCODE_LOCKINGCAPSLOCK = 130,  */
    /*     ASL_SCANCODE_LOCKINGNUMLOCK = 131, */
    /*     ASL_SCANCODE_LOCKINGSCROLLLOCK = 132, */
    ASL_SCANCODE_KP_COMMA = 133,

    ASL_SCANCODE_KP_EQUALSAS400 = 134,

    /* used on Asian keyboards, see
     * footnotes in USB doc */
    ASL_SCANCODE_INTERNATIONAL1 = 135,

    ASL_SCANCODE_INTERNATIONAL2 = 136,
    ASL_SCANCODE_INTERNATIONAL3 = 137, /* Yen */
    ASL_SCANCODE_INTERNATIONAL4 = 138,
    ASL_SCANCODE_INTERNATIONAL5 = 139,
    ASL_SCANCODE_INTERNATIONAL6 = 140,
    ASL_SCANCODE_INTERNATIONAL7 = 141,
    ASL_SCANCODE_INTERNATIONAL8 = 142,
    ASL_SCANCODE_INTERNATIONAL9 = 143,
    ASL_SCANCODE_LANG1 = 144, /* Hangul/English toggle */
    ASL_SCANCODE_LANG2 = 145, /* Hanja conversion */
    ASL_SCANCODE_LANG3 = 146, /* Katakana */
    ASL_SCANCODE_LANG4 = 147, /* Hiragana */
    ASL_SCANCODE_LANG5 = 148, /* Zenkaku/Hankaku */
    ASL_SCANCODE_LANG6 = 149, /* reserved */
    ASL_SCANCODE_LANG7 = 150, /* reserved */
    ASL_SCANCODE_LANG8 = 151, /* reserved */
    ASL_SCANCODE_LANG9 = 152, /* reserved */

    ASL_SCANCODE_ALTERASE = 153, /* Erase-Eaze */
    ASL_SCANCODE_SYSREQ = 154,
    ASL_SCANCODE_CANCEL = 155,
    ASL_SCANCODE_CLEAR = 156,
    ASL_SCANCODE_PRIOR = 157,
    ASL_SCANCODE_RETURN2 = 158,
    ASL_SCANCODE_SEPARATOR = 159,
    ASL_SCANCODE_OUT = 160,
    ASL_SCANCODE_OPER = 161,
    ASL_SCANCODE_CLEARAGAIN = 162,
    ASL_SCANCODE_CRSEL = 163,
    ASL_SCANCODE_EXSEL = 164,

    ASL_SCANCODE_KP_00 = 176,
    ASL_SCANCODE_KP_000 = 177,
    ASL_SCANCODE_THOUSANDSSEPARATOR = 178,
    ASL_SCANCODE_DECIMALSEPARATOR = 179,
    ASL_SCANCODE_CURRENCYUNIT = 180,
    ASL_SCANCODE_CURRENCYSUBUNIT = 181,
    ASL_SCANCODE_KP_LEFTPAREN = 182,
    ASL_SCANCODE_KP_RIGHTPAREN = 183,
    ASL_SCANCODE_KP_LEFTBRACE = 184,
    ASL_SCANCODE_KP_RIGHTBRACE = 185,
    ASL_SCANCODE_KP_TAB = 186,
    ASL_SCANCODE_KP_BACKSPACE = 187,
    ASL_SCANCODE_KP_A = 188,
    ASL_SCANCODE_KP_B = 189,
    ASL_SCANCODE_KP_C = 190,
    ASL_SCANCODE_KP_D = 191,
    ASL_SCANCODE_KP_E = 192,
    ASL_SCANCODE_KP_F = 193,
    ASL_SCANCODE_KP_XOR = 194,
    ASL_SCANCODE_KP_POWER = 195,
    ASL_SCANCODE_KP_PERCENT = 196,
    ASL_SCANCODE_KP_LESS = 197,
    ASL_SCANCODE_KP_GREATER = 198,
    ASL_SCANCODE_KP_AMPERSAND = 199,
    ASL_SCANCODE_KP_DBLAMPERSAND = 200,
    ASL_SCANCODE_KP_VERTICALBAR = 201,
    ASL_SCANCODE_KP_DBLVERTICALBAR = 202,
    ASL_SCANCODE_KP_COLON = 203,
    ASL_SCANCODE_KP_HASH = 204,
    ASL_SCANCODE_KP_SPACE = 205,
    ASL_SCANCODE_KP_AT = 206,
    ASL_SCANCODE_KP_EXCLAM = 207,
    ASL_SCANCODE_KP_MEMSTORE = 208,
    ASL_SCANCODE_KP_MEMRECALL = 209,
    ASL_SCANCODE_KP_MEMCLEAR = 210,
    ASL_SCANCODE_KP_MEMADD = 211,
    ASL_SCANCODE_KP_MEMSUBTRACT = 212,
    ASL_SCANCODE_KP_MEMMULTIPLY = 213,
    ASL_SCANCODE_KP_MEMDIVIDE = 214,
    ASL_SCANCODE_KP_PLUSMINUS = 215,
    ASL_SCANCODE_KP_CLEAR = 216,
    ASL_SCANCODE_KP_CLEARENTRY = 217,
    ASL_SCANCODE_KP_BINARY = 218,
    ASL_SCANCODE_KP_OCTAL = 219,
    ASL_SCANCODE_KP_DECIMAL = 220,
    ASL_SCANCODE_KP_HEXADECIMAL = 221,

    ASL_SCANCODE_LCTRL = 224,
    ASL_SCANCODE_LSHIFT = 225,
    ASL_SCANCODE_LALT = 226, /* alt, option */
    ASL_SCANCODE_LGUI = 227, /* windows, command (apple), meta */
    ASL_SCANCODE_RCTRL = 228,
    ASL_SCANCODE_RSHIFT = 229,
    ASL_SCANCODE_RALT = 230, /* alt gr, option */
    ASL_SCANCODE_RGUI = 231, /* windows, command (apple), meta */

    /* I'm not sure if this is really not covered
     * by any of the above, but since there's a
     * special KMOD_MODE for it I'm adding it here
     */
    ASL_SCANCODE_MODE = 257,

    ASL_SCANCODE_AUDIONEXT = 258,
    ASL_SCANCODE_AUDIOPREV = 259,
    ASL_SCANCODE_AUDIOSTOP = 260,
    ASL_SCANCODE_AUDIOPLAY = 261,
    ASL_SCANCODE_AUDIOMUTE = 262,
    ASL_SCANCODE_MEDIASELECT = 263,
    ASL_SCANCODE_WWW = 264,
    ASL_SCANCODE_MAIL = 265,
    ASL_SCANCODE_CALCULATOR = 266,
    ASL_SCANCODE_COMPUTER = 267,
    ASL_SCANCODE_AC_SEARCH = 268,
    ASL_SCANCODE_AC_HOME = 269,
    ASL_SCANCODE_AC_BACK = 270,
    ASL_SCANCODE_AC_FORWARD = 271,
    ASL_SCANCODE_AC_STOP = 272,
    ASL_SCANCODE_AC_REFRESH = 273,
    ASL_SCANCODE_AC_BOOKMARKS = 274,

    ASL_SCANCODE_BRIGHTNESSDOWN = 275,
    ASL_SCANCODE_BRIGHTNESSUP = 276,

    /* display mirroring/dual display
     * switch, video mode switch
     */
    ASL_SCANCODE_DISPLAYSWITCH = 277,

    ASL_SCANCODE_KBDILLUMTOGGLE = 278,
    ASL_SCANCODE_KBDILLUMDOWN = 279,
    ASL_SCANCODE_KBDILLUMUP = 280,
    ASL_SCANCODE_EJECT = 281,
    ASL_SCANCODE_SLEEP = 282,

    ASL_SCANCODE_APP1 = 283,
    ASL_SCANCODE_APP2 = 284,

    /* Walther keys */

    /* Add any other keys here. */

    /* not a key, just marks the number of scancodes
     * for array bounds
     */
    ASL_NUM_SCANCODES = 512
} AslScancode;

#endif
