// file      : xsd/cxx/xml/sax/std-input-source.hxx
// license   : GNU GPL v2 + exceptions; see accompanying LICENSE file

#ifndef XSD_CXX_XML_SAX_STD_INPUT_SOURCE_HXX
#define XSD_CXX_XML_SAX_STD_INPUT_SOURCE_HXX

#include <istream>

#include <xsd/cxx/xml/string.hxx>

#include <xercesc/sax/InputSource.hpp>
#include <xercesc/util/BinInputStream.hpp>

namespace xsd
{
  namespace cxx
  {
    namespace xml
    {
      namespace sax
      {
        class std_input_stream: public xercesc::BinInputStream
        {
        public:
          std_input_stream (std::istream& is)
              : is_ (is)
          {
          }

          virtual XMLFilePos
          curPos () const
          {
            return static_cast<XMLFilePos> (is_.tellg ());
          }

          virtual XMLSize_t
          readBytes (XMLByte* const buf, const XMLSize_t size)
          {
            // Some implementations don't clear gcount if you
            // call read() on a stream that is in the eof state.
            //
            if (is_.eof ())
              return 0;

            // Unset the exception failbit while we are working
            // with the stream.
            //
            std::ios_base::iostate old (is_.exceptions ());
            is_.exceptions (old & ~std::ios_base::failbit);

            is_.read (reinterpret_cast<char*> (buf),
                      static_cast<std::streamsize> (size));

            // Clear the fail bit if it was caused by eof and restore
            // the original exception state. If there are any pending
            // errors then the exception will be thrown now.
            //
            if (is_.fail () && is_.eof ())
              is_.clear (is_.rdstate () & ~std::ios_base::failbit);

            is_.exceptions (old);

            // Make sure that if we failed, readBytes won't be called
            // again.
            //
            return !is_.fail ()
              ? static_cast<XMLSize_t> (is_.gcount ())
              : 0;
          }

          virtual const XMLCh*
          getContentType () const
          {
            return 0;
          }

        private:
          std::istream& is_;
        };


        class std_input_source: public xercesc::InputSource
        {
        public:
          std_input_source (std::istream& is)
              : is_ (&is)
          {
          }

          template <typename C>
          std_input_source (std::istream& is, const C* system_id)
              : xercesc::InputSource (xml::string (system_id).c_str ()),
                is_ (&is)
          {
          }

          template <typename C>
          std_input_source (std::istream& is,
                            const std::basic_string<C>& system_id)
              : xercesc::InputSource (xml::string (system_id).c_str ()),
                is_ (&is)
          {
          }

          template <typename C>
          std_input_source (std::istream& is,
                            const C* system_id,
                            const C* public_id)
              : xercesc::InputSource (xml::string (system_id).c_str (),
                                      xml::string (public_id).c_str ()),
                is_ (&is)
          {
          }

          template <typename C>
          std_input_source (std::istream& is,
                            const std::basic_string<C>& system_id,
                            const std::basic_string<C>& public_id)
              : xercesc::InputSource (xml::string (system_id).c_str (),
                                      xml::string (public_id).c_str ()),
                is_ (&is)
          {
          }

          struct copy {};

          // Throws the copy exception if this function is called more
          // than once.
          //
          virtual xercesc::BinInputStream*
          makeStream () const
          {
            if (is_ == 0)
              throw copy ();

            std::istream& is (*is_);

            is_ = 0;

            return new std_input_stream (is);
          }

        private:
          mutable std::istream* is_;
        };
      }
    }
  }
}

#endif  // XSD_CXX_XML_SAX_STD_INPUT_SOURCE_HXX
