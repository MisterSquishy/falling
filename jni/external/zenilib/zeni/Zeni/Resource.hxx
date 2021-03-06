/* This file is part of the Zenipex Library (zenilib).
 * Copyright (C) 2011 Mitchell Keith Bloch (bazald).
 *
 * zenilib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * zenilib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with zenilib.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ZENI_RESOURCE_HXX
#define ZENI_RESOURCE_HXX

#include <Zeni/Resource.h>

namespace Zeni {

  unsigned long Resource::assign() {
    //Mutex::Lock lock(m_mutex);

    unsigned long rv = ++m_current;
    if(!rv) {
      --m_current;
      throw Resource_Init_Failure();
    }
    return rv;
  }

}

#endif
