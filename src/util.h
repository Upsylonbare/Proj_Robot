#ifndef UTIL_H 
#define UTIL_H 

/**
 *   \file util.h
 *   \brief Mes macros pour faire des traces et des assertions (en couleur grâce à Clément Le Goffic)
  *  \warning Ne fonctionne qu'avec le compilo gcc pour les fonctions TRACE
  *  \author Jerome Delatour adapté par Clément Le Goffic
  *  \date Decembre 2012
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "config.h"

#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define DEFAUT  "\033[39m"

#ifndef NDEBUG
#define TRACE(startColor,fmt, ...) do {fprintf(stderr, startColor "%s:%d:%s(): " fmt DEFAUT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); fflush (stderr); } while(0);    
#define ASSERT_PRINTERROR(assertion) do {if (!(assertion)) {perror("Erreur"); assert(assertion);} } while(0);    
#define STOP_ON_ERROR(error_condition) do {     \
	if(error_condition) { \
		fprintf (stderr, "*** Error (%s) at \
		%s:%d\nExiting\n", \
		#error_condition, __FILE__, __LINE__); \
		perror (""); \
		_exit (1); } \
    } while (0); 

#define TRACE_PUML_START fprintf(stderr, "%s\n", "@startuml"); fflush(stderr); 
#define TRACE_PUML_END fprintf(stderr, "%s\n", "@enduml"); fflush(stderr); 
#define TRACE_PUML(fmt, ...) fprintf(stderr,fmt, ##__VA_ARGS__); fflush(stderr); 
//Clement's MACRO
#define LOG_REMOTEUI(fmt, ...) do{TRACE(YELLOW,fmt,##__VA_ARGS__);}while(0);
#define LOG_CLIENT(fmt, ...) do{TRACE(CYAN,fmt,##__VA_ARGS__);}while(0);
#define LOG_ROBOT(fmt, ...) do{TRACE(BLUE,fmt,##__VA_ARGS__);}while(0);
#define LOG_PILOT(fmt, ...) do{TRACE(GREEN,fmt,##__VA_ARGS__);}while(0);
#define LOG_SERVER(fmt, ...) do{TRACE(MAGENTA,fmt,##__VA_ARGS__);}while(0);



#else
#define TRACE(fmt, ...) 
#define ASSERT_PRINTERROR(assertion) assert(assertion);
#define TRACE_PUML(fmt, ...)
#define TRACE_PUML_START 
#define TRACE_PUML_END

#define STOP_ON_ERROR(error_condition)
#endif 

#endif /* UTIL_H */


