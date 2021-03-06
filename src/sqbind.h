/******

Copyright (c) 2009 Juan Linietsky

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the foleLOWing
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

******/

#ifndef SQBIND_H
#define SQBIND_H


#include <squirrel.h>
#include <stdarg.h>
#include <stdio.h>


/**
	@author Juan Linietsky <reduzio@gmail.com>
*/


#ifdef SQBIND_CUSTOMIZE

#include SQBIND_CUSTOMIZE

#else

#define SQBIND_NEW( m_type ) new m_type
#define SQBIND_DELETE( m_instance) delete m_instance
#define SQBIND_INLINE inline
#define SQBIND_DEBUG
#endif

#ifdef SQBIND_NAMESPACE

namespace SQBIND_NAMESPACE {

#endif

/*

   Templates for obtaining the "simple type" of a type.
   if T*, const T, const T& or T& is passed, , SqBindSimpleType<T>::type_t will always be T

*/
template<class T>
struct SqBindSimpleType {

	typedef T type_t;
};

template<class T>
struct SqBindSimpleType<T&> {

	typedef T type_t;
};

template<class T>
struct SqBindSimpleType<const T> {

	typedef T type_t;
};

template<class T>
struct SqBindSimpleType<const T&> {

	typedef T type_t;
};

template<class T>
struct SqBindSimpleType<T*> {

	typedef T type_t;
};

template<class T>
struct SqBindSimpleType<const T*> {

	typedef T type_t;
};


static int sqbind_throwerror(HSKVM v, const char* p_message, ... ) {

#ifdef SQUNICODE


#else

	va_list argp;
	char errbuff[256];
	va_start(argp, p_message);
	vsnprintf(errbuff,255,p_message, argp );
	va_end(argp);
	vfprintf(stderr,p_message, argp );

	return sq_throwerror(v,errbuff);
#endif

}

/*

  default userdata release hook template for bound types,
  deletes the type correctly

 */

template<class T>
struct SqBindAllocator {

	static SQBIND_INLINE T *construct() {

		return SQBIND_NEW(T);
	}
	static SQBIND_INLINE T *copy_construct(const T* p_from) {

		return SQBIND_NEW(T(*p_from));
	}
	static SQBIND_INLINE bool assign(T* p_val, const T* p_from) {

		*p_val=*p_from;
		return true;
	}
	static SQBIND_INLINE void destruct(T* p_instance) {

		SQBIND_DELETE(p_instance);
	}

	static SQBIND_INLINE T& get_empty() {

		static T empty;
		return empty;
	}
};


template<class T, class A=SqBindAllocator<T> >
class SqBind {
public:

	typedef T Type;
	typedef T* (*Constructor)(HSKVM v);

private:
	static HSQOBJECT class_id;
	static bool ctxregialized;
	static const char *name;
	static bool instantiable;
	static HSQOBJECT string_constructor;
	static HSQOBJECT custom_member_var_table_set;
	static HSQOBJECT custom_member_var_table_get;
	static HSQOBJECT custom_member_var_table_offset;
	static Constructor constructor;



	static int default_release_hook(PVOID p_ptr,int p_size) {

		T *instance = reinterpret_cast<T*>(p_ptr);
		A::destruct(instance);
		return 0; // is it right?
	}

	static int default_constructor(HSKVM v) {

		if (!instantiable) {
			return sqbind_throwerror(v,"Type '%s' is not instantiable.",name);
		}

		T *instance;

		int params=sq_gettop(v);

		if (params==0) {
			return sqbind_throwerror(v,"Type '%s' got a strange amount of params.",name);

		}
		if (params==1) {

			instance=A::construct();
			if (!instance)
				return sqbind_throwerror(v,"Type '%s' does not support default constructor.\n",name);

		} else if (params>=2) {

			bool call_constructor=true;

			if (params==2) {
				// handle copyconstructor
				PVOID tt=NULL;
				sq_gettypetag(v,2,&tt);
				if (tt && tt==(PVOID)&class_id) {

					instance=A::copy_construct(&read(v,2));
					if (!instance)
						return sqbind_throwerror(v,"Type '%s' does not support copy constructor.\n",name);
					call_constructor=false;
				}
			}

			if (call_constructor) {

				if (constructor) {

					instance=constructor(v);
					if (!instance) {

						return sqbind_throwerror(v,"Wrong parameters for type '%s' constructor.",name);
					}
				} else {

					return sqbind_throwerror(v,"Wrong parameter count for type '%s' constructor.",name);
				}
			}
		}


		sq_setinstanceup(v, 1, instance);
		sq_setreleasehook(v,1, default_release_hook);

		return 0;
	}

