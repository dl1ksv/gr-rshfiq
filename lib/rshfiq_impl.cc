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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <gnuradio/blocks/float_to_complex.h>

#include <iostream>
#include <fstream>
#include <exception>

#include "rshfiq_impl.h"
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


namespace gr {
  namespace rshfiq {

    rshfiq::sptr
    rshfiq::make(const std::string audio_device_name,const std::string control_device,int frequeny)
    {
      return gnuradio::get_initial_sptr
        (new rshfiq_impl(audio_device_name, control_device, frequeny));
    }

    /*
     * The private constructor
     */
    rshfiq_impl::rshfiq_impl(const std::string audio_device_name,const std::string control_device,int frequeny)
      : gr::hier_block2("rshfiq",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
      struct termios options;
      std::string device_name;
      gr::blocks::float_to_complex::sptr f2c;

      char cmd[]={'*','w','\r','\n',}; /* get version */
      char answer[20];
      int rc;

    /*
     * First we try to find the control device
     */
      d_fd=-1;
      if(control_device.empty())
       {
        std::cerr << "No control device given! " << std::endl;
        throw std::runtime_error("Control device missing");
       }
      else
       d_fd=open(control_device.c_str(),O_RDWR| O_NOCTTY );
      if( d_fd < 0)
       {
        std::cerr << "++++Could not open control device: " << control_device << std::endl;
        throw std::runtime_error("Control device missing");
       }

      tcgetattr(d_fd, &options);
      cfsetspeed(&options, B57600);
      /** No handshake */
      options.c_cflag &= ~CRTSCTS;
      options.c_iflag &= ~IXON;
      options.c_cflag |= (CLOCAL | CREAD);
      options.c_cflag &= ~HUPCL;
      options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
      options.c_oflag &= ~OPOST;
 //     cfmakeraw(&options);

      tcsetattr(d_fd,TCSANOW,&options);
      rc=read_string(answer,20);
      if( rc > 0)
        std::cerr << "Init: " << answer << std::endl;
      else
       std::cerr << "******No init message" << std::endl;
      tcflush(d_fd,TCIFLUSH);
      rc=write(d_fd,cmd,4);
      if(rc !=4)
       {
        std::cerr << "Could not write command, rc: " << rc << std::endl;
        throw std::runtime_error("Could not initialize rig");
       }
      rc=read_string(answer,20);
      std::cerr <<"Rig version: " << answer << std::endl;
      if(!strstr(answer,"RS-HFIQ"))
       {
        d_device="unknown";
        std::cerr << "Invalid rig" << answer << std::endl;
        throw std::runtime_error("Invalid rig");
       }
      else
       d_device = answer;

      /*
       * Now let's find the soundcard
       */
      if(!audio_device_name.empty())  {
          try {
               /* Audio source; sample rate fixed at 96kHz */
               d_soundcard = gr::audio::source::make(96000, audio_device_name, true);
          }
          catch (std::exception ) {

              std::string line;
              std::cerr << "Could not open device: " <<audio_device_name << std::endl;
              std::cerr << "Try plugin device" << std::endl;

              std::ifstream cards( "/proc/asound/cards" );
              device_name.clear();
              if ( cards.is_open() )  {
                  while ( cards.good() )  {
                      getline (cards, line);

                      if ( line.find( audio_device_name ) != std::string::npos )  {
                          int id;
                          std::istringstream( line ) >> id;
                          std::cerr <<"Id: " << id << " Card: " << line << std::endl;

                          std::ostringstream hw_id;
                          hw_id << "plughw:" << id<<",0"; // build alsa identifier
                          device_name= hw_id.str();
                          break;

                      }
                  }
                  cards.close();
                  if(device_name.empty()) {
                      throw std::runtime_error("Soundcard for RS-HFIQ not found");
                  }
              }
              else {
              throw std::runtime_error("Alsa not found.");
              }
            }

            d_soundcard = gr::audio::source::make(96000, device_name, true);
          }
        else
           throw std::runtime_error("No Soundcard given");
        f2c = gr::blocks::float_to_complex::make(1);

        connect(d_soundcard, 0, f2c, 0);
        connect(d_soundcard, 1, f2c, 1);
        connect(f2c, 0, self(), 0);

    }

    /*
     * Our virtual destructor.
     */
    rshfiq_impl::~rshfiq_impl()
    {
     if(d_fd >= 0)
      close(d_fd);
    }


    int rshfiq_impl::read_string(char *answer,int length)
    {
      int rc,i;
      struct timeval tv;
      fd_set rfds, efds;

      tv.tv_sec=5;
      tv.tv_usec=0;


      FD_ZERO(&rfds);
      FD_SET(d_fd, &rfds);
      efds = rfds;

      rc = select(d_fd+1, &rfds, NULL, &efds, &tv);
      /**
      if(rc > 0)
       std::cerr << "Daten verfügbar" << std::endl;
      else
       {
        std::cerr <<"Keine Daten verfügbar, rc: " << rc<< std::endl;
        std::cerr <<"Timeout" << tv.tv_sec << std::endl;
        answer[0]=0;
        return 0;
       } */
       if (rc <=0 )
       	return 0;

      for(i=0; i < length;i++)
       {
        rc=read(d_fd,answer+i,1);
//        std::cerr << "Gelesen byte : " << i <<" rc: " << rc <<" Wert: " << answer[i] << std::endl;
        if(rc <0 )
         {
          answer[i]=0;
//          std::cerr << "Error reading byte,rc: " << rc << std::endl;
          return i;
         }
        if((answer[i] == '\n') | (answer[i] == '\r'))
         {
          answer[i]=0;
//          std::cerr << "Gelesen: " << rc << " bytes: " << answer << std::endl;
          return i;
         }
       }
      return i;
    }
    void rshfiq_impl::set_rs_hfiq_freq(unsigned int f)
    {
     int rc;
     char cmdstr[15];
     sprintf(cmdstr,"*f%u\r",f);
     rc=write(d_fd,cmdstr,strlen(cmdstr));
     if(rc != strlen(cmdstr))
      std::cerr <<"Could not set frequency to:" << f << " Hz" << std::endl;
    }
    void rshfiq_impl::set_freq(float freq)
    {
     set_rs_hfiq_freq((unsigned int)(freq));
    }
  } /* namespace rshfiq */
} /* namespace gr */

