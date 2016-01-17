/* GNU/Linux S/390 specific low level interface, for the in-process
   agent library for GDB.

   Copyright (C) 2016 Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include "server.h"
#include "tracepoint.h"
#include <sys/auxv.h>

#ifndef HWCAP_S390_HIGH_GPRS
#define HWCAP_S390_HIGH_GPRS 512
#endif

#ifndef HWCAP_S390_TE
#define HWCAP_S390_TE 1024
#endif

#ifndef HWCAP_S390_VX
#define HWCAP_S390_VX 2048
#endif

/* Only some of the tdescs are used in ipa.  We don't care about last_break,
   syscall number, orig r2, or TDB (fast tracepoint has no way of saving
   those), so we ignore linux*v[12] and te_linux64.  Unfortunately, we have
   to handle both vx_linux* and tevx_linux*, since they have different
   numbers for VX registers.  */

#ifndef __s390x__

/* Defined in auto-generated file s390-linux32.c.  */
void init_registers_s390_linux32 (void);
extern const struct target_desc *tdesc_s390_linux32;

/* Defined in auto-generated file s390-linux64.c.  */
void init_registers_s390_linux64 (void);
extern const struct target_desc *tdesc_s390_linux64;

/* Defined in auto-generated file s390-vx-linux64.c.  */
void init_registers_s390_vx_linux64 (void);
extern const struct target_desc *tdesc_s390_vx_linux64;

/* Defined in auto-generated file s390-tevx-linux64.c.  */
void init_registers_s390_tevx_linux64 (void);
extern const struct target_desc *tdesc_s390_tevx_linux64;

#else

/* Defined in auto-generated file s390x-linux64.c.  */
void init_registers_s390x_linux64 (void);
extern const struct target_desc *tdesc_s390x_linux64;

/* Defined in auto-generated file s390x-vx-linux64.c.  */
void init_registers_s390x_vx_linux64 (void);
extern const struct target_desc *tdesc_s390x_vx_linux64;

/* Defined in auto-generated file s390x-tevx-linux64.c.  */
void init_registers_s390x_tevx_linux64 (void);
extern const struct target_desc *tdesc_s390x_tevx_linux64;

#endif

#define FT_FPR(x) (0x000 + (x) * 0x10)
#define FT_VR(x) (0x000 + (x) * 0x10)
#define FT_VR_L(x) (0x008 + (x) * 0x10)
#define FT_GPR(x) (0x200 + (x) * 8)
#define FT_GPR_U(x) (0x200 + (x) * 8)
#define FT_GPR_L(x) (0x204 + (x) * 8)
#define FT_GPR(x) (0x200 + (x) * 8)
#define FT_ACR(x) (0x280 + (x) * 4)
#define FT_PSWM 0x2c0
#define FT_PSWM_U 0x2c0
#define FT_PSWA 0x2c8
#define FT_PSWA_L 0x2cc
#define FT_FPC 0x2d0

/* Mappings between registers collected by the jump pad and GDB's register
   array layout used by regcache.

   See linux-s390-low.c (s390_install_fast_tracepoint_jump_pad) for more
   details.  */

#ifndef __s390x__

/* Used for s390-linux32, s390-linux32v1, s390-linux32v2.  */

static const int s390_linux32_ft_collect_regmap[] = {
  /* 32-bit PSWA and PSWM.  */
  FT_PSWM_U, FT_PSWA_L,
  /* 32-bit GPRs (mapped to lower halves of 64-bit slots).  */
  FT_GPR_L (0), FT_GPR_L (1), FT_GPR_L (2), FT_GPR_L (3),
  FT_GPR_L (4), FT_GPR_L (5), FT_GPR_L (6), FT_GPR_L (7),
  FT_GPR_L (8), FT_GPR_L (9), FT_GPR_L (10), FT_GPR_L (11),
  FT_GPR_L (12), FT_GPR_L (13), FT_GPR_L (14), FT_GPR_L (15),
  /* ACRs */
  FT_ACR (0), FT_ACR (1), FT_ACR (2), FT_ACR (3),
  FT_ACR (4), FT_ACR (5), FT_ACR (6), FT_ACR (7),
  FT_ACR (8), FT_ACR (9), FT_ACR (10), FT_ACR (11),
  FT_ACR (12), FT_ACR (13), FT_ACR (14), FT_ACR (15),
  /* FPRs (mapped to upper halves of 128-bit VR slots).  */
  FT_FPR (0), FT_FPR (1), FT_FPR (2), FT_FPR (3),
  FT_FPR (4), FT_FPR (5), FT_FPR (6), FT_FPR (7),
  FT_FPR (8), FT_FPR (9), FT_FPR (10), FT_FPR (11),
  FT_FPR (12), FT_FPR (13), FT_FPR (14), FT_FPR (15),
  /* orig_r2, last_break, system_call */
  -1, -1, -1,
};

