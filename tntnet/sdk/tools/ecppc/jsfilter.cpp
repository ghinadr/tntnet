/* jsfilter.cpp
 * Copyright (C) 2003-2005 Tommi Maekitalo
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

#include "tnt/ecppc/jsfilter.h"
#include <iostream>
#include <locale>

namespace tnt
{
  namespace ecppc
  {
    ////////////////////////////////////////////////////////////////////////
    // Jsfilter
    //
    void Jsfilter::operator() (char ch)
    {
      switch (state)
      {
        case state_start:
          if (ch == '/')
            state = state_slash;
          else
          {
            if (std::isspace(ch, std::locale()))
            {
              state = state_space;
            }
            else if (ch == '"' || ch == '\'')
            {
              delim = ch;
              state = state_string;
            }
            out << ch;
          }
          break;

        case state_slash:
          if (ch == '*')
          {
            out << ' ';
            state = state_comment;
          }
          else if (ch == '/')
          {
            out << ' ';
            state = state_comment1;
          }
          else
          {
            out << '/' << ch;
            state = state_start;
          }
          break;

        case state_space:
          if (ch == '/')
            state = state_slash;
          else if (!std::isspace(ch, std::locale()))
          {
            out << ch;
            state = state_start;
          }
          break;

        case state_comment:
          if (ch == '*')
            state = state_comment_end;
          break;

        case state_comment_end:
          if (ch == '/')
            state = state_space;
          else if (ch != '*')
            state = state_comment;
          break;

        case state_comment1:
          if (ch == '\n')
            state = state_space;
          break;

        case state_string:
          if (ch == '\\')
            state = state_stringesc;
          else
          {
            out << ch;
            if (ch == delim)
              state = state_start;
          }
          break;

        case state_stringesc:
          out << ch;
          break;
      }
    }
  }
}
