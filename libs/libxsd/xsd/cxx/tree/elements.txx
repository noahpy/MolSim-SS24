// file      : xsd/cxx/tree/elements.txx
// license   : GNU GPL v2 + exceptions; see accompanying LICENSE file

#include <xercesc/util/XMLUniDefs.hpp>

namespace xsd
{
  namespace cxx
  {
    namespace tree
    {
      // user_data_keys_template
      //
      template <int dummy>
      const XMLCh user_data_keys_template<dummy>::node[21] =
      {xercesc::chLatin_x, xercesc::chLatin_s, xercesc::chLatin_d, // xsd
       xercesc::chColon, xercesc::chColon,                         // ::
       xercesc::chLatin_c, xercesc::chLatin_x, xercesc::chLatin_x, // cxx
       xercesc::chColon, xercesc::chColon,                         // ::
       xercesc::chLatin_t, xercesc::chLatin_r, xercesc::chLatin_e, // tre
       xercesc::chLatin_e, xercesc::chColon, xercesc::chColon,     // e::
       xercesc::chLatin_n, xercesc::chLatin_o, xercesc::chLatin_d, // nod
       xercesc::chLatin_e, xercesc::chNull                         // e\0
      };


      // simple_type
      //
      template <typename C, typename B>
      simple_type<C, B>::
      simple_type (const simple_type& other,
                   flags f,
                   container* c)
          : B (other, f, c)
      {
      }

      template <typename C, typename B>
      simple_type<C, B>* simple_type<C, B>::
      _clone (flags f, container* c) const
      {
        return new simple_type (*this, f, c);
      }

      // fundamental_base
      //
      template <typename T, typename C, typename B, schema_type::value ST>
      fundamental_base<T, C, B, ST>* fundamental_base<T, C, B, ST>::
      _clone (flags f, container* c) const
      {
        return new fundamental_base (*this, f, c);
      }
    }
  }
}
