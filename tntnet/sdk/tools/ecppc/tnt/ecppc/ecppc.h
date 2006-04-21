/* tnt/ecppc/ecppc.h
 * Copyright (C) 2005 Tommi Maekitalo
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

#ifndef TNT_ECPPC_ECPPC_H
#define TNT_ECPPC_ECPPC_H

#include <cxxtools/arg.h>
#include <exception>
#include <iosfwd>
#include <tnt/ecpp/parsehandler.h>
#include <set>

namespace tnt
{
  namespace ecppc
  {
    class Ecppc
    {
        std::string requestname;
        std::string extname;
        const char* inputfile;

        typedef std::set<std::string> inputfiles_type;
        inputfiles_type inputfiles;

        std::string ns;
        std::string ofile;
        std::string odir;
        cxxtools::Arg<std::string> mimetype;
        cxxtools::Arg<std::string> mimedb;
        cxxtools::Arg<bool> binary;
        cxxtools::Arg<bool> multibinary;
        cxxtools::Arg<bool> singleton;
        cxxtools::Arg<std::string> componentclass;
        cxxtools::Arg<std::string> baseclass;
        cxxtools::Arg<bool> compress;
        cxxtools::Arg<bool> verbose;
        cxxtools::Arg<bool> debug;
        cxxtools::Arg<bool> trace;
        cxxtools::Arg<bool> generateDependencies;
        cxxtools::Arg<bool> generateHeader;

        int runDepencencies();
        int runGenerator();
        void runParser(std::istream& in, tnt::ecpp::ParseHandler& handler);

      public:
        Ecppc(int& argc, char* argv[]);
        int run();
    };

    class Usage : public std::exception
    {
        std::string msg;
      public:
        Usage(const char* progname);
        ~Usage() throw()  { }

        const char* what() const throw()
        { return msg.c_str(); }
    };
  }
}

#endif // TNT_ECPPC_ECPPC_H

