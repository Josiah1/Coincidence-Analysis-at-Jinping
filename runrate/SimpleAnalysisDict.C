// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME SimpleAnalysisDict

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
#include "SimpleAnalysis.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_SimpleAnalysis(void *p = 0);
   static void *newArray_SimpleAnalysis(Long_t size, void *p);
   static void delete_SimpleAnalysis(void *p);
   static void deleteArray_SimpleAnalysis(void *p);
   static void destruct_SimpleAnalysis(void *p);
   static void streamer_SimpleAnalysis(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SimpleAnalysis*)
   {
      ::SimpleAnalysis *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::SimpleAnalysis >(0);
      static ::ROOT::TGenericClassInfo 
         instance("SimpleAnalysis", ::SimpleAnalysis::Class_Version(), "SimpleAnalysis.h", 22,
                  typeid(::SimpleAnalysis), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::SimpleAnalysis::Dictionary, isa_proxy, 16,
                  sizeof(::SimpleAnalysis) );
      instance.SetNew(&new_SimpleAnalysis);
      instance.SetNewArray(&newArray_SimpleAnalysis);
      instance.SetDelete(&delete_SimpleAnalysis);
      instance.SetDeleteArray(&deleteArray_SimpleAnalysis);
      instance.SetDestructor(&destruct_SimpleAnalysis);
      instance.SetStreamerFunc(&streamer_SimpleAnalysis);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SimpleAnalysis*)
   {
      return GenerateInitInstanceLocal((::SimpleAnalysis*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::SimpleAnalysis*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr SimpleAnalysis::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *SimpleAnalysis::Class_Name()
{
   return "SimpleAnalysis";
}

//______________________________________________________________________________
const char *SimpleAnalysis::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SimpleAnalysis*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int SimpleAnalysis::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SimpleAnalysis*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *SimpleAnalysis::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SimpleAnalysis*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *SimpleAnalysis::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SimpleAnalysis*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void SimpleAnalysis::Streamer(TBuffer &R__b)
{
   // Stream an object of class SimpleAnalysis.

   TSelector::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_SimpleAnalysis(void *p) {
      return  p ? new(p) ::SimpleAnalysis : new ::SimpleAnalysis;
   }
   static void *newArray_SimpleAnalysis(Long_t nElements, void *p) {
      return p ? new(p) ::SimpleAnalysis[nElements] : new ::SimpleAnalysis[nElements];
   }
   // Wrapper around operator delete
   static void delete_SimpleAnalysis(void *p) {
      delete ((::SimpleAnalysis*)p);
   }
   static void deleteArray_SimpleAnalysis(void *p) {
      delete [] ((::SimpleAnalysis*)p);
   }
   static void destruct_SimpleAnalysis(void *p) {
      typedef ::SimpleAnalysis current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_SimpleAnalysis(TBuffer &buf, void *obj) {
      ((::SimpleAnalysis*)obj)->::SimpleAnalysis::Streamer(buf);
   }
} // end of namespace ROOT for class ::SimpleAnalysis

namespace {
  void TriggerDictionaryInitialization_SimpleAnalysisDict_Impl() {
    static const char* headers[] = {
"SimpleAnalysis.h",
0
    };
    static const char* includePaths[] = {
"/home/jinping/sw/root6.10.08/include",
"/work/wangzhe10_work/JinjingLi/work/Analysis/Coincidence/eventrate_pedestal/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "SimpleAnalysisDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$SimpleAnalysis.h")))  SimpleAnalysis;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "SimpleAnalysisDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "SimpleAnalysis.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"SimpleAnalysis", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("SimpleAnalysisDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_SimpleAnalysisDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_SimpleAnalysisDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_SimpleAnalysisDict() {
  TriggerDictionaryInitialization_SimpleAnalysisDict_Impl();
}
