/* -*- c++ -*- */
/* 
 * Copyright 2017 dl1ksv.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */


#ifndef INCLUDED_RSHFIQ_RSHFIQ_H
#define INCLUDED_RSHFIQ_RSHFIQ_H

#include <rshfiq/api.h>
#include <gnuradio/hier_block2.h>

#include <string>

namespace gr {
  namespace rshfiq {

    /*!
     * \brief <+description of block+>
     * \ingroup rshfiq
     *
     */
    class RSHFIQ_API rshfiq : virtual public hier_block2
    {
     public:
      typedef boost::shared_ptr<rshfiq> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of rshfiq::rshfiq.
       *
       * To avoid accidental use of raw pointers, rshfiq::rshfiq's
       * constructor is in a private implementation
       * class. rshfiq::rshfiq::make is the public interface for
       * creating new instances.
       */
      static sptr make(const std::string audio_device_name,const std::string control_device="/dev/ttyACM0",int frequeny=3500000);

      virtual void set_freq(float freq) = 0;
    };

  } // namespace rshfiq
} // namespace gr

#endif /* INCLUDED_RSHFIQ_RSHFIQ_H */

