moat sound
801E29CC  0C0314DD  jal 800C5374  (801E2E24, 3F800000)

??
801E2AA0  0C008BF4  jal 80022FD0  (801E2EB0, 000020E5)


typedef struct
{
  void       *p0;               /* 0x0000 */
  void       *p4;               /* 0x0004 */
  void       *p8;               /* 0x0008 */
  char        unk_00_[0x0004];  /* 0x000C */
  void       *p10;              /* 0x0010 */
  void       *p14;              /* 0x0014 */
  void       *p18;              /* 0x0018 */
  char        unk_01_[0x000C];  /* 0x001C */
  uint16_t    sfx_id;           /* 0x0028 */
  char        unk_02_[0x0002];  /* 0x002A */
  uint8_t     prev;             /* 0x002C */
  uint8_t     next;             /* 0x002D */
  char        unk_03_[0x0002];  /* 0x002E */
                                /* 0x0030 */
} z64_sfx_cp_t;

typedef struct
{
  uint16_t    sfx_id;           /* 0x0000 */
  void       *p4;               /* 0x0004 */
  uint8_t     b8;               /* 0x0008 */
  void       *pC;               /* 0x000C */
  void       *p10;              /* 0x0010 */
  void       *p14;              /* 0x0014 */
                                /* 0x0018 */
} z64_sfx_cmd_t;

/* 80104360 */  uint8_t         sfx_write_pos;
/* 80104364 */  uint8_t         sfx_read_pos;
/* 80104368 */  z64_sfx_cp_t   *sfx_cp_lists[7];
/* 80122E90 */  z64_sfx_cmd_t   sfx_buf[0x100];
/* 80124698 */  uint8_t         sfx_cp_pos[7];      /* index of first free control point */

/* 80022FD0 */
void f80022FD0(z64_actor_t *actor, uint16_t sfx_id)
{
  f80064780(actor + 0xE4, sfx_id);
}

/* 80064780 */
void f80064780(void *arg0, uint16_t sfx_id)
{
  f800C806C(sfx_id, arg0, 0x04, 0x801043A0, 0x801043A0, 0x801043A8);
}

/* 800C5374 */
void f800C5374(void *arg0, float volume)
{
  if (sfx_has_cp(0x2006)) {
    if (volume != w.[0x80121DD0]) {
      w.[0x80121DD4] = volume;                                  /* target volume */
      w.[0x80121DDC] = 0x00000028;                              /* fade frames */
      w.[0x80121DD8] = (w.[0x80121DD4] - w.[0x80121DD0]) / 40.; /* fade step */
    }
  }
  else {
    w.[0x80121DD0] = volume;                                    /* current volume */
  }
  f800C806C(0x2006, arg0, 0x04, 0x80121DD0, 0x801043A0, 0x801043A8);
}

/* 800C806C */
/* insert sfx command */
void f800C806C(uint16_t sfx_id, void *arg1, uint8_t arg2, void *arg3, void *arg4, void *arg5)
{
  uint8_t bnk_idx = (sfx_id & 0xF000) >> 12;
  if (b.[0x80124754 + bnk_idx] == 0) {
    z64_sfx_cmd_t *cmd = &sfx_buf[sfx_write_pos];
    cmd->sfx_id = sfx_id;
    cmd->p4 = arg1;
    cmd->b8 = arg2;
    cmd->pC = arg3;
    cmd->p10 = arg4;
    cmd->p14 = arg5;
    ++sfx_write_pos;
  }
}