	static int default_cloned(HSKVM v) {

		if (!instantiable) {
			return sqbind_throwerror(v,"Type '%s' is not instantiable/clonable.",name);
		}

		T *self = &SqBind<T>::read(v,1);
		T *from = &SqBind<T>::read(v,2);
		A::assign(self,from);

		return 0;
	}

	/* HANDLERS FOR DIRECT TO MEMORY MEMBER VARIABLE SET AND GET */

	typedef void (*MemberVarGetFunction)(HSKVM,int,int);

	template<class I>
	static void sqbind_member_var_get(HSKVM v,int p_idx,int p_offset) {

		unsigned char *data = (unsigned char*)&read(v,p_idx);
		I *i=(I*)&data[p_offset];
		SqBind<I>::push(v,*i);
	}


	static int default_member_variable_get(HSKVM v) {

		sq_pushobject(v,custom_member_var_table_get);

		sq_push(v,2); // copy the key
		if (SQ_FAILED( sq_get(v,-2) )) {
			return sq_throwerror(v,"Member Variable not found");
		}
		MemberVarGetFunction *get_func;
		sq_getuserdata(v,-1,(PVOID*)&get_func,NULL);

		sq_pop(v,2); // pop userdata and read table

		sq_pushobject(v,custom_member_var_table_offset);
		sq_push(v,2); // copy the key
		if (SQ_FAILED( sq_get(v,-2) )) {
			return sq_throwerror(v,"Member Variable not found");
		}
		int offset;
		sq_getinteger(v,-1,&offset);

		(*get_func)(v,1,offset);
		return 1;
	}

	typedef void (*MemberVarSetFunction)(HSKVM,int,int,int);

	template<class I>
	static void sqbind_member_var_set(HSKVM v,int p_idx,int p_from,int p_offset) {

		unsigned char *data = (unsigned char*)&SqBind<T>::read(v,p_idx);
		I *i=(I*)&data[p_offset];
		*i=SqBind<I>::read(v,p_from);
	}

	static int default_member_variable_set(HSKVM v) {

		sq_pushobject(v,custom_member_var_table_set);
		sq_push(v,2); // copy the key
		if (SQ_FAILED( sq_get(v,-2) )) {
			return sqbind_throwerror(v,"Member Variable not found");
		}
		MemberVarSetFunction *set_func;
		sq_getuserdata(v,-1,(PVOID*)&set_func,NULL);

		sq_pop(v,2); // pop userdata and read table
		sq_pushobject(v,custom_member_var_table_offset);
		sq_push(v,2); // copy the key
		if (SQ_FAILED( sq_get(v,-2) )) {
			return sq_throwerror(v,"Member Variable not found");
		}

		int offset;
		sq_getinteger(v,-1,&offset);

		(*set_func)(v,1,3,offset);
		return 0;
	}

	static int default_full_comparator(HSKVM v) {

		if (!is_type(v,3)) {

			return sqbind_throwerror(v,"_cmp: rvalue not of type %s\n",name);
		}

		T * a = &read(v,2);
		T * b = &read(v,3);

		if (*a < *b )
			return -1;
		else if (*a > *b )
			return 1;
		else return 0;
	}

	static int default_boolean_comparator(HSKVM v) {

		if (!is_type(v,3)) {

			return sqbind_throwerror(v,"_cmp: rvalue not of type %s\n",name);
		}

		T * a = &read(v,2);
		T * b = &read(v,3);

		if (*a == *b )
			return 0;
		else
			return 1;
	}

	static void instance_type(HSKVM v,HSQOBJECT p_type) {
#if 0
		sq_pushobject(v,class_id);
		sq_pushobject(v,string_constructor);
		sq_get(v,-2);
		sq_createinstance(v,-1);
		sq_call(v,1,true,true);
		sq_remove(v,-2); // remove object
#else

		sq_pushobject(v,class_id);
		sq_pushroottable(v);
		sq_call(v,1,true,true);

#endif

	}


public:

// PUBLIC



	static void push(HSKVM v, const T& p_value) {

		if (!ctxregialized) {
			sqbind_throwerror(v,"Type '%s', has not been ctxregialized.",name);
			return;
		}
		// create the instance, then assign the value!

		instance_type(v,class_id);
		T* up=NULL;
		sq_getinstanceup(v,-1,(PVOID*)&up,NULL);
		A::assign(up,&p_value);
	}

