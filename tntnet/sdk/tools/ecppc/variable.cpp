/* variable.cpp
 * Copyright (C) 2003 Tommi Maekitalo
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

#include "tnt/ecppc/variable.h"
#include <sstream>
#include <locale>

namespace tnt
{
  namespace ecppc
  {
    Variable::Variable(const std::string& arg, const std::string& value_)
    {
      // 'name' might be prefixed by a type
      // the variablename is the last word in 'name'
      // type is the rest before the variablename
      // examples:
      //   " int var "
      //   " var"
      //   " ns :: someclass  param"
      std::ostringstream a;

      std::string::size_type e = arg.size();
      while (e > 0 && std::isspace(arg.at(e - 1), std::locale()))
        --e;

      // e points past the last character of our arg

      if (e > 1 && arg.at(e - 2) == '[' && arg.at(e-1) == ']')
      {
        isvector = true;
        e -= 2;
        while (e > 0 && std::isspace(arg.at(e - 1), std::locale()))
          --e;
      }
      else
        isvector = false;

      std::string::size_type b = e;
      while (b > 0 && !std::isspace(arg.at(b - 1), std::locale()))
        --b;
      // b points to the first character of our arg

      std::string::size_type t = b;
      while (t > 0 && std::isspace(arg.at(t - 1), std::locale()))
        --t;
      // t points past the last character of the type

      name = std::string(arg.begin() + b, arg.begin() + e);
      type = std::string(arg.begin(), arg.begin() + t);
      value = value_;

    }

    void Variable::getParamCodeVector(std::ostream& o) const
    {
      if (type.empty())
      {
        o << "typedef std::vector<std::string> " << name << "_type;\n"
          << name << "_type " << name << "(qparam.begin(\"" << name
          << "\"), qparam.end());\n";
      }
      else
      {
        o << "typedef std::vector<" << type << "> " << name << "_type;\n"
          << name << "_type " << name << ";\n"
          << "std::transform(qparam.begin(\"" << name
          << "\"), qparam.end(), std::back_inserter(" << name
          << "), tnt::stringToConverter<" << type
          << ">(reply.out().getloc()));\n";
      }
    }

    void Variable::getParamCode(std::ostream& o) const
    {
      if (isvector)
        getParamCodeVector(o);
      else if (!type.empty())
      {
        // we have a type

        // print out type and name
        o << type << ' ' << name << " = ";

        if (value.empty())
        {
          // no default-value

          o << "tnt::stringToConverter<" << type
            << ">(reply.out().getloc())( qparam.param(\"" << name << "\") );\n";
        }
        else
        {
          // with default-value
          o << "qparam.has(\"" << name << "\") ? tnt::stringToWithDefaultConverter<"
            << type << ">(" << value << ", reply.out().getloc())(qparam.param(\""
            << name << "\")) : " << value << ";\n";
        }
      }
      else
      {
        // type defaults to std::string
        o << "std::string " << name 
          << " = qparam.param(\"" << name << '"';
        if (!value.empty())
          o << ", " << value;

        o << ");\n";
      }
    }

    void Variable::getConfigInit(std::ostream& o, const std::string& classname) const
    {
      if (!type.empty())
      {
        // we have a type

        o << "    if (config.hasValue(\"" << name << "\"))\n"
          << "      " << classname << "::" << name << " = tnt::stringTo<" << type
          << ">( config.getValue(\"" << name << "\") );\n";
      }
      else
      {
        // type defaults to std::string
        if (value.empty())
          o << "    " << classname << "::" << name 
            << " = config.getValue(\"" << name << "\");\n";
        else
          o << "    if (config.hasValue(\"" << name << "\"))\n"
            << "      " << classname << "::" << name << " = config.getValue(\""
            << name << "\");\n";
      }
    }

    void Variable::getConfigDecl(std::ostream& o, const std::string& classname) const
    {
      std::string t = type.empty() ? "std::string" : type;

      o << t << ' ' << classname << "::" << name;
      if (!value.empty())
        o << " = " << value;
      o << ";\n";
    }

    void Variable::getConfigHDecl(std::ostream& o) const
    {
      std::string t = type.empty() ? "std::string" : type;

      o << "    static " << t << " " << name << ";\n";
    }
  }
}
