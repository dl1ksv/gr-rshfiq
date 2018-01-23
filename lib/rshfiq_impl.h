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

#ifndef INCLUDED_RSHFIQ_RSHFIQ_IMPL_H
#define INCLUDED_RSHFIQ_RSHFIQ_IMPL_H

#include <rshfiq/rshfiq.h>
#include <gnuradio/audio/source.h>

#include <string>

namespace gr {
  namespace rshfiq {

    class rshfiq_impl : public rshfiq
    {
     private:
      gr::audio::source::sptr d_soundcard; /*!< The audio input source */
      int d_fd;                         /*!< File descriptor to control the device, set frequency, etc */
      unsigned int d_freq_req;           /*!< The latest requested frequency in Hz */

      void set_rs_hfiq_freq(unsigned int f);
      int read_string(char *answer,int length);

     public:
      rshfiq_impl(const std::string audio_device_name,const std::string control_device,int frequeny);
      ~rshfiq_impl();
     void set_freq(float freq);
     std::string d_device;
    };

  } // namespace rshfiq
} // namespace gr

#endif /* INCLUDED_RSHFIQ_RSHFIQ_IMPL_H */

