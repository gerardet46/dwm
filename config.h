/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx       = 2;   /* border pixel of windows */
static unsigned int snap           = 32;  /* snap pixel */
static unsigned int gappih         = 7;   /* horiz inner gap between windows */
static unsigned int gappiv         = 7;   /* vert inner gap between windows */
static unsigned int gappoh         = 8;   /* horiz outer gap between windows and screen edge */
static unsigned int gappov         = 8;   /* vert outer gap between windows and screen edge */
static int smartgaps               = 1;   /* 1 means no outer gap when there is only one window */
static int smartborders            = 1;   /* 1 means no outer gap when there is only one window */
static unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static unsigned int systrayonleft  = 0;   /* 0: systray in the right corner, >0: systray on left of status text */
static unsigned int systrayspacing = 2;   /* systray spacing */
static int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static int showsystray             = 1;   /* 0 means no systray */
static int showbar                 = 1;   /* 0 means no bar */
static int topbar                  = 1;   /* 0 means bottom bar */
static int user_bh                 = 24;  /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static char font[]                 = "Ubuntu:size=11";
static char *fonts[]               = { font, "Font Awesome 6:size=13", "Noto Color Emoji:pixelsize=12:antialias=true:autohint=true" };
static char normbgcolor[]          = "#20242c";
static char normbordercolor[]      = "#3b4252";
static char normfgcolor[]          = "#e5e9f0";
static char selbgcolor[]           = "#3b4252";
static char selfgcolor[]           = "#eceff4";
static char primarycolor[]         = "#94628a"; /* dark emphasis  */
static char secondarycolor[]       = "#b48ead"; /* light emphasis */
static char *colors[][3]           = {
    /*                   fg              bg            border   */
    [SchemeStatus]   = { normfgcolor,    normbgcolor,  normbordercolor },
    [SchemeNorm]     = { normfgcolor,    normbgcolor,  normbordercolor },
    [SchemeSel]      = { selfgcolor,     primarycolor, primarycolor    },
    [SchemeTagsSel]  = { secondarycolor, selbgcolor,   "#000000" }, /* Tagbar left selected {text,background,not used but cannot be empty} */
    [SchemeTagsNorm] = { normfgcolor,    normbgcolor,  "#000000" }, /* Tagbar left unselected {text,background,not used but cannot be empty} */
    [SchemeInfoSel]  = { selfgcolor,     primarycolor, "#000000" }, /* infobar middle  selected {text,background,not used but cannot be empty} */
    [SchemeInfoNorm] = { normfgcolor,    normbgcolor,  "#000000" }, /* infobar middle  unselected {text,background,not used but cannot be empty} */
};

#define BROWSER   "$BROWSER"
#define TERMINAL  "st"
#define STATUSBAR "dwmblocks"
#define FMANAGER  "st -e nnn"
#define MAIL      "st -e neomutt ; pkill -RTMIN+12 dwmblocks"