/* 800C828C */
/* process one sfx command */
void f800C828C(void)
{
800c82cc: z64_sfx_cmd_t *cmd = &sfx_buf[sfx_read_pos];
800c82dc: if (cmd->sfx_id != 0) {
800c82e8:   uint8_t bnk_idx = (cmd->sfx_id & 0xF000) >> 12;
800c82f8:   z64_sfc_cp_t *cp_list = sfx_cp_lists[bnk_idx];
800c8304:   $a1 := b.[cp_list + 0x002d]
800c830c:   $v0 := $a1
800c8308:   if ($a1 == 0xFF) {
L1:
800c85c8:     $a2 := bnk_idx + 0x80124698
800c85cc:     $t1 := b.[$a2]
800c85ec:     if (0xFF <> b.[cp_list + $t1 * 0x30 + 0x002d]) {
800c85f4:       if ($v0 <> 0) {
800c860c:         $v0 := cp_list + ($t1 & 0x00ff) * 0x00000030
800c8610:         w.[$v0] := sx(w.[cmd + 0x0004])
800c861c:         w.[$v0 + 0x0004] := sx(w.[cmd + 0x0004]) + 0x0004
800c8628:         w.[$v0 + 0x0008] := sx(w.[cmd + 0x0004]) + 0x0008
800c8630:         b.[$v0 + 0x000c] := b.[cmd + 0x0008]
800c8638:         w.[$v0 + 0x0010] := sx(w.[cmd + 0x000c])
800c8640:         w.[$v0 + 0x0014] := sx(w.[cmd + 0x0010])
800c8648:         w.[$v0 + 0x0018] := sx(w.[cmd + 0x0014])
800c866c:         $v1 := sx(w.[0x8010433c + (cmd->sfx_id ~>> 0x0c & 0x00ff) * 0x00000004]) + (cmd->sfx_id & 0x01ff) * 0x00000004
800c8678:         h.[$v0 + 0x0026] := h.[$v1 + 0x0002]
800c8684:         b.[$v0 + 0x0024] := b.[$v1]
800c8694:         b.[$v0 + 0x002a] := 0x00000001
800c8698:         b.[$v0 + 0x002b] := 0x00000002
800c869c:         $a0 := bnk_idx - 0x7fedb970
800c86a0:         h.[$v0 + 0x0028] := h.[cmd]
800c86a8:         b.[$v0 + 0x002c] := b.[$a0]
800c86c4:         b.[cp_list + b.[$a0] * 0x00000030 + 0x002d] := b.[$a2]
800c86c8:         $t1 := b.[$a2]
800c86cc:         cp_list := cp_list
800c86d4:         b.[$a0] := $t1
800c86e0:         $t6 := b.[cp_list + $t1 * 0x00000030 + 0x002d]
800c86ec:         b.[$a2] := $t6
800c86f8:         b.[cp_list + ($t6 & 0x00ff) * 0x00000030 + 0x002c] := 0x000000ff
800c86fc:         b.[$v0 + 0x002d] := 0x000000ff
                }
                goto L0
              }
            }
800c8310:   else if ($a1 == 0) {
              goto L1
            }
800c82d4:   $s1 := 0x00000080
800c8300:   $t4 := 0
800c8320:   $s2 := b.[$sp + 0x004b]
            while (1) {
800c8330:     $s0 := cp_list + $a1 * 0x00000030
800c8338:     if (sx(w.[cmd + 0x0004]) <> sx(w.[$s0])) {
L5:
800c8590:       if ($a1 == 0) {
800c8594:         $v0 := $a1
                }
                else {
800c85a4:         $a1 := b.[cp_list + $a1 * 0x00000030 + 0x002d]
800c85a8:         $v0 := $a1
                }
800c85ac:       if (0x000000ff == $v0) {
                  goto L1
                }
800c85b4:       if ($v0 <> 0) {
                  continue
                }
800c85bc:       b.[$sp + 0x004b] := $s2
                goto L1
              }
800c8354:     $t5 := sx(w.[0x8010433c + (cmd->sfx_id ~>> 0x0c & 0x00ff) * 0x00000004])
800c835c:     $ra := (cmd->sfx_id & 0x01ff) * 0x00000004
800c8360:     $a2 := $t5 + $ra
800c836c:     if ((h.[$a2 + 0x0002] & 0x0020) == 0) {
L6:
800c838c:       $v1 := h.[$s0 + 0x0028]
800c83a0:       $v0 := b.[0x801018a4 + b.[0x8010438c] * 0x00000007 + bnk_idx]
800c83a8:       v147 := $v0
800c83a4:       if (cmd->sfx_id <> $v1) {
                  $t4 := $t4
                  $t1 := v147
                  $v1 := $v1
800c83b8:         if ($t4 <> 0) {
                    $s2 := $s2
800c83f0:           $a0 := $v1 & 0xffff
800c83fc:           if (b.[$s0 + 0x0024] < $s2) {
800c841c:             $s1 := $a1 & 0x00ff
800c8420:             $s2 := b.[sx(w.[0x8010433c + ($a0 ~>> 0x0c & 0x00ff) * 0x00000004]) + ($a0 & 0x01ff) * 0x00000004]
                    }
                  }
                  else {
800c83bc:           $a0 := $v1 & 0xffff
800c83e0:           $s1 := $a1 & 0x00ff
800c83e8:           $s2 := b.[sx(w.[0x8010433c + ($a0 ~>> 0x0c & 0x00ff) * 0x00000004]) + ($a0 & 0x01ff) * 0x00000004]
                  }
800c8428:         $t4 := $t4 + 0x0001 & 0x00ff
800c8430:         v141 := $t4
800c842c:         if ($t4 <> $t1) {
800c8430:           $v1 := v141
                  }
                  else {
800c8430:           $v1 := v141
800c8440:           if (b.[$a2] < $s2) {
800c8438:             $a1 := 0
                    }
                    else {
800c844c:             $a1 := $s1 & 0x00ff
                    }
                  }
                }
                else {
800c83ac:         $t4 := $v0 & 0x00ff
800c83a8:         $t1 := v147
800c83b4:         $v1 := $t4
                }
800c8450:       if ($v1 == $t1) {
800c845c:         $v0 := 0x00000030 * $a1
800c8464:         $v1 := $t5 + $ra
800c846c:         v137 := cp_list + $v0
800c8468:         if ((cmd->sfx_id & 0x0c00) <> 0) {
800c846c:           $s0 := v137
L4:
800c8488:           $t7 := h.[$s0 + 0x0026]
L2:
800c8490:           if (($t7 & 0x0008) == 0) {
800c8494:             $t7 := b.[cmd + 0x0008]
                    }
800c84a0:           else if (b.[$s0 + 0x002a] == 0x00000001) {
800c84a4:             $t7 := b.[cmd + 0x0008]
                    }
                    else {
800c84a8:             $a0 := b.[$s0 + 0x002e]
800c84ac:             b.[$sp + 0x005d] := $t4
800c84b0:             w.[$sp + 0x0058] := cmd
800c84b4:             w.[$sp + 0x0040] := $t0
800c84b8:             w.[$sp + 0x0050] := $v1
800c84c0:             w.[$sp + 0x0038] := $v0
800c84bc:             call 0x800c7ffc
800c84c4:             $t0 := sx(w.[$sp + 0x0040])
800c84c8:             $v0 := sx(w.[$sp + 0x0038])
800c84cc:             $v1 := sx(w.[$sp + 0x0050])
800c84d4:             cmd := sx(w.[$sp + 0x0058])
800c84d8:             $t4 := b.[$sp + 0x005d]
800c84dc:             $s0 := cp_list + $v0
800c84e0:             $t7 := b.[cmd + 0x0008]
                    }
800c84e4:           b.[$s0 + 0x000c] := $t7
800c84f8:           h.[cp_list + $v0 + 0x0028] := h.[cmd]
800c8508:           b.[cp_list + $v0 + 0x002a] := 0x00000001
800c8514:           b.[cp_list + $v0 + 0x002b] := 0x00000002
800c8524:           w.[cp_list + $v0 + 0x0010] := sx(w.[cmd + 0x000c])
800c8534:           w.[cp_list + $v0 + 0x0014] := sx(w.[cmd + 0x0010])
800c8544:           w.[cp_list + $v0 + 0x0018] := sx(w.[cmd + 0x0014])
800c8554:           h.[cp_list + $v0 + 0x0026] := h.[$v1 + 0x0002]
800c8564:           b.[cp_list + $v0 + 0x0024] := b.[$v1]
800c856c:           cp_list := cp_list
L3:
800c858c:           $a1 := 0
                  }
                  else {
800c846c:           $s0 := v137
800c8478:           if ((h.[$v1 + 0x0002] & 0x0004) <> 0) {
800c847c:             $t7 := h.[$s0 + 0x0026]
                      goto L2
                    }
800c8480:           if ($s1 <> $a1) {
800c857c:             if (b.[$s0 + 0x002a] <> 0x00000005) {
800c8580:               $a1 := 0
                      }
                      else {
800c8584:               b.[$s0 + 0x002a] := 0x00000004
800c8588:               cp_list := cp_list
                        goto L3
                      }
                    }
                    else {
                      goto L4
                    }
                  }
                }
                goto L5
              }
800c837c:     if (b.[$a2] == b.[$s0 + 0x0024]) {
                break
              }
              goto L6
            }
          }
}

/* 800C99BC */
/* process sfx commands */

/* 800C9B74 */
int sfx_has_cp(uint16_t sfx_id)
{
  uint8_t bnk_idx = (sfx_id & 0xF000) >> 12;
  z64_sfx_cp_t *cp_list = sfx_cp_lists[bnk_idx];
  
  uint8_t cp_idx = cp_list[0].next;
  while (cp_idx != 0xFF) {
    z64_sfx_cp_t *cp = &cp_list[cp_idx];
    if (cp->sfx_id == sfx_id)
      return 1;
    cp_idx = cp->next;
  }

  return 0;
}
