/*COPYRIGHT**
    Copyright (C) 2012-2014 Intel Corporation.  All Rights Reserved.

    This file is part of SEP Development Kit

    SEP Development Kit is free software; you can redistribute it
    and/or modify it under the terms of the GNU General Public License
    version 2 as published by the Free Software Foundation.

    SEP Development Kit is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SEP Development Kit; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    As a special exception, you may use this file as part of a free software
    library without restriction.  Specifically, if other files instantiate
    templates or use macros or inline functions from this file, or you compile
    this file and link it with other files to produce an executable, this
    file does not by itself cause the resulting executable to be covered by
    the GNU General Public License.  This exception does not however
    invalidate any other reasons why the executable file might be covered by
    the GNU General Public License.
**COPYRIGHT*/

#ifndef _IVYTOWN_PCU_H_INC_
#define _IVYTOWN_PCU_H_INC_

#define  ENABLE_ALL_PMU                          0x20000000
#define  DISABLE_ALL_PMU                         0x80000000

#define  IVYTOWN_PCU_MSR_PMON_BOX_CTL            0xC24
#define  DISABLE_PCU_COUNTERS                    0x100
#define  ENABLE_PCU_COUNTERS                     0x400000

#define IVYTOWN_PCU_MSR_PMON_CTL0                0xc30
#define IVYTOWN_PCU_MSR_PMON_CTL1                0xc31
#define IVYTOWN_PCU_MSR_PMON_CTL2                0xc32
#define IVYTOWN_PCU_MSR_PMON_CTL3                0xc33

#define IS_THIS_PCU_BOX_EVSEL_CTL_MSR(x)       ( x == IVYTOWN_PCU_MSR_PMON_CTL0       \
                                                   || x == IVYTOWN_PCU_MSR_PMON_CTL1  \
                                                   || x == IVYTOWN_PCU_MSR_PMON_CTL2  \
                                                   || x == IVYTOWN_PCU_MSR_PMON_CTL3 )

extern  DISPATCH_NODE                            ivytown_pcu_dispatch;

#endif