/* tagging */
static const char *tags[] = { "  ", "  ", "  ", "  ", "  ", " 6 ", " 7 ", " 8 ", "  " };
static const char *tagsalt[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
 *	WM_CLASS(STRING) = instance, class
 *	WM_NAME(STRING) = title
 */
    /* class      instance          title             tags mask  isfloating   issticky    monitor */
    { "Firefox",        NULL,       NULL,              1 << 1,       0,           0,        -1 },
    { "librewolf",      NULL,       NULL,              0 << 1,       0,           0,        -1 },
    { "Navigator",      NULL,       NULL,              1 << 1,       0,           0,        -1 },
    { "Brave-browser",  NULL,       NULL,              1 << 1,       0,           0,        -1 },
    { "Spotify",        NULL,       NULL,              1 << 8,       0,           0,        -1 },
    { "zoom",           NULL,       NULL,              1 << 3,       1,           0,        -1 },
    { "zoom",           NULL,       "Zoom Meeting",    1 << 3,       0,           0,        -1 },
    { "TelegramDesktop",NULL,       NULL,              1 << 4,       0,           0,        -1 },
    { "mpv",            NULL,       "video0 - mpv",    0,            1,           1,        -1 },
    { "lxqt-openssh-askpass",   NULL, NULL,            0,            1,           0,        -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
    /* symbol  arrange function */
    { "[]=",   tile },    /* first entry is default */
    { "[M]",   monocle },

    /*{ "[@]",   spiral },
      { "[\\]",  dwindle },*/

    { "H[]",   deck },
    { "TTT",   bstack },

    /*{ "|M|",   centeredmaster },
    { ">M>",   centeredfloatingmaster },

    { "###",   nrowgrid },
    { "---",   horizgrid },
    { ":::",   gaplessgrid },
    { "===",   bstackhoriz },
    { "HHH",   grid },*/
    { "><>",   NULL },    /* no layout function means floating behavior */
    { NULL,    NULL },
};

/* key definitions */
#define MODKEY Mod4Mask /* Mod key */
#define ALTMOD Mod1Mask /* Alt key */

#define TAGKEYS(KEY, TAG)\
    {MODKEY,                       KEY, view,        {.ui = 1 << TAG}},\
    {MODKEY|ControlMask,           KEY, toggleview,  {.ui = 1 << TAG}},\
    {MODKEY|ShiftMask,             KEY, tag,         {.ui = 1 << TAG}},\
    {MODKEY|ControlMask|ShiftMask, KEY, toggletag,   {.ui = 1 << TAG}},\
    {MODKEY|ALTMOD,                KEY, focusnthmon, {.i = TAG}},\
    {MODKEY|ALTMOD|ShiftMask,      KEY, tagnthmon,   {.i = TAG}},

#define STACKKEYS(MOD, ACTION)\
    {MOD, XK_j,     ACTION##stack, {.i = INC(+1)}},\
    {MOD, XK_k,     ACTION##stack, {.i = INC(-1)}},\
    {MOD, XK_grave, ACTION##stack, {.i = PREVSEL}},\
    {MOD, XK_a,     ACTION##stack, {.i = 1}},\
    {MOD, XK_z,     ACTION##stack, {.i = 2}},\
    {MOD, XK_x,     ACTION##stack, {.i = -1}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { TERMINAL, NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
    { "font",                    STRING,  &font },
    { "normbgcolor",             STRING,  &normbgcolor },
    { "normbordercolor",         STRING,  &normbordercolor },
    { "normfgcolor",             STRING,  &normfgcolor },
    { "selbgcolor",              STRING,  &selbgcolor },
    { "selfgcolor",              STRING,  &selfgcolor },
    { "primarycolor",            STRING,  &primarycolor },
    { "secondarycolor",          STRING,  &secondarycolor },
    { "gappih",                  INTEGER, &gappih },
    { "gappiv",                  INTEGER, &gappiv },
    { "gappoh",                  INTEGER, &gappoh },
    { "gappov",                  INTEGER, &gappov },
    { "smartgaps",               INTEGER, &smartgaps },
    { "smartborders",            INTEGER, &smartborders },
    { "systraypinning",          INTEGER, &systraypinning },
    { "systrayonleft",           INTEGER, &systrayonleft },
    { "systrayspacing",          INTEGER, &systrayspacing },
    { "systraypinningfailfirst", INTEGER, &systraypinningfailfirst },
    { "showsystray",             INTEGER, &showsystray },
    { "showbar",                 INTEGER, &showbar },
    { "topbar",                  INTEGER, &topbar },
    { "user_bh",                 INTEGER, &user_bh },
    { "borderpx",          	     INTEGER, &borderpx },
    { "snap",          		     INTEGER, &snap },
    { "nmaster",          	     INTEGER, &nmaster },
    { "resizehints",       	     INTEGER, &resizehints },
    { "mfact",      	 	     FLOAT,   &mfact },
};

#include <X11/XF86keysym.h>

static Key keys[] = {
    /* modifier                     key        function        argument */
    { 0,                            XK_F2,     spawn,          SHCMD("dmenu_run") },
    { ShiftMask,                    XK_F2,     spawn,          SHCMD("passmenu") },
    { MODKEY,                       XK_F2,     spawn,          SHCMD("mydmenu_run") },
    { MODKEY,                       XK_F5,     xrdb,           {0}},
    { MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
    { MODKEY,                       XK_b,      togglebar,      {0} },
    STACKKEYS(MODKEY,                          focus)
    STACKKEYS(MODKEY|ShiftMask,                push)
    { MODKEY,                       XK_m,      incnmaster,     {.i = +1 } },
    { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
    { MODKEY,                       XK_h,      setmfact,       {.f = -0.02} },
    { MODKEY,                       XK_l,      setmfact,       {.f = +0.02} },
    { MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.1} },
    { MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.1} },
    { MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
    { MODKEY,                       XK_space,  zoom,           {0} },
    { MODKEY|Mod1Mask,              XK_u,      incrgaps,       {.i = +1 } },
    { MODKEY|Mod1Mask|ShiftMask,    XK_u,      incrgaps,       {.i = -1 } },
    { MODKEY|Mod1Mask,              XK_i,      incrigaps,      {.i = +1 } },
    { MODKEY|Mod1Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
    { MODKEY|Mod1Mask,              XK_o,      incrogaps,      {.i = +1 } },
    { MODKEY|Mod1Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
    { MODKEY|Mod1Mask,              XK_6,      incrihgaps,     {.i = +1 } },
    { MODKEY|Mod1Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
    { MODKEY|Mod1Mask,              XK_7,      incrivgaps,     {.i = +1 } },
    { MODKEY|Mod1Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
    { MODKEY|Mod1Mask,              XK_8,      incrohgaps,     {.i = +1 } },
    { MODKEY|Mod1Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
    { MODKEY|Mod1Mask,              XK_9,      incrovgaps,     {.i = +1 } },
    { MODKEY|Mod1Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
    { MODKEY|Mod1Mask,              XK_0,      togglegaps,     {0} },
    { MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
    { MODKEY,                       XK_Tab,    cyclelayout,    {.i = +1} },
    { MODKEY|ShiftMask,             XK_Tab,    cyclelayout,    {.i = -1} },
    { MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
    { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} }, /* default */
    { MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[3]} }, /* bstack */
    { MODKEY,	                    XK_u,      setlayout,      {.v = &layouts[2]} }, /* deck */
    { MODKEY|ShiftMask,             XK_u,      setlayout,      {.v = &layouts[1]} }, /* monocle */
    { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
    { MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
    { MODKEY,                       XK_s,      togglesticky,   {0} },
    { MODKEY|ShiftMask,             XK_g,      togglesmartgaps,{0} },
    { MODKEY|ShiftMask,             XK_b,      togglesmartborders,{0} },
    { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
    { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
    { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
    { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
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
    { MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
    { MODKEY|ShiftMask,             XK_r,      quit,           {1} }, 

    /* VOLUM - BRILLANTOR */
    /*{ 0, XF86XK_AudioMute,          spawn,    SHCMD("pactl set-sink-mute @DEFAULT_SINK@ toggle && kill -44 $(pidof dwmblocks)") },
    { 0, XF86XK_AudioRaiseVolume,   spawn,    SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +5% && kill -44 $(pidof dwmblocks)") },
    { 0, XF86XK_AudioLowerVolume,   spawn,    SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -5% && kill -44 $(pidof dwmblocks)") },
    { 0, XF86XK_MonBrightnessUp,    spawn,    SHCMD("light -A 3") },
    { 0, XF86XK_MonBrightnessDown,  spawn,    SHCMD("light -U 3") },
    { 0, XF86XK_AudioPrev,          spawn,    SHCMD("mpc prev") },
    { 0, XF86XK_AudioNext,          spawn,    SHCMD("mpc next") },
    { 0, XF86XK_AudioPause,         spawn,    SHCMD("mpc pause") },
    { 0, XF86XK_AudioPlay,          spawn,    SHCMD("mpc play") },
    { 0, XF86XK_AudioStop,          spawn,    SHCMD("mpc stop") },
    { 0, XF86XK_AudioRewind,        spawn,    SHCMD("mpc seek -10") },
    { 0, XF86XK_AudioForward,       spawn,    SHCMD("mpc seek +10") },
    { 0, XF86XK_AudioMedia,         spawn,    SHCMD(TERMINAL " -e ncmpcpp") },
    { 0, XF86XK_AudioMicMute,       spawn,    SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },
    { 0, XF86XK_PowerOff,           spawn,    SHCMD("sysact") },
    { 0, XF86XK_Calculator,         spawn,    SHCMD(TERMINAL " -e bc -l") },
    { 0, XF86XK_Sleep,              spawn,    SHCMD("sudo -A zzz") },
    { 0, XF86XK_WWW,                spawn,    SHCMD(BROWSER) },
    { 0, XF86XK_DOS,                spawn,    SHCMD(TERMINAL) },
    { 0, XF86XK_ScreenSaver,        spawn,    SHCMD("slock & xset dpms force off") },
    { 0, XF86XK_TaskPane,           spawn,    SHCMD(TERMINAL " -e htop") },
    { 0, XF86XK_Mail,               spawn,    SHCMD(TERMINAL " -e neomutt ; pkill -RTMIN+12 dwmblocks") },
    { 0, XF86XK_MyComputer,         spawn,    SHCMD(TERMINAL " -e nnn /") },
    { 0, XF86XK_Launch1,            spawn,    SHCMD("xset dpms force off") },
    { 0, XF86XK_TouchpadToggle,     spawn,    SHCMD("(synclient | grep 'TouchpadOff.*1' && synclient TouchpadOff=0) || synclient TouchpadOff=1") },
    { 0, XF86XK_TouchpadOff,        spawn,    SHCMD("synclient TouchpadOff=1") },
    { 0, XF86XK_TouchpadOn,         spawn,    SHCMD("synclient TouchpadOff=0") },
    */

    /* SCRIPTS */
    /* 0xa1 is the key for "'". Execute xev to discover more keys */
    /*{ 0,                         XK_F3,       spawn,   SHCMD("dm-emoji")   },
    { 0,                         0xa1,        spawn,   SHCMD("flybinds")      },
    { 0,                         XK_Print,    spawn,   SHCMD("dm-captura")       },
    { MODKEY,                    XK_p,        spawn,   SHCMD("flybinds c m")  }, /* SHCMD("monitors") */
    /*{ MODKEY,                    XK_c,        spawn,   SHCMD("webcam")        },
    { ControlMask|Mod1Mask,      XK_Delete,   spawn,   SHCMD("flybinds x")    }, /* SHCMD("power") */

    /* CAPS LOCK */
    /*{ MODKEY, 0xa1, spawn, SHCMD("xdotool key Caps_Lock") },*/

    /* APPS */
    /*{ MODKEY|Mod1Mask,  XK_l,    spawn,   SHCMD("slock & xset dpms force off") },
    { MODKEY,           XK_w,    spawn,   SHCMD(BROWSER)  },
    { MODKEY,           XK_f,    spawn,   SHCMD(FMANAGER) },
    { MODKEY,           XK_e,    spawn,   SHCMD(MAIL)     },*/
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[1]} },
    { ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
    { ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
    { ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

/* signal definitions */
/* signum must be greater than 0 */
/* trigger signals using `xsetroot -name "fsignal:<signum>"` */
static Signal signals[] = {
    /* signum function      argument  */
    /* LAYOUTS */
    { 0,      setlayout,    {.v = &layouts[0]} }, /* default */
    { 1,      setlayout,    {.v = &layouts[3]} }, /* bstack */
    { 4,      setlayout,    {.v = &layouts[2]} }, /* deck */
    { 5,      setlayout,    {.v = &layouts[1]} }, /* monocle */

    /* DWM MISC */
    { 10,     killclient,   {0}        },
    { 11,     quit,         {1}        }, 
    { 12,     setborderpx,  {.i = +1}  },
    { 13,     setborderpx,  {.i = -1}  },
    { 14,     incrgaps,     {.i = +1 } },
    { 15,     incrgaps,     {.i = -1 } },
    { 16,     incrigaps,    {.i = +1 } },
    { 17,     incrigaps,    {.i = -1 } },
    { 18,     incrogaps,    {.i = +1 } },
    { 19,     incrogaps,    {.i = -1 } },
    { 20,     incrihgaps,   {.i = +1 } },
    { 21,     incrihgaps,   {.i = -1 } },
    { 22,     incrivgaps,   {.i = +1 } },
    { 23,     incrivgaps,   {.i = -1 } },
    { 24,     incrohgaps,   {.i = +1 } },
    { 25,     incrohgaps,   {.i = -1 } },
    { 26,     incrovgaps,   {.i = +1 } },
    { 27,     incrovgaps,   {.i = -1 } },
    { 28,     togglegaps,   {0}        },
    { 29,     defaultgaps,  {0}        },

    /* TOGGLE */
    { 50,    togglebar,          {0} },
    { 51,    togglefloating,     {0} },
    { 52,    togglefullscr,      {0} },
    { 53,    togglesticky,       {0} },
    { 54,    togglesmartgaps,    {0} },
    { 55,    togglesmartborders, {0} },
};
