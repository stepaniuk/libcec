#pragma once

/*
 * boblight
 * Copyright (C) Bob  2009 
 * 
 * boblight is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * boblight is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "os-dependent.h"
#include "baudrate.h"
#include <string>
#include <vector>
#include <stdint.h>

#ifndef __WINDOWS__
#include <termios.h>
#else
#include "../util/threads.h"
#include "../util/buffer.h"
#endif

namespace CEC
{
  #define PAR_NONE 0
  #define PAR_EVEN 1
  #define PAR_ODD  2

  class CSerialPort
  {
    public:
      CSerialPort();
      virtual ~CSerialPort();

      bool Open(std::string name, int baudrate, int databits = 8, int stopbits = 1, int parity = PAR_NONE);
      bool SetBaudRate(int baudrate);
      bool IsOpen() const;
      void Close();

      int  Write(std::vector<uint8_t> data)
      {
        return Write(&data[0], data.size());
      }
      int  Write(uint8_t* data, int len);
      int  Read(uint8_t* data, int len, int iTimeoutMs = -1);

      std::string GetError() { return m_error; }
      std::string GetName() { return m_name; }

  private:
      std::string     m_error;
      std::string     m_name;

  #ifdef __WINDOWS__
      bool SetTimeouts(bool bBlocking);

      HANDLE             m_handle;
      bool               m_bIsOpen;
      int                m_iBaudrate;
      int                m_iDatabits;
      int                m_iStopbits;
      int                m_iParity;
      int64_t            m_iTimeout;
      CecBuffer<uint8_t> m_buffer;
      HANDLE             m_ovHandle;
  #else
      struct termios     m_options;
      int                m_fd;
  #endif
  };
};