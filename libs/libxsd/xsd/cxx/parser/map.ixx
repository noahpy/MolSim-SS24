// file      : xsd/cxx/parser/map.ixx
// license   : GNU GPL v2 + exceptions; see accompanying LICENSE file

namespace xsd
{
  namespace cxx
  {
    namespace parser
    {
      // parser_map_impl
      //
      template <typename C>
      inline parser_map_impl<C>::parser_map_impl ()
      {
      }

      template <typename C>
      inline void parser_map_impl<C>::
      insert (parser_base<C>& parser)
      {
        map_[parser._dynamic_type ()] = &parser;
      }
    }
  }
}