/* Used for s390-linux64, s390-linux64v1, s390-linux64v2, s390-vx-linux64.  */

static const int s390_linux64_ft_collect_regmap[] = {
  /* 32-bit PSWA and PSWM.  */
  FT_PSWM_U, FT_PSWA_L,
  /* 32-bit halves of 64-bit GPRs.  */
  FT_GPR_U (0), FT_GPR_L (0),
  FT_GPR_U (1), FT_GPR_L (1),
  FT_GPR_U (2), FT_GPR_L (2),
  FT_GPR_U (3), FT_GPR_L (3),
  FT_GPR_U (4), FT_GPR_L (4),
  FT_GPR_U (5), FT_GPR_L (5),
  FT_GPR_U (6), FT_GPR_L (6),
  FT_GPR_U (7), FT_GPR_L (7),
  FT_GPR_U (8), FT_GPR_L (8),
  FT_GPR_U (9), FT_GPR_L (9),
  FT_GPR_U (10), FT_GPR_L (10),
  FT_GPR_U (11), FT_GPR_L (11),
  FT_GPR_U (12), FT_GPR_L (12),
  FT_GPR_U (13), FT_GPR_L (13),
  FT_GPR_U (14), FT_GPR_L (14),
  FT_GPR_U (15), FT_GPR_L (15),
  /* ACRs */
  FT_ACR (0), FT_ACR (1), FT_ACR (2), FT_ACR (3),
  FT_ACR (4), FT_ACR (5), FT_ACR (6), FT_ACR (7),
  FT_ACR (8), FT_ACR (9), FT_ACR (10), FT_ACR (11),
  FT_ACR (12), FT_ACR (13), FT_ACR (14), FT_ACR (15),
  /* FPRs (mapped to upper halves of 128-bit VR slots).  */
  FT_FPR (0), FT_FPR (1), FT_FPR (2), FT_FPR (3),
  FT_FPR (4), FT_FPR (5), FT_FPR (6), FT_FPR (7),
  FT_FPR (8), FT_FPR (9), FT_FPR (10), FT_FPR (11),
  FT_FPR (12), FT_FPR (13), FT_FPR (14), FT_FPR (15),
  /* orig_r2, last_break, system_call */
  -1, -1, -1,
  /* Lower halves of 128-bit VRs. */
  FT_VR_L (0), FT_VR_L (1), FT_VR_L (2), FT_VR_L (3),
  FT_VR_L (4), FT_VR_L (5), FT_VR_L (6), FT_VR_L (7),
  FT_VR_L (8), FT_VR_L (9), FT_VR_L (10), FT_VR_L (11),
  FT_VR_L (12), FT_VR_L (13), FT_VR_L (14), FT_VR_L (15),
  /* And the next 16 VRs.  */
  FT_VR (16), FT_VR (17), FT_VR (18), FT_VR (19),
  FT_VR (20), FT_VR (21), FT_VR (22), FT_VR (23),
  FT_VR (24), FT_VR (25), FT_VR (26), FT_VR (27),
  FT_VR (28), FT_VR (29), FT_VR (30), FT_VR (31),
};

/* Used for s390-te-linux64, s390-tevx-linux64.  */

