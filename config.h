/* See LICENSE file for copyright and license details. */

/* appearance */
#include <X11/X.h>
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *nicefont[]       = { "Cantarell:size=16" };
static const char *monofont[]       = { "MesloLGS Nerd Font Mono:size=16" };
static const char col_black[]       = "#000000";
static const char col_cyan1[]       = "#101e21";
static const char col_cyan2[]       = "#81e8fc";
static const char col_cyan3[]       = "#d4f6fc";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_cyan3, col_black, col_black }, //"#2a4247", col_gray2 },
	[SchemeSel]  = { col_cyan2, col_cyan1, col_black }, //"#54848e", col_cyan  },
};

/* tagging */
static const char *tags[] = {
	"", // terminal
	"", // browser
	"", // general
	"", // code
	"󰝚", // spotify
};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "kitty",           NULL,     NULL,           0,         0,        -1 },
	{ "Thorium-browser", NULL,     NULL,           0,         0,        -1 },
	{ "Spotify",         NULL,     NULL,      1 << 4,         0,         0 },
	{ "code-oss",        NULL,     NULL,      1 << 3,         0,        -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "tiling",     tile },    /* first entry is default */
	{ "floating", NULL },    /* no layout function means floating behavior */
	{ "monocle",  monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, 

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "rofi", "-show", "drun", NULL };
static const char *termcmd[]  = { "kitty", NULL };
static const char *spotifycmd[]= { "spotify-launcher", NULL };
static const char *codecmd[]= { "code", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_r,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_t,      spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_s,      spawn,          {.v = spotifycmd } },
	{ MODKEY,                       XK_c,      spawn,          {.v = codecmd } },
	{ MODKEY,                       XK_b,      spawn,          SHCMD ("thorium-browser")},
	{ MODKEY,                       XK_e,      spawn,          SHCMD ("nemo")},
	{ MODKEY,                       XK_z,      spawn,          SHCMD ("blueberry")},
	{ MODKEY,                       XK_p,      spawn,          SHCMD ("flameshot full -p /home/trent/Pictures/")},
	{ MODKEY|ShiftMask,             XK_p,      spawn,          SHCMD ("flameshot gui -p /home/trent/Pictures/")},
	{ MODKEY|ControlMask,           XK_p,      spawn,          SHCMD ("flameshot gui --clipboard")},
	{ 0,                            0x1008ff02, spawn,         SHCMD ("xbacklight -inc 960")},
	{ 0,                            0x1008ff03, spawn,         SHCMD ("xbacklight -dec 960")},
	{ 0,                            0x1008ff11, spawn,         SHCMD ("pactl set-sink-volume 0 -2%")},
	{ 0,                            0x1008ff12, spawn,         SHCMD ("pactl set-sink-mute 0 toggle")},
	{ 0,                            0x1008ff13, spawn,         SHCMD ("pactl set-sink-volume 0 +2%")},
	{ 0,                            0x1008FF14, spawn,         SHCMD ("playerctl play-pause")},
	{ 0,                            0x1008FF16, spawn,         SHCMD ("playerctl previous")},
	{ 0,                            0x1008FF17, spawn,         SHCMD ("playerctl next")},
	{ MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_Right,  tag,            {.ui = +1} },
	{ MODKEY,                       XK_Left,   tag,            {.ui = -1} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	// { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

static const Key startcommand = {0, 0, spawn, SHCMD ("/home/trent/dwm-trent/autostart.sh")};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

