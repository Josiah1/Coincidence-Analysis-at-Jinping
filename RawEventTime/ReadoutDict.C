// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME ReadoutDict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "Readout.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *Readout_Dictionary();
   static void Readout_TClassManip(TClass*);
   static void *new_Readout(void *p = 0);
   static void *newArray_Readout(Long_t size, void *p);
   static void delete_Readout(void *p);
   static void deleteArray_Readout(void *p);
   static void destruct_Readout(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Readout*)
   {
      ::Readout *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Readout));
      static ::ROOT::TGenericClassInfo 
         instance("Readout", "Readout.h", 18,
                  typeid(::Readout), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &Readout_Dictionary, isa_proxy, 0,
                  sizeof(::Readout) );
      instance.SetNew(&new_Readout);
      instance.SetNewArray(&newArray_Readout);
      instance.SetDelete(&delete_Readout);
      instance.SetDeleteArray(&deleteArray_Readout);
      instance.SetDestructor(&destruct_Readout);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Readout*)
   {
      return GenerateInitInstanceLocal((::Readout*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Readout*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *Readout_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Readout*)0x0)->GetClass();
      Readout_TClassManip(theClass);
   return theClass;
   }

   static void Readout_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_Readout(void *p) {
      return  p ? new(p) ::Readout : new ::Readout;
   }
   static void *newArray_Readout(Long_t nElements, void *p) {
      return p ? new(p) ::Readout[nElements] : new ::Readout[nElements];
   }
   // Wrapper around operator delete
   static void delete_Readout(void *p) {
      delete ((::Readout*)p);
   }
   static void deleteArray_Readout(void *p) {
      delete [] ((::Readout*)p);
   }
   static void destruct_Readout(void *p) {
      typedef ::Readout current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Readout

namespace {
  void TriggerDictionaryInitialization_ReadoutDict_Impl() {
    static const char* headers[] = {
"Readout.h",
0
    };
    static const char* includePaths[] = {
"/home/jinping/sw/root6.10.08/include",
"/work/wangzhe10_work/JinjingLi/work/Analysis/Coincidence/RawEventTime/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "ReadoutDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$Readout.h")))  Readout;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "ReadoutDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "Readout.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"Readout", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("ReadoutDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_ReadoutDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_ReadoutDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_ReadoutDict() {
  TriggerDictionaryInitialization_ReadoutDict_Impl();
}
