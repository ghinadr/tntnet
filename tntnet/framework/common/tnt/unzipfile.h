/* tnt/unzip_pp.h
 * Copyright (C) 2003/2006 Tommi Maekitalo
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * is provided AS IS, WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, and
 * NON-INFRINGEMENT.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 */

#ifndef TNT_UNZIPFILE_H
#define TNT_UNZIPFILE_H

#include <stdexcept>
#include <iostream>
#include <string>

namespace tnt
{
  class unzipError : public std::runtime_error
  {
      int err;

      static std::string formatMsg(int e, const char* msg, const char* function);

    public:
      unzipError(int e, const std::string& msg = "unzipError")
        : std::runtime_error(msg),
          err(e)
          { }
      unzipError(int e, const char* msg, const char* function)
        : std::runtime_error(formatMsg(e, msg, function)),
          err(e)
          { }

      int getErr() const  { return err; }
  };

  class unzipFileNotFound : public unzipError
  {
    public:
      unzipFileNotFound(const std::string& file)
        : unzipError(0, "file not found " + file)
        { }
  };

  class unzipEndOfListOfFile : public unzipError
  {
    public:
      unzipEndOfListOfFile(const char* function = 0);
  };

  class unzipParamError : public unzipError
  {
    public:
      unzipParamError(const char* function = 0);
  };

  class unzipBadZipFile : public unzipError
  {
    public:
      unzipBadZipFile(const char* function = 0);
  };

  class unzipInternalError : public unzipError
  {
    public:
      unzipInternalError(const char* function = 0);
  };

  class unzipCrcError : public unzipError
  {
    public:
      unzipCrcError(const char* function = 0);
  };

  class unzipFile
  {
      struct unzFileStruct;
      unzFileStruct* file;

    public:
      unzipFile()
        : file(0)
        { }

      unzipFile(const std::string& path)
        : file(0)
      { open(path); }

      ~unzipFile();

      void open(const std::string& path);
      void close();

      void goToFirstFile();
      void goToNextFile();
      void locateFile(const std::string& fileName, bool caseSensitivity = true);
      void openCurrentFile();
      void openCurrentFile(const std::string& pw);
      void closeCurrentFile();
      int readCurrentFile(void* buf, unsigned len);
  };

  class unzipFileStreamBuf : public std::streambuf
  {
      char_type buffer[512];
      unzipFile& file;

    public:
      unzipFileStreamBuf(unzipFile& f, const std::string& fileName, bool caseSensitivity)
        : file(f)
      {
        f.locateFile(fileName, caseSensitivity);
        f.openCurrentFile();
      }

      unzipFileStreamBuf(unzipFile& f, const std::string& fileName, bool caseSensitivity,
        const std::string& password)
        : file(f)
      {
        f.locateFile(fileName, caseSensitivity);
        f.openCurrentFile(password);
      }

      ~unzipFileStreamBuf()
      { file.closeCurrentFile(); }

      /// overridden from std::streambuf
      int_type overflow(int_type c);
      /// overridden from std::streambuf
      int_type underflow();
      /// overridden from std::streambuf
      int sync();
  };

  class unzipFileStream : public std::istream
  {
      unzipFileStreamBuf streambuf;

    public:
      unzipFileStream(unzipFile& f, const std::string& fileName, bool caseSensitivity = true)
        : std::istream(&streambuf),
          streambuf(f, fileName, caseSensitivity)
      { }
  };

}

#endif // UNZIPFILE_H

