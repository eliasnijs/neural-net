////////////////////////////////////////////////////////////////////////////////
//// NOTE(Elias): General anscii codes

#define ANSI_BEL				"\a"
#define ANSI_BS					"\b"
#define ANSI_HT					"\t"
#define ANSI_LF					"\n"
#define ANSI_VT					"\v"
#define ANSI_FF					"\f"
#define ANSI_CR					"\r"
#define ANSI_ESC				"\033"


////////////////////////////////////////////////////////////////////////////////
//// NOTE(Elias): Cursor controls

#define ANSI_ESC_TO_POS				ANSI_ESC "[H"
#define ANSI_ESC_TO_LC(l,c)			ANSI_ESC "\[" #l ";" #c "f"
#define ANSI_ESC_N_UP(n) 			ANSI_ESC "\[" #n "A"
#define ANSI_ESC_N_DOWN(n)			ANSI_ESC "\[" #n "B"
#define ANSI_ESC_N_RIGHT(n)			ANSI_ESC "\[" #n "C"
#define ANSI_ESC_N_LEFT(n)			ANSI_ESC "\[" #n "D"
#define ANSI_ESC_START_OF_N_DOWN(n)		ANSI_ESC "\[" #n "E"
#define ANSI_ESC_START_OF_N_UP(n)		ANSI_ESC "\[" #n "F"
#define ANSI_ESC_TO_COL_N(n)			ANSI_ESC "\[" #n "G"
#define ANSI_ESC_REQ_CURSOR_POS(l,c)		ANSI_ESC "\[" #l ";" #c "R"
#define ANSI_ESC_UP_W_SCROLL			ANSI_ESC "[M"
#define ANSI_ESC_SAVE_CURSOR_POS_DEC		ANSI_ESC "[7"
#define ANSI_ESC_SAVE_CURSOR_POS_SCO		ANSI_ESC "[7"
#define ANSI_ESC_RESTORE_LAST_CURSOR_POS_DEC	ANSI_ESC "[8"
#define ANSI_ESC_RESTORE_LAST_CURSOR_POS_SCO	ANSI_ESC "[8"


////////////////////////////////////////////////////////////////////////////////
//// NOTE(Elias): Erase functions

#define ANSI_ESC_ERASE_TO_SCREEN_END		ANSI_ESC "[0J"
#define ANSI_ESC_ERASE_FROM_SCREEN_START	ANSI_ESC "[1J"
#define ANSI_ESC_ERASE_SCREEN			ANSI_ESC "[2J"
#define ANSI_ESC_ERASE_SAVED			ANSI_ESC "[3J"
#define ANSI_ESC_ERASE_IN_LINE			ANSI_ESC "[K"
#define ANSI_ESC_ERASE_TO_END			ANSI_ESC "[0K"
#define ANSI_ESC_ERASE_FROM_START		ANSI_ESC "[1K"
#define ANSI_ESC_ERASE_LINE			ANSI_ESC "[2K"


////////////////////////////////////////////////////////////////////////////////
//// NOTE(Elias): Text decoration

#define ANSI_ESC_RESET_ALL			ANSI_ESC "[0m"
#define ANSI_ESC_BOLD				ANSI_ESC "[1m"
#define ANSI_ESC_BOLD_RESET			ANSI_ESC "[21m"
#define ANSI_ESC_DIM				ANSI_ESC "[2m"
#define ANSI_ESC_DIM_RESET			ANSI_ESC "[22m"
#define ANSI_ESC_ITALIC				ANSI_ESC "[3m"
#define ANSI_ESC_ITALIC_RESET			ANSI_ESC "[23m"
#define ANSI_ESC_UNDERLINE			ANSI_ESC "[4m"
#define ANSI_ESC_UNDERLINE_RESET		ANSI_ESC "[24m"
#define ANSI_ESC_BLINK				ANSI_ESC "[5m"
#define ANSI_ESC_BLINK_RESET			ANSI_ESC "[25m"
#define ANSI_ESC_INVERSE			ANSI_ESC "[7m"
#define ANSI_ESC_INVERSE_RESET			ANSI_ESC "[27m"
#define ANSI_ESC_HIDDEN				ANSI_ESC "[8m"
#define ANSI_ESC_HIDDEN_RESET			ANSI_ESC "[28m"
#define ANSI_ESC_STRIKETHROUGH			ANSI_ESC "[9m"
#define ANSI_ESC_STRIKETHROUGH_RESET		ANSI_ESC "[29m"


////////////////////////////////////////////////////////////////////////////////
//// NOTE(Elias): RGB color

#define ANSI_ESC_RGB_FG(r,g,b)			ANSI_ESC "[38;2;" #r ";" #g ";" #b "m"
#define ANSI_ESC_RGB_BG(r,g,b)			ANSI_ESC "[48;2;" #r ";" #g ";" #b "m"


////////////////////////////////////////////////////////////////////////////////
//// NOTE(Elias): Screen modes

#define ANSI_ESC_CURSOR_HIDDEN			ANSI_ESC "[?25l"
#define ANSI_ESC_CURSOR_VISIBLE			ANSI_ESC "[?25h"
#define ANSI_ESC_RESTORE_SCREEN			ANSI_ESC "[?47l"
#define ANSI_ESC_SAVE_SCREEN			ANSI_ESC "[?47h"
#define ANSI_ESC_ENABLE_ALT_BUFFER		ANSI_ESC "[?1049h"
#define ANSI_ESC_DISABLE_ALT_BUFFER		ANSI_ESC "[?1049l"


// NOTE(Elias): view others at:
// https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797