static const int s390_te_linux64_ft_collect_regmap[] = {
  /* 32-bit PSWA and PSWM.  */
  FT_PSWM_U, FT_PSWA_L,
  /* 32-bit halves of 64-bit GPRs.  */
  FT_GPR_U (0), FT_GPR_L (0),
  FT_GPR_U (1), FT_GPR_L (1),
  FT_GPR_U (2), FT_GPR_L (2),
  FT_GPR_U (3), FT_GPR_L (3),
  FT_GPR_U (4), FT_GPR_L (4),
  FT_GPR_U (5), FT_GPR_L (5),
  FT_GPR_U (6), FT_GPR_L (6),
  FT_GPR_U (7), FT_GPR_L (7),
  FT_GPR_U (8), FT_GPR_L (8),
  FT_GPR_U (9), FT_GPR_L (9),
  FT_GPR_U (10), FT_GPR_L (10),
  FT_GPR_U (11), FT_GPR_L (11),
  FT_GPR_U (12), FT_GPR_L (12),
  FT_GPR_U (13), FT_GPR_L (13),
  FT_GPR_U (14), FT_GPR_L (14),
  FT_GPR_U (15), FT_GPR_L (15),
  /* ACRs */
  FT_ACR (0), FT_ACR (1), FT_ACR (2), FT_ACR (3),
  FT_ACR (4), FT_ACR (5), FT_ACR (6), FT_ACR (7),
  FT_ACR (8), FT_ACR (9), FT_ACR (10), FT_ACR (11),
  FT_ACR (12), FT_ACR (13), FT_ACR (14), FT_ACR (15),
  /* FPRs (mapped to upper halves of 128-bit VR slots).  */
  FT_FPR (0), FT_FPR (1), FT_FPR (2), FT_FPR (3),
  FT_FPR (4), FT_FPR (5), FT_FPR (6), FT_FPR (7),
  FT_FPR (8), FT_FPR (9), FT_FPR (10), FT_FPR (11),
  FT_FPR (12), FT_FPR (13), FT_FPR (14), FT_FPR (15),
  /* orig_r2, last_break, system_call */
  -1, -1, -1,
  /* TDB */
  -1, -1, -1, -1,
  -1, -1, -1, -1,
  -1, -1, -1, -1,
  -1, -1, -1, -1,
  -1, -1, -1, -1,
  /* Lower halves of 128-bit VRs. */
  FT_VR_L (0), FT_VR_L (1), FT_VR_L (2), FT_VR_L (3),
  FT_VR_L (4), FT_VR_L (5), FT_VR_L (6), FT_VR_L (7),
  FT_VR_L (8), FT_VR_L (9), FT_VR_L (10), FT_VR_L (11),
  FT_VR_L (12), FT_VR_L (13), FT_VR_L (14), FT_VR_L (15),
  /* And the next 16 VRs.  */
  FT_VR (16), FT_VR (17), FT_VR (18), FT_VR (19),
  FT_VR (20), FT_VR (21), FT_VR (22), FT_VR (23),
  FT_VR (24), FT_VR (25), FT_VR (26), FT_VR (27),
  FT_VR (28), FT_VR (29), FT_VR (30), FT_VR (31),
};

#else /* __s390x__ */

/* Used for s390x-linux64, s390x-linux64v1, s390x-linux64v2, s390x-vx-linux64.  */

static const int s390x_ft_collect_regmap[] = {
  /* 64-bit PSWA and PSWM.  */
  FT_PSWM, FT_PSWA,
  /* 64-bit GPRs.  */
  FT_GPR (0), FT_GPR (1), FT_GPR (2), FT_GPR (3),
  FT_GPR (4), FT_GPR (5), FT_GPR (6), FT_GPR (7),
  FT_GPR (8), FT_GPR (9), FT_GPR (10), FT_GPR (11),
  FT_GPR (12), FT_GPR (13), FT_GPR (14), FT_GPR (15),
  /* ACRs */
  FT_ACR (0), FT_ACR (1), FT_ACR (2), FT_ACR (3),
  FT_ACR (4), FT_ACR (5), FT_ACR (6), FT_ACR (7),
  FT_ACR (8), FT_ACR (9), FT_ACR (10), FT_ACR (11),
  FT_ACR (12), FT_ACR (13), FT_ACR (14), FT_ACR (15),
  /* FPRs (mapped to upper halves of 128-bit VR slots).  */
  FT_FPR (0), FT_FPR (1), FT_FPR (2), FT_FPR (3),
  FT_FPR (4), FT_FPR (5), FT_FPR (6), FT_FPR (7),
  FT_FPR (8), FT_FPR (9), FT_FPR (10), FT_FPR (11),
  FT_FPR (12), FT_FPR (13), FT_FPR (14), FT_FPR (15),
  /* orig_r2, last_break, system_call */
  -1, -1, -1,
  /* Lower halves of 128-bit VRs. */
  FT_VR_L (0), FT_VR_L (1), FT_VR_L (2), FT_VR_L (3),
  FT_VR_L (4), FT_VR_L (5), FT_VR_L (6), FT_VR_L (7),
  FT_VR_L (8), FT_VR_L (9), FT_VR_L (10), FT_VR_L (11),
  FT_VR_L (12), FT_VR_L (13), FT_VR_L (14), FT_VR_L (15),
  /* And the next 16 VRs.  */
  FT_VR (16), FT_VR (17), FT_VR (18), FT_VR (19),
  FT_VR (20), FT_VR (21), FT_VR (22), FT_VR (23),
  FT_VR (24), FT_VR (25), FT_VR (26), FT_VR (27),
  FT_VR (28), FT_VR (29), FT_VR (30), FT_VR (31),
};

