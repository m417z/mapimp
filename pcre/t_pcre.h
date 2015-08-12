#ifndef _T_PCRE_H
#define _T_PCRE_H

#include "pcre.h"

#ifdef _UNICODE
#define _t_pcre               pcre16
#define _t_pcre_extra         pcre16_extra
#define _t_pcre_compile2      pcre16_compile2
#define _t_pcre_free          pcre16_free
#define _t_pcre_exec          pcre16_exec
#define _t_pcre_study         pcre16_study
#else
#define _t_pcre               pcre
#define _t_pcre_extra         pcre_extra
#define _t_pcre_compile2      pcre_compile2
#define _t_pcre_free          pcre_free
#define _t_pcre_exec          pcre_exec
#define _t_pcre_study         pcre_study
#endif

#endif /* End of pcre.h */