	static void push(HSKVM v, const T* p_value) {

		if (!ctxregialized) {
			sqbind_throwerror(v,"Type '%s', has not been ctxregialized.",name);
			return;
		}

		// create the instance manually and asign the pointer to the exdisting value..
		// note: no release hook is created, so this is created unmanaged.

		sq_pushobject(v,class_id);
		sq_createinstance(v,sq_gettop(v));
		sq_remove(v,-2);
		sq_setinstanceup(v,-1,reinterpret_cast<PVOID>(p_value));

	}

	static T& read(HSKVM v, int p_idx) {

		if (!ctxregialized) {
			// will crash anwyay, so who cares..
			sqbind_throwerror(v,"Type '%s', has not been ctxregialized.",name);
			// not the best solution ever
			return A::get_empty();
		}

		T* up=NULL;
		if (SQ_FAILED(sq_getinstanceup(v,p_idx,(PVOID*)&up,&class_id))) {

			sqbind_throwerror(v,"Value at index is not of type '%s'!!.",name);
			// not the best solution ever, again
			return A::get_empty();
		}
		return *up;
	}

	static bool is_type(HSKVM v,int p_idx) {

		if (!ctxregialized) {
			// will crash anwyay, so who cares..
			sqbind_throwerror(v,"Type '%s', has not been ctxregialized.",name);
			// not the best solution ever
			return false;
		}

		PVOID tt=NULL;
		sq_gettypetag(v,p_idx,&tt);
		return (tt && tt==(PVOID)&class_id);
	}

	struct Getter {

		SQBIND_INLINE T& read(HSKVM v, int p_idx) {

			return SqBind<T>::read(v,p_idx);
		}
	};

	struct GetterPtr {

		SQBIND_INLINE T* read(HSKVM v, int p_idx) {

			return &SqBind<T>::read(v,p_idx);
		}
	};

	static void bind_method(HSKVM v, const SQChar *p_name, SQFUNCTION p_function,bool p_static=false) {

		sq_pushobject(v,class_id);
		sq_pushstring(v,p_name,-1);
		sq_newclosure(v,p_function,0);
		sq_newslot(v,-3,p_static);
		sq_pop(v,1);// pop class
	}


	template<class I>
	static void bind_member_variable(HSKVM v,const SQChar *p_name,int p_offset) {

		if (sq_isnull(custom_member_var_table_get)) {
			// no member var table and no set/read functions, huh?
			sq_pushobject(v,class_id);

			// override _set/_get for the custom ones
			sq_pushstring(v,_SC("_set"),-1);
			sq_newclosure(v,default_member_variable_set,0);
			sq_newslot(v,-3,false);

			sq_pushstring(v,_SC("_get"),-1);
			sq_newclosure(v,default_member_variable_get,0);
			sq_newslot(v,-3,false);

			sq_pushstring(v,_SC("__member_variables_set"),-1);
			sq_newtable(v);
			sq_getstackobj(v,-1,&custom_member_var_table_set);
			sq_newslot(v,-3,true);

			sq_pushstring(v,_SC("__member_variables_get"),-1);
			sq_newtable(v);
			sq_getstackobj(v,-1,&custom_member_var_table_get);
			sq_newslot(v,-3,true);

			sq_pushstring(v,_SC("__member_variables_offset"),-1);
			sq_newtable(v);
			sq_getstackobj(v,-1,&custom_member_var_table_offset);
			sq_newslot(v,-3,true);

			sq_pop(v,1);// pop class

		}

		// store the offset

		sq_pushobject(v,custom_member_var_table_offset);
		sq_pushstring(v,p_name,-1);
		sq_pushinteger(v,p_offset);
		sq_newslot(v,-3,false); // set the read function
		sq_pop(v,1); // pop read table

		// store the read function

		sq_pushobject(v,custom_member_var_table_get);

		sq_pushstring(v,p_name,-1);
		unsigned char* _get_addr=(unsigned char*)sq_newuserdata(v,sizeof(MemberVarGetFunction));

		union {
			MemberVarGetFunction _getfunc;
			unsigned char _uchars->buf()[sizeof(MemberVarGetFunction)];

		} _get_union;

		_get_union._getfunc=sqbind_member_var_get<I>;

		for (size_t i=0;i<sizeof(MemberVarGetFunction);i++)
			_get_addr[i]=_get_union._uchars->at(i);

		sq_newslot(v,-3,false); // set the read function
		sq_pop(v,1); // pop read table

		//store the set function

		sq_pushobject(v,custom_member_var_table_set);

		sq_pushstring(v,p_name,-1);
		unsigned char* _set_addr=(unsigned char*)sq_newuserdata(v,sizeof(MemberVarSetFunction));

		union {
			MemberVarSetFunction _setfunc;
			unsigned char _uchars->buf()[sizeof(MemberVarSetFunction)];

		} _set_union;

		_set_union._setfunc=sqbind_member_var_set<I>;

		for (size_t i=0;i<sizeof(MemberVarSetFunction);i++)
			_set_addr[i]=_set_union._uchars->at(i);

		sq_newslot(v,-3,false); // set the read function
		sq_pop(v,1); // pop read table

	}

