/*

  .....____    ...::$$$$=$=$=$  ...____
_____/    /__        $$     $$___/    /___
\           //\\____$$   __$$\__         /
 \__     __//        /  / $$__  /    ___/
   /    /___\   \.   ) / $$  / /    /___
  /        /    /   / / $$  __/        /
  \_______....  ___/ / $$_   \\_______/
          /    /$$=$=$=$/ \   \
      ...(____/           /   /
                 .......:::./

    mapimp plugin version 1.0
    coded by takerZ
    tPORt, 2009-2012

YOU CAN USE THE SOURCE ANY WAY YOU LIKE WITHOUT
RESTRICTIONS, BUT STILL REMEMBER THAT THIS SOFTWARE
IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE BLAH BLAH BLAH
*/

#ifndef __MAPIMP_H__
#define __MAPIMP_H__

/* use PCRE library */
#define PCRE_STATIC
#include "pcre\\t_pcre.h"

/* and OllyDbg/ImmDbg Plugin SDK to compile */
#if defined(OLLYDBG)
#include "pdk\\odbg\\plugin.h"
#elif defined(OLLYDBG2)
#include "pdk\\odbg2\\plugin.h"
#elif defined(IMMDBG)
#include "pdk\\idbg\\plugin.h"
#endif

/* Target specific stuff */
#if defined(OLLYDBG)
#pragma comment(lib, "pdk\\odbg\\ollydbgvc7.lib")
#elif defined(OLLYDBG2)
#pragma comment(lib, "pdk\\odbg2\\ollydbg.lib")
#elif defined(IMMDBG)
#pragma comment(lib, "pdk\\idbg\\immunitydebugger.lib")
#endif

#pragma comment(lib, "comctl32.lib")
#ifdef _UNICODE
#pragma comment(lib, "pcre\\pcre16.lib")
#else
#pragma comment(lib, "pcre\\pcre.lib")
#endif

/* Shut deprecated/unsafe and macro missing params warnings the hell up */
#pragma warning(disable: 4996 4003)

typedef struct
{
    size_t count;
    LPVOID first;
    LPVOID last;
} list_t, *plist_t;

typedef struct
{
    BOOL    comments;
    BOOL    labels;
    BOOL    check_collisions;
    ULONG   autoimport;
    BOOL    read_file_segments;
    BOOL    demangle;
    BOOL    use_masks;
    plist_t masks;
} config_t, *pconfig_t;

typedef struct
{
    LPVOID         next;
    TCHAR*         buffer;
    TCHAR*         repl;
    size_t         repl_s;
    int            insert;
    int            type;
    int            errcode;
    char*          errptr;
    int            erroffset;
    _t_pcre*       regex;
    _t_pcre_extra* extra;
} mask_t, *pmask_t;

#define OVECTOR_COMP_COUNT 3

typedef struct
{
    int rm_so;
    int rm_eo;
    int reserved;
} match_t;

typedef struct
{
    ULONG  base;
    ULONG  size;
    TCHAR* name;
    PULONG segments;
    size_t nseg;
} module_t, *pmodule_t;

typedef struct
{
    LPVOID next;
    TCHAR* buffer;
    size_t size;
    ULONG  offset;
    ULONG  segment;
} name_t, *pname_t;

/* Plugin action identifiers */
typedef enum
{
    ACTION_IMPORT,
    ACTION_OPTIONS,
    ACTION_DELIMITER,
    ACTION_ABOUT
} __action_t;

/* Autoimport option definitions */
typedef enum
{
    AUTOIMPORT_DISABLED,
    AUTOIMPORT_ASK,
    AUTOIMPORT_ALWAYS
} __autoimport_t;

/* mask_compile output definitions */
typedef enum
{
    FILTER_NOERROR,
    /*
        The last pcre_compile2() error code is 67
        as defined in PCRE documentation
    */
    FILTER_INVALID_MASK = 68,
    FILTER_INVALID_KEY,
    FILTER_INVALID_REPLACEMENT
} __filter_error_t;

/* mask_filter output definitions */
typedef enum
{
    FILTER_NOTHING,
    FILTER_SKIP,
    FILTER_CUT,
    FILTER_REPLACE
} __mask_type_t;

/* module_info error codes */
typedef enum
{
    MODULE_SUCCESS,
    MODULE_OUT_OF_RANGE,
    MODULE_BASE_NOT_FOUND,
    MODULE_FILE_MAPPING_FAILURE,
    MODULE_FILE_SHARING_VIOLATION,
    MODULE_MEMORY_READ_FAILURE
} __module_error_t;

/* Module manual details constants */
#define MODULE_MANUAL_WIDTH  195
#define MODULE_MANUAL_HEIGHT 90
#define MODULE_SIZE_MAX      ULONG_MAX
#define MODULE_UNKNOWN_NAME  "unknown"

typedef enum
{
    ID_ADD = 2000,
    ID_APPLY,
    ID_READFROM,
    ID_ASKTOIMPORT,
    ID_AUTOIMPORT,
    ID_CANCEL,
    ID_COLLISIONS,
    ID_COMMENTS,
    ID_MEMORY,
    ID_DECREASE,
    ID_DELETE,
    ID_DEMANGLE,
    ID_DONOTHING,
    ID_EDIT,
    ID_FILTER,
    ID_IMPORT,
    ID_IMPORTALWAYS,
    ID_INCREASE,
    ID_INSERT,
    ID_LABELS,
    ID_LOAD,
    ID_MASKS,
    ID_MAXINCREASE,
    ID_MAXDECREASE,
    ID_FILE,
    ID_OVERWRITE,
    ID_SAVE,
    ID_SKIP,
    ID_USEMASKS
} __wnd_id_t;

#define DBGPRINT(token)  Addtolist(0, -1, "  >> "#token)
#define DBGPRINTF(token) Addtolist(0, -1, "  >> "token)
#define WATCH_D(token)   Addtolist(0, -1, "  >> "#token" == %d", token)
#define WATCH_H(token)   Addtolist(0, -1, "  >> "#token" == %08X", token)
#define WATCH_STR(token) Addtolist(0, -1, "  >> "#token" == \"%s\"", token)

/* .UDD tag */
#define TAG_MAPIMP               0x504D694D

#define MAPBUFLEN                2048
#define ERRBUFLEN                2 * TEXTLEN
#ifdef MASKTEST
#define TEXTLEN                  255
#endif

/* Configuration file parameters */
#define CONFIG_STR_COMMENTS      _T("comments")
#define CONFIG_STR_LABELS        _T("labels")
#define CONFIG_STR_COLLISIONS    _T("check_collisions")
#define CONFIG_STR_FILESEG       _T("read_file_segments")
#define CONFIG_STR_AUTOIMPORT    _T("autoimport")
#define CONFIG_STR_DEMANGLE      _T("demangle")
#define CONFIG_STR_USEMASKS      _T("use_masks")
#define CONFIG_STR_MASKS         _T("masks")
#define CONFIG_STR_MAXSIZE       sizeof(CONFIG_STR_FILESEG)

#define CONFIG_SEPARATOR_CHARSET _T(" =-\n")

/* Option window definitions */
#define OPTWND_CLASS_NAME        _T("mapimp_wnd")
#define OPTWND_WINDOW_NAME       _T("Options")
#define OPTWND_FONT_NAME         _T("Verdana")
#if defined(OLLYDBG) || defined(IMMDBG)
#define OPTWND_ICON_NAME         _T("ICO_OPTIONS")
#endif
#define OPTWND_WINDOW_WIDTH      427
#define OPTWND_WINDOW_HEIGHT     312

#define INPUTWND_TYPE            196

#endif
