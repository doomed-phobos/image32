#include "string.h"

#include <iterator>
#include <cstdarg>
#include <vector>

std::string string_to_lower(const std::string& str)
{
   std::string result(str);

   for(auto it = result.begin(); it != result.end(); it++)
      *it = std::tolower(*it);

   return result;
}

std::string format_to_string(const char* format, ...)
{
   va_list args;
   va_start(args, format);
   std::size_t required_size = vsnprintf(nullptr, 0, format, args);
   std::vector<char> buf(++required_size);

   vsnprintf(&buf[0], buf.size(), format, args);
   
   va_end(args);

   return std::string(&buf[0]);
}

static size_t insert_utf8_char(std::string* result, wchar_t wchr)
{
   int size, bits, b, i;

   if (wchr < 128) {
      if (result)
      result->push_back(wchr);
      return 1;
   }

   bits = 7;
   while (wchr >= (1<<bits))
      bits++;

   size = 2;
   b = 11;

   while (b < bits) {
      size++;
      b += 5;
   }

   if (result) {
      b -= (7-size);
      int firstbyte = wchr>>b;
      for (i=0; i<size; i++)
      firstbyte |= (0x80>>i);

      result->push_back(firstbyte);

      for (i=1; i<size; i++) {
      b -= 6;
      result->push_back(0x80 | ((wchr>>b)&0x3F));
      }
   }

   return size;
}

std::string to_utf8(const wchar_t* wsrc, const int n)
{
   size_t required_size = 0;
   const wchar_t* p = wsrc;

   for(int i = 0; i < n; i++, ++p)
      required_size += insert_utf8_char(nullptr, *p);
   
   if(!required_size)
      return std::string();

   std::string result;
   result.reserve(++required_size);

   for(int i = 0; i < n; i++, ++wsrc)
      insert_utf8_char(&result, *wsrc);

   return result;
}

template<typename BaseIterator>
class utf8_iterator_t : public std::iterator<std::forward_iterator_tag,
                                             std::string::value_type,
                                             std::string::difference_type,
                                             typename BaseIterator::pointer,
                                             typename BaseIterator::reference>
{
public:
   typedef typename BaseIterator::pointer pointer;

   utf8_iterator_t() {}
   explicit utf8_iterator_t(const BaseIterator& it) : m_internal(it) {}

   utf8_iterator_t& operator++() {
      int c = *m_internal;
      ++m_internal;

      if (c & 0x80) {
        int n = 1;
        while (c & (0x80>>n))
          n++;

        c &= (1<<(8-n))-1;

        while (--n > 0) {
          int t = *m_internal;
          ++m_internal;

          if ((!(t & 0x80)) || (t & 0x40)) {
            --m_internal;
            return *this;
          }

          c = (c<<6) | (t & 0x3F);
        }
      }

      return *this;
   }

   utf8_iterator_t& operator+=(int sum) {
      while(sum--)
         operator++();
      return *this;
   }

   utf8_iterator_t operator+(int i) {
      utf8_iterator_t it(*this);
      it += i;
      return it;
    }

   const int operator*() const {
      BaseIterator it = m_internal;
      int c = *it;
      ++it;

      if (c & 0x80) {
         int n = 1;
         while (c & (0x80>>n))
            n++;

         c &= (1<<(8-n))-1;

         while (--n > 0) {
            int t = *it;
            ++it;

            if ((!(t & 0x80)) || (t & 0x40))
               return '^';

            c = (c<<6) | (t & 0x3F);
         }
      }  

      return c;
   }

   bool operator==(const utf8_iterator_t& it) const {
      return m_internal == it.m_internal;
   }

   bool operator!=(const utf8_iterator_t& it) const {
      return m_internal != it.m_internal;
   }

   pointer operator->() {
      return m_internal.operator->();
   }

   std::string::difference_type operator-(const utf8_iterator_t& it) const {
      return m_internal - it.m_internal;
   }
private:
   BaseIterator m_internal;
};

class utf8_iterator : public utf8_iterator_t<std::string::iterator> {
public:
   utf8_iterator() { }
   utf8_iterator(const utf8_iterator_t<std::string::iterator>& it)
   : utf8_iterator_t<std::string::iterator>(it) {
   }
   explicit utf8_iterator(const std::string::iterator& it)
   : utf8_iterator_t<std::string::iterator>(it) {
   }
};

class utf8_const_iterator : public utf8_iterator_t<std::string::const_iterator> {
public:
   utf8_const_iterator() { }
   utf8_const_iterator(const utf8_iterator_t<std::string::const_iterator>& it)
   : utf8_iterator_t<std::string::const_iterator>(it) {
   }
   explicit utf8_const_iterator(const std::string::const_iterator& it)
   : utf8_iterator_t<std::string::const_iterator>(it) {
   }
};

class utf8 {
public:
   utf8(std::string& s) : m_begin(utf8_iterator(s.begin())),
                        m_end(utf8_iterator(s.end())) {
   }
   const utf8_iterator& begin() const { return m_begin; }
   const utf8_iterator& end() const { return m_end; }
private:
   utf8_iterator m_begin;
   utf8_iterator m_end;
};

class utf8_const {
public:
   utf8_const(const std::string& s) : m_begin(utf8_const_iterator(s.begin())),
                                    m_end(utf8_const_iterator(s.end())) {
   }
   const utf8_const_iterator& begin() const { return m_begin; }
   const utf8_const_iterator& end() const { return m_end; }
private:
   utf8_const_iterator m_begin;
   utf8_const_iterator m_end;
};

int utf8_length(const std::string& str)
{
   utf8_const_iterator it(str.begin());
   utf8_const_iterator end(str.end());
   int size = 0;

   while(it != end)
      ++it, ++size;

   return size;
}

std::wstring from_utf8(const std::string& str)
{
   int required_size = utf8_length(str);
   std::vector<wchar_t> buf(++required_size);
   std::vector<wchar_t>::iterator buf_it = buf.begin();
   utf8_const_iterator it(str.begin());
   utf8_const_iterator end(str.end());

   while (it != end) {
      *buf_it = *it;
      ++buf_it;
      ++it;
   }

   return std::wstring(&buf[0]);
}