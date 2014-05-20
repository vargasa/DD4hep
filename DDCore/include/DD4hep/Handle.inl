// $Id: Handle.h 1059 2014-04-04 20:24:53Z markus.frank@cern.ch $
//====================================================================
//  AIDA Detector description implementation for LCD
//--------------------------------------------------------------------
//
//  Author     : M.Frank
//
//====================================================================

// Framework include files
#include "DD4hep/Handle.h"

/*
 *   DD4hep namespace declaration
 */
namespace DD4hep {

  /*
   *   Geometry sub-namespace declaration
   */
  namespace Geometry {

    template <typename T> void Handle<T>::bad_assignment(const std::type_info& from, const std::type_info& to) {
      invalidHandleAssignmentError(from,to);
    }
    template <typename T> void Handle<T>::assign(T* n, const std::string& nam, const std::string& tit) {
      this->m_element = n;
      if (!nam.empty())
        n->SetName(nam.c_str());
      if (!tit.empty())
        n->SetTitle(tit.c_str());
    }

    template <typename T> const char* Handle<T>::name() const {
      return this->m_element ? this->m_element->GetName() : "";
    }

    /// Checked object access. Throws invalid handle runtime exception
    template <typename T> T* Handle<T>::access() const   {
      if ( this->m_element ) return this->m_element;
      invalidHandleError(typeid(T));
      return 0; // We have thrown an exception before - does not harm!
    }

  } /* End namespace Geometry  */
} /* End namespace DD4hep      */

#define DD4HEP_INSTANTIATE_HANDLE(X)                                    \
  namespace DD4hep { namespace Geometry {                               \
  template <> void Handle<X>::verifyObject() const {	                \
    increment_object_validations();					\
    if (m_element && dynamic_cast<X*>((TObject*)m_element) == 0) {	\
      bad_assignment(typeid(*m_element), typeid(X));			\
    }									\
  }}}									\
  template struct DD4hep::Geometry::Handle<X>

#define DD4HEP_INSTANTIATE_HANDLE_NAMED(X)                              \
  namespace DD4hep { namespace Geometry {                               \
  template <> const char* Handle<X>::name() const                       \
  { return this->m_element ? this->m_element->name.c_str() : ""; }      \
  template <> void                                                      \
  Handle<X>::assign(X* p, const std::string& n, const std::string& t) { \
      this->m_element = p;                                              \
      p->name = n;                                                      \
      p->type = t;                                                      \
  }                                                                     \
  template <> void Handle<X>::verifyObject() const {	                \
    increment_object_validations();					\
    if (m_element && dynamic_cast<X*>((TObject*)m_element) == 0) {	\
      bad_assignment(typeid(*m_element), typeid(X));			\
    }									\
  }}}									\
  template struct DD4hep::Geometry::Handle<X>

#define DD4HEP_INSTANTIATE_HANDLE_UNNAMED(X)                            \
  namespace DD4hep { namespace Geometry {                               \
  template <> void                                                      \
  Handle<X>::assign(X* n, const std::string&, const std::string&)       \
  { this->m_element = n;}                                               \
  template <> const char* Handle<X>::name() const { return ""; }        \
  }}                                                                    \
  DD4HEP_INSTANTIATE_HANDLE(X)