/* Used for s390x-te-linux64, s390x-tevx-linux64.  */

static const int s390x_te_ft_collect_regmap[] = {
  /* 64-bit PSWA and PSWM.  */
  FT_PSWM, FT_PSWA,
  /* 64-bit GPRs.  */
  FT_GPR (0), FT_GPR (1), FT_GPR (2), FT_GPR (3),
  FT_GPR (4), FT_GPR (5), FT_GPR (6), FT_GPR (7),
  FT_GPR (8), FT_GPR (9), FT_GPR (10), FT_GPR (11),
  FT_GPR (12), FT_GPR (13), FT_GPR (14), FT_GPR (15),
  /* ACRs */
  FT_ACR (0), FT_ACR (1), FT_ACR (2), FT_ACR (3),
  FT_ACR (4), FT_ACR (5), FT_ACR (6), FT_ACR (7),
  FT_ACR (8), FT_ACR (9), FT_ACR (10), FT_ACR (11),
  FT_ACR (12), FT_ACR (13), FT_ACR (14), FT_ACR (15),
  /* FPRs (mapped to upper halves of 128-bit VR slots).  */
  FT_FPR (0), FT_FPR (1), FT_FPR (2), FT_FPR (3),
  FT_FPR (4), FT_FPR (5), FT_FPR (6), FT_FPR (7),
  FT_FPR (8), FT_FPR (9), FT_FPR (10), FT_FPR (11),
  FT_FPR (12), FT_FPR (13), FT_FPR (14), FT_FPR (15),
  /* orig_r2, last_break, system_call */
  -1, -1, -1,
  /* TDB */
  -1, -1, -1, -1,
  -1, -1, -1, -1,
  -1, -1, -1, -1,
  -1, -1, -1, -1,
  -1, -1, -1, -1,
  /* Lower halves of 128-bit VRs. */
  FT_VR_L (0), FT_VR_L (1), FT_VR_L (2), FT_VR_L (3),
  FT_VR_L (4), FT_VR_L (5), FT_VR_L (6), FT_VR_L (7),
  FT_VR_L (8), FT_VR_L (9), FT_VR_L (10), FT_VR_L (11),
  FT_VR_L (12), FT_VR_L (13), FT_VR_L (14), FT_VR_L (15),
  /* And the next 16 VRs.  */
  FT_VR (16), FT_VR (17), FT_VR (18), FT_VR (19),
  FT_VR (20), FT_VR (21), FT_VR (22), FT_VR (23),
  FT_VR (24), FT_VR (25), FT_VR (26), FT_VR (27),
  FT_VR (28), FT_VR (29), FT_VR (30), FT_VR (31),
};

#endif

enum {
#ifndef __s390x__
  S390_LINUX32,
#endif
  S390_LINUX64,
  S390_VX_LINUX64,
  S390_TEVX_LINUX64,
};


