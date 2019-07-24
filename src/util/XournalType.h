/*
 * Xournal++
 *
 * Type macros like GLib use for C to find wrong pointers at runtime
 *
 * The attributes start with z__ because if they start with __ they appear
 * as first element in the autocomplete list...
 *
 * @author Xournal++ Team
 * https://github.com/xournalpp/xournalpp
 *
 * @license GNU GPLv2 or later
 */

#pragma once

#include "logger/Logger.h"
#include <config-dev.h>

#include <glib.h>

// Include string and vector everywhere
#include <string>
using std::string;
#include <vector>
using std::vector;

#include <string.h>

#ifdef DEV_MEMORY_CHECKING

#ifdef DEV_MEMORY_LEAK_CHECKING

void xoj_memoryleak_initType(int id);
void xoj_memoryleak_releaseType(int id);
void xoj_momoryleak_printRemainingObjects();

#endif

#ifdef DEV_CALL_LOG
#define CALL_LOG(type, clazz, obj) Log::trace(type, clazz, __FUNCTION__, (long) obj)
#else
#define CALL_LOG(type, clazz, obj) (void(0))
#endif

#define XOJ_DECLARE_TYPE(type, id) \
	const int __XOJ_TYPE_ ## type = id

/**
 * This creates a Xournal mTypeinformation Attribute
 */
#define XOJ_TYPE_ATTRIB \
	int z__xoj_type; \
	unsigned int z__xoj_typeCheckvalue

/**
 * Initalize the Xournal type info, this should be called in the constructor
 */
#ifdef DEV_MEMORY_LEAK_CHECKING

const char* xoj_type_getName(int id);


#define XOJ_INIT_TYPE(type) \
	this->z__xoj_type = __XOJ_TYPE_ ## type; \
	this->z__xoj_typeCheckvalue = 0xFFAA00AA; \
	CALL_LOG("init", #type, this); \
	xoj_memoryleak_initType(__XOJ_TYPE_ ## type)
#else
#define XOJ_INIT_TYPE(type) \
	this->z__xoj_type = __XOJ_TYPE_ ## type; \
	CALL_LOG("init", #type, this); \
	this->z__xoj_typeCheckvalue = 0xFFAA00AA
#endif

/**
 * Release the Xournal type info, this should be called in the destructor
 */
#ifdef DEV_MEMORY_LEAK_CHECKING
#define XOJ_RELEASE_TYPE(type)                         \
	do                                                 \
	{                                                  \
		XOJ_CHECK_TYPE(type);                          \
		this->z__xoj_type = -(__XOJ_TYPE_##type);      \
		this->z__xoj_typeCheckvalue = 0xFFAA00AA;      \
		CALL_LOG("release", #type, this);              \
		xoj_memoryleak_releaseType(__XOJ_TYPE_##type); \
	} while (false)
#else
#define XOJ_RELEASE_TYPE(type) do { \
		XOJ_CHECK_TYPE(type) \
		this->z__xoj_type = -(__XOJ_TYPE_ ## type); \
		CALL_LOG("release", #type, this); \
		this->z__xoj_typeCheckvalue = 0xFFAA00AA; } while(false)
#endif


/**
 * Checks the type of "this" and returns if "this" is not an instance of "name"
 */
#define XOJ_CHECK_TYPE_OBJ(obj, type) \
	if(obj == NULL) { \
		g_warning("XojTypeCheck failed: NULL %s:%i", __FILE__, __LINE__);\
	} \
	if(((type *)obj)->z__xoj_typeCheckvalue != 0xFFAA00AA) { \
		g_warning("XojTypeCheck failed: expected %s but get something else on %s:%i", #type, __FILE__, __LINE__);\
	} \
	if(((type *)obj)->z__xoj_type != __XOJ_TYPE_ ## type) { \
		g_warning("XojTypeCheck failed: expected %s but get %s on %s:%i", #type, xoj_type_getName(((type *)obj)->z__xoj_type), __FILE__, __LINE__);\
	} \
	CALL_LOG("call", #type, obj)

/**
 * Checks the type of "this" and returns if "this" is not an instance of "name"
 */
#define XOJ_CHECK_TYPE(type) \
	if(this == NULL) { \
		g_warning("XojTypeCheck failed: NULL %s:%i", __FILE__, __LINE__);\
	} \
	if(((type *)this)->z__xoj_typeCheckvalue != 0xFFAA00AA) { \
		g_warning("XojTypeCheck failed: expected %s but get something else on %s:%i", #type, __FILE__, __LINE__);\
	} \
	if(((type *)this)->z__xoj_type != __XOJ_TYPE_ ## type) { \
		g_warning("XojTypeCheck failed: expected %s but get %s on %s:%i", #type, xoj_type_getName(((type *)this)->z__xoj_type), __FILE__, __LINE__);\
	} \
	CALL_LOG("call", #type, this)


#include "XournalTypeList.h"

#else //DEV_MEMORY_CHECKING

#define XOJ_DECLARE_TYPE(name, id) ((void) 0)
#define XOJ_TYPE_ATTRIB int z__xoj_type[0]
#define XOJ_INIT_TYPE(name) ((void) 0)
#define XOJ_RELEASE_TYPE(name) ((void) 0)
#define XOJ_CHECK_TYPE_OBJ(obj, name) ((void) 0)
#define XOJ_CHECK_TYPE(name) ((void) 0)

#endif //DEV_MEMORY_CHECKING
