/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx         =  5;        /* border pixel of windows */
static unsigned int gappx            =  30;       /* gaps between windows */
static unsigned int snap             =  32;       /* snap pixel */
static int showbar                   =  1;        /* 0 means no bar */
static int topbar                    =  1;        /* 0 means bottom bar */
static int user_bh                   =  24;       /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const char *fonts[]           =  { "JetBrains Mono:size=10", "monospace:size=10", "Noto Color Emoji:pixelsize=12", "Hack Nerd Font Mono:style=Regular:size=16" };
static const char dmenufont[]        =  "monospace:size=10";
static const char terminalfont[]     =  "monospace:style=Bold:size=10";
static char normbgcolor[]            =  "#383c4a";
static char normbordercolor[]        =  "#444444";
static char normfgcolor[]            =  "#eeeeee";
static char normtagscolor[]          =  "#7c818c";
static char selfgcolor[]             =  "#fefefe";
static char selbgcolor[]             =  "#222222";
static char selbordercolor[]         =  "#5294e2";
static char seltagsfgcolor[]         =  "#5294e2";
static char seltagsbgcolor[]         =  "#4b5162";
static char *colors[][4] = {
    /*               fg           bg           border,          float   */
    [SchemeNorm] = { seltagsfgcolor, normbgcolor, normbordercolor, normbordercolor },
    [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor,  selbordercolor },
    [SchemeStatus]  = { seltagsfgcolor, normbgcolor,  "#000000"  }, // Statusbar right {text,background,not used but cannot be empty}
    [SchemeTagsSel]  = { seltagsfgcolor, seltagsbgcolor,  "#000000"  }, // Tagbar left selected {text,background,not used but cannot be empty}
    [SchemeTagsNorm]  = { normtagscolor, normbgcolor,  "#000000"  }, // Tagbar left unselected {text,background,not used but cannot be empty}
    [SchemeInfoSel]  = { selfgcolor, seltagsfgcolor,  "#000000"  }, // infobar middle  selected {text,background,not used but cannot be empty}
    [SchemeInfoNorm]  = { seltagsfgcolor, normbgcolor,  "#000000"  }, // infobar middle  unselected {text,background,not used but cannot be empty}
};

/* tagging */
static const char *tags[] = { "", "", "", "ﴔ", "", "", "", "", "" };
static const char *tagsalt[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     iscentered   isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            0,           1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           0,           -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, NULL };
static const char *termcmd[]  = { "st", NULL };

/* Third Party Softwares */
static const char *flameshot[] = { "flameshot", "gui", NULL };
/**/

ResourcePref resources [] = {
    { "normbgcolor",            STRING,  &normbgcolor },
    { "normbordercolor",        STRING,  &normbordercolor },
    { "normfgcolor",            STRING,  &normfgcolor },
    { "selbgcolor",             STRING,  &selbgcolor },
    { "selbordercolor",         STRING,  &selbordercolor },
    { "selfgcolor",             STRING,  &selfgcolor },
    { "borderpx",               INTEGER, &borderpx },
    { "gappx",                  INTEGER, &gappx },
    { "snap",                   INTEGER, &snap },
    { "showbar",                INTEGER, &showbar },
    { "topbar",                 INTEGER, &topbar },
    { "userbh",                 INTEGER, &user_bh },
    { "nmaster",                INTEGER, &nmaster },
    { "resizehints",            INTEGER, &resizehints },
    { "mfact",                  FLOAT,   &mfact },
};

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
    { MODKEY|ShiftMask,             XK_F5,     xrdb,           {.v = NULL } },
    { MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
    { MODKEY,                       XK_n,      togglealttag,   {0} },
    TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} },
    /* CUSTOM DMENU BINDINGS */
    { MODKEY,                       XK_x,      spawn,          SHCMD(". ~/.local/bin/dmenuemoji") },
    { MODKEY|ControlMask,           XK_m,      spawn,          SHCMD(". ~/.local/bin/dmenumount") },
    { MODKEY|ControlMask,           XK_u,      spawn,          SHCMD(". ~/.local/bin/dmenuumount") },
    { MODKEY|ControlMask,           XK_p,      spawn,          SHCMD(". ~/.local/bin/dmenupower") },
    { MODKEY/*|ControlMask*/,       XK_g,      spawn,          SHCMD(". ~/.local/bin/dmenugames") },
    /* THIRD PARTY SOFTWARES */
    { MODKEY|ShiftMask,             XK_s,      spawn,          {.v = flameshot} },
    { MODKEY|ShiftMask,             XK_l,      spawn,          SHCMD("betterlockscreen -l dimblur") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
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