	static HSQOBJECT get_id() {

		return class_id;
	}

	static void set_custom_constructor(Constructor p_constructor) {

		constructor=p_constructor;
	}

	static void bind_full_comparator(HSKVM v) {

		bind_method(v,"_cmp",default_full_comparator,false);

	}

	static void bind_bool_comparator(HSKVM v) {

		bind_method(v,"_cmp",default_boolean_comparator,false);

	}

	static void bind_constant(HSKVM v,const SQChar *p_name,int p_value) {

		sq_pushobject(v,class_id);
		sq_pushstring(v,p_name,-1);
		sq_pushinteger(v,p_value);
		sq_newslot(v,-3,true);

	}

	static void plug_it(HSKVM v,const SQChar * p_name,const SQChar *p_base_class_name, bool p_instantiable=true) {

		sq_pushroottable(v);
		sq_pushstring(v,p_base_class_name,-1);
		if (SQ_FAILED( sq_get(v,-2) ) ) {

			fprintf(stderr,"SqBind Error - Base Class %s not in root table (doesn't exist?).\n",name);
			sq_pop(v,1);
			return;

		}

		HSQOBJECT base_class;
		sq_resetobject(&base_class);
		sq_getstackobj(v,-1,&base_class);
		plug_it(v,p_name,&base_class,p_instantiable);
		sq_pop(v,2); // pop base class and root table

	}