static const struct {
  const struct target_desc **tdesc;
  void (*init) (void);
  const int *regmap;
  int regnum;
} s390_targets[] = {
#ifndef __s390x__
  {
    &tdesc_s390_linux32,
    init_registers_s390_linux32,
    s390_linux32_ft_collect_regmap,
    (sizeof s390_linux32_ft_collect_regmap
     / sizeof s390_linux32_ft_collect_regmap[0]),
  },
  {
    &tdesc_s390_linux64,
    init_registers_s390_linux64,
    s390_linux64_ft_collect_regmap,
    /* Subtract number of VX regs.  */
    (sizeof s390_linux64_ft_collect_regmap
     / sizeof s390_linux64_ft_collect_regmap[0]) - 32,
  },
  {
    &tdesc_s390_vx_linux64,
    init_registers_s390_vx_linux64,
    s390_linux64_ft_collect_regmap,
    (sizeof s390_linux64_ft_collect_regmap
     / sizeof s390_linux64_ft_collect_regmap[0]),
  },
  {
    &tdesc_s390_tevx_linux64,
    init_registers_s390_tevx_linux64,
    s390_te_linux64_ft_collect_regmap,
    (sizeof s390_te_linux64_ft_collect_regmap
     / sizeof s390_te_linux64_ft_collect_regmap[0]),
  },
#else
  {
    &tdesc_s390x_linux64,
    init_registers_s390x_linux64,
    s390x_ft_collect_regmap,
    /* Subtract number of VX regs.  */
    (sizeof s390x_ft_collect_regmap
     / sizeof s390x_ft_collect_regmap[0]) - 32,
  },
  {
    &tdesc_s390x_vx_linux64,
    init_registers_s390x_vx_linux64,
    s390x_ft_collect_regmap,
    (sizeof s390x_ft_collect_regmap
     / sizeof s390x_ft_collect_regmap[0]),
  },
  {
    &tdesc_s390x_tevx_linux64,
    init_registers_s390x_tevx_linux64,
    s390x_te_ft_collect_regmap,
    (sizeof s390x_te_ft_collect_regmap
     / sizeof s390x_te_ft_collect_regmap[0]),
  },
#endif
};


static int s390_target;


/* Fill in REGCACHE with registers saved by the jump pad in BUF.  */

void
supply_fast_tracepoint_registers (struct regcache *regcache,
				  const unsigned char *buf)
{
  int i;
  int regnum = s390_targets[s390_target].regnum;
  const int *regmap =  s390_targets[s390_target].regmap;

  for (i = 0; i < regnum; i++)
    if (regmap[i] != -1)
      supply_register (regcache, i, ((char *) buf) + regmap[i]);
}

IP_AGENT_EXPORT_FUNC ULONGEST
gdb_agent_get_raw_reg (const unsigned char *raw_regs, int regnum)
{
  const int *regmap =  s390_targets[s390_target].regmap;
  int offset;
  if (regnum >= s390_targets[s390_target].regnum)
    return 0;
  offset = regmap[regnum];
  if (offset == -1)
    return 0;

  /* The regnums are variable, better to figure out size by FT offset.  */

  /* 64-bit ones.  */
  if (offset < FT_VR(16)
#ifdef __s390x__
      || (offset >= FT_GPR(0) && offset < FT_ACR(0))
      || offset == FT_PSWM
      || offset == FT_PSWA
#endif
     )
    return *(uint64_t *) (raw_regs + offset);

  if (offset >= FT_ACR(0 && offset < FT_PSWM)
      || offset == FT_FPC
#ifndef __s390x__
      || (offset >= FT_GPR(0) && offset < FT_ACR(0))
      || offset == FT_PSWM_U
      || offset == FT_PSWA_L
#endif
     )
    return *(uint32_t *) (raw_regs + offset);

  /* This leaves 128-bit VX.  No way to return them.  */
  return 0;
}

void
initialize_low_tracepoint (void)
{
  unsigned long hwcap = getauxval (AT_HWCAP);
#ifndef __s390x__
  /* XXX Other targets temporarily disabled due to lack of support in
     gdbserver.  */
  s390_target = S390_LINUX32;
#else
  if (!(hwcap & HWCAP_S390_VX))
    s390_target = S390_LINUX64;
  else if (!(hwcap & HWCAP_S390_TE))
    s390_target = S390_VX_LINUX64;
  else
    s390_target = S390_TEVX_LINUX64;
#endif
  s390_targets[s390_target].init ();
  ipa_tdesc = *s390_targets[s390_target].tdesc;
}