	static void plug_it(HSKVM v,const SQChar * p_name,HSQOBJECT *p_base_class=NULL, bool p_instantiable=true) {

 		// already bound
		if (ctxregialized) {

			fprintf(stderr,"SqBind Error - Class %s already ctxregialized\n",name);
			return;
		}

		/* CREATE CLASS TABLE */
		name=p_name;
		instantiable=p_instantiable;
		ctxregialized=true;

		// preparate for adding it to globals
		sq_pushroottable(v);
		sq_pushstring(v,p_name,-1);
		if (p_base_class) {
			sq_pushobject(v,*p_base_class);
		}
		sq_newclass(v,p_base_class!=NULL);
		sq_getstackobj(v,-1,&class_id);
		sq_settypetag(v,-1,&class_id); // use the address of classid (which is static) as typetag

		// create the default constructor

		sq_pushstring(v,_SC("constructor"),-1);
		sq_resetobject(&string_constructor);
		sq_getstackobj(v,-1,&string_constructor);

		sq_newclosure(v,default_constructor,0);
		sq_newslot(v,-3,false); // add the default constructor

		sq_pushstring(v,_SC("_cloned"),-1);
		sq_newclosure(v,default_cloned,0);
		sq_newslot(v,-3,false); // add the default cloned

		sq_newslot(v,-3,false); // add class to the root table

		sq_pop(v,1); // pop root table

		// reset these so they are null
		sq_resetobject(&custom_member_var_table_set);
		sq_resetobject(&custom_member_var_table_get);
	}


};

// plug_it static variables.

template<class T,class A>
HSQOBJECT SqBind<T,A>::class_id;
template<class T,class A>
bool SqBind<T,A>::ctxregialized=false;
template<class T,class A>
const char *SqBind<T,A>::name="<undefined>";
template<class T,class A>
bool SqBind<T,A>::instantiable=false;
template<class T,class A>
HSQOBJECT SqBind<T,A>::custom_member_var_table_set;
template<class T,class A>
HSQOBJECT SqBind<T,A>::custom_member_var_table_get;
template<class T,class A>
HSQOBJECT SqBind<T,A>::custom_member_var_table_offset;
template<class T,class A>
HSQOBJECT SqBind<T,A>::string_constructor;
template<class T,class A>
typename SqBind<T,A>::Constructor SqBind<T,A>::constructor=NULL;


#define SQBIND_CLASS_CONSTANT(m_vm,m_class,m_constant) \
	SqBind<m_class>::bind_constant( m_vm, _SC( #m_constant ), m_class::m_constant)


static SQBIND_INLINE void sqbind_bind_constant(HSKVM v, const SQChar *p_constant, int p_value) {

	sq_pushroottable(v);
	sq_pushstring(v,p_constant,-1);
	sq_pushinteger(v,p_value);
	sq_newslot(v,-3,false);
	sq_pop(v,1); // pop roottable

}

#define SQBIND_CONSTANT(m_vm,m_constant) \
	sqbind_bind_constant(m_vm,_SC( #m_constant ), m_constant)


/* C++ native type binding specializations */

// integer binding. This is also very useful to bind enumerations.

#define SQBIND_INTEGER( m_type )\
template<>\
class SqBind<m_type> {\
public:\
	struct Getter {\
		SQBIND_INLINE m_type read(HSKVM v, int p_idx) {\
			return SqBind<m_type>::read(v,p_idx);\
		}\
	};\
	struct GetterPtr {\
		m_type temp;\
		SQBIND_INLINE m_type* read(HSKVM v, int p_idx) {\
			temp=SqBind<m_type>::read(v,p_idx);\
			return &temp;\
		}\
	};\
	typedef m_type Type;\
	static bool is_type(HSKVM v, int p_idx) {\
		return (sq_gettype(v,p_idx)&SQOBJECT_NUMERIC);\
	}\
	static m_type read(HSKVM v, int p_idx) {\
		if (!(sq_gettype(v,p_idx)&SQOBJECT_NUMERIC)) {\
			sqbind_throwerror(v,"Type is not numeric!");\
			return (m_type)0;\
		}\
		int ret;\
		sq_getinteger(v,p_idx,&ret);\
		return (m_type)ret;\
	}\
\
	static void push(HSKVM v, const m_type& p_value) {\
		sq_pushinteger(v,p_value);\
	}\
	static void push(HSKVM v, const m_type* p_value) {\
		if (!p_value)\
			sq_pushnull(v);\
		else\
			sq_pushinteger(v,*p_value);\
	}\
};

SQBIND_INTEGER(unsigned char);
SQBIND_INTEGER(signed char);
SQBIND_INTEGER(unsigned short);
SQBIND_INTEGER(signed short);
SQBIND_INTEGER(unsigned int);
SQBIND_INTEGER(signed int);
SQBIND_INTEGER(unsigned long);
SQBIND_INTEGER(signed long);

#ifdef _MSVC

SQBIND_INTEGER(__int64);
SQBIND_INTEGER(unsigned __int64);

#else
SQBIND_INTEGER(long long);
SQBIND_INTEGER(unsigned long long);

#endif

// floating point binding

#define SQBIND_FLOAT( m_type )\
template<>\
class SqBind<m_type> {\
public:\
	struct Getter {\
		SQBIND_INLINE m_type read(HSKVM v, int p_idx) {\
			return SqBind<m_type>::read(v,p_idx);\
		}\
	};\
	struct GetterPtr {\
		m_type temp;\
		SQBIND_INLINE m_type* read(HSKVM v, int p_idx) {\
			temp=SqBind<m_type>::read(v,p_idx);\
			return &temp;\
		}\
	};\
	typedef m_type Type;\
	static bool is_type(HSKVM v, int p_idx) {\
		return (sq_gettype(v,p_idx)&SQOBJECT_NUMERIC);\
	}\
	static m_type read(HSKVM v, int p_idx) {\
		if (!(sq_gettype(v,p_idx)&SQOBJECT_NUMERIC)) {\
			sqbind_throwerror(v,"Type is not numeric!");\
			return 0;\
		}\
		SQFloat ret;\
		sq_getfloat(v,p_idx,&ret);\
		return (m_type)ret;\
	}\
\
	static void push(HSKVM v, const m_type& p_value) {\
		sq_pushfloat(v,p_value);\
	}\
	static void push(HSKVM v, const m_type* p_value) {\
		if (!p_value)\
			sq_pushnull(v);\
		else\
			sq_pushfloat(v,*p_value);\
	}\
};


SQBIND_FLOAT(float);
SQBIND_FLOAT(double);
SQBIND_FLOAT(long double);

// bool binding

#define SQBIND_BOOL( m_type )\
template<>\
class SqBind<m_type> {\
public:\
	struct Getter {\
		SQBIND_INLINE m_type read(HSKVM v, int p_idx) {\
			return SqBind<m_type>::read(v,p_idx);\
		}\
	};\
	struct GetterPtr {\
		m_type temp;\
		SQBIND_INLINE m_type* read(HSKVM v, int p_idx) {\
			temp=SqBind<m_type>::read(v,p_idx);\
			return &temp;\
		}\
	};\
	typedef m_type Type;\
	static bool is_type(HSKVM v, int p_idx) {\
		if (sq_gettype(v,p_idx)&SQOBJECT_NUMERIC) {\
			return true;\
		} else if (sq_gettype(v,p_idx)==OT_BOOL) {\
			return true;\
		} \
		return false;\
	}\
	static m_type read(HSKVM v, int p_idx) {\
		if (sq_gettype(v,p_idx)&SQOBJECT_NUMERIC) {\
			int ret;\
			sq_getinteger(v,p_idx,&ret);\
			return (m_type)ret;\
		} else if (sq_gettype(v,p_idx)==OT_BOOL) {\
			bool ret;\
			sq_getbool(v,p_idx,&ret);\
			return (m_type)ret;\
		} else {		\
			sqbind_throwerror(v,"Type is not numeric!");\
			return false;\
		}\
	}\
\
	static void push(HSKVM v, const m_type& p_value) {\
		sq_pushbool(v,p_value);\
	}\
	static void push(HSKVM v, const m_type* p_value) {\
		if (!p_value)\
			sq_pushnull(v);\
		else\
			sq_pushbool(v,*p_value);\
	}\
};

SQBIND_BOOL(bool);

#if 0

// this is an example class on how to bind your own string type to native
// in this example, std::string is used.

template<>
class SqBind<std::string> {
public:
	struct Getter {
		SQBIND_INLINE std::string read(HSKVM v, int p_idx) {
			return SqBind<std::string>::read(v,2);
		}
	};
	struct GetterPtr {
		std::string temp;
		SQBIND_INLINE std::string* read(HSKVM v, int p_idx) {
			temp=SqBind<std::string>::read(v,2);
			return &temp;
		}
	};
	static std::string read(HSKVM v, int p_idx) {
		if (sq_gettype(v,p_idx)!=OT_STRING) {
			sqbind_throwerror(v,"Type is not string!");
			return std::string();
		}
		const SQChar * str;
		sq_getstring(v,p_idx,&str);
		return std::string(str);
	}

	static void push(HSKVM v, const std::string& p_value) {
		sq_pushstring(v,p_value.c_str(),-1);
	}
};

#endif



/* HELPERS FOR sqbind_method */

template<class M>
void sqbind_push_method_userdata(HSKVM v, M method ) {

	union {

		M m;
		unsigned char b[sizeof(M)];
	} _mu;

	_mu.m=method;
	unsigned char *ptr =(unsigned char*)sq_newuserdata(v, sizeof(M));
	for (size_t i=0;i<sizeof(M);i++)
		ptr[i]=_mu.b[i];
}

template<class T>
struct SqCParam {
	typename SqBind<T>::Getter getter;
};

template<class T>
struct SqCParam<T&> {
	typename SqBind<T>::Getter getter;
};

template<class T>
struct SqCParam<const T&> {
	typename SqBind<T>::Getter getter;
};

template<class T>
struct SqCParam<const T> {
	typename SqBind<T>::Getter getter;
};

template<class T>
struct SqCParam<T*> {

	typename SqBind<T>::GetterPtr getter;
};

template<class T>
struct SqCParam<const T*> {

	typename SqBind<T>::GetterPtr getter;
};

template<class T, class M>
static void sqbind_push_method_userdata(HSKVM v, M method ) {

	union {

		M m;
		unsigned char b[sizeof(M)];
	} _mu;

	_mu.m=method;
	unsigned char *ptr =(unsigned char*)sq_newuserdata(v,sizeof(M));
	for (size_t i=0;i<sizeof(M);i++)
		ptr[i]=_mu.b[i];


}

// little helper


#define _SQBC( m_type ) SqBind<typename SqBindSimpleType<m_type>::type_t>

#include "sqmethods.inc"

#ifdef SQBIND_NAMESPACE

}; // end of namespace
#endif


#endif
