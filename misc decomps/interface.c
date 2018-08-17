
typedef enum
{
  Z64_ACTIONBTN_A,
  Z64_ACTIONBTN_B,
  Z64_ACTIONBTN_START,
} z64_actionbtn_t;

/*
  Actions:
    0000  Attack
    0001  Check
    0002  Enter
    0003  Return
    0004  Open
    0005  Jump
    0006  Decide
    0007  Dive
    0008  Faster
    0009  Throw
    000A  *blank*
    000B  Climb
    000C  Drop
    000D  Down
    000E  Save
    000F  Speak
    0010  Next
    0011  Grab
    0012  Stop
    0013  Put Away
    0014  Reel
    0015  1
    0016  2
    0017  3
    0018  4
    0019  5
    001A  6
    001B  7
    001C  8
*/

/* game + 0x104F0 */
typedef struct
{
  char            unk_00_[0x0130];        /* 0x0000 */
  char           *parameter;              /* 0x0130 */
  char           *action_tex;             /* 0x0134 */
  char           *item_tex;               /* 0x0138 */
  char           *minimap_tex;            /* 0x013C */
  char            unk_01_[0x00AC];        /* 0x0140 */
  uint16_t        h1EC;                   /* 0x01EC */
  char            unk_02_[0x0002];        /* 0x01EE */
  uint16_t        a_action;               /* 0x01F0 */
  char            unk_03_[0x0002];        /* 0x01F2 */
  float           f1F4;                   /* 0x01F4 */
  char            unk_04_[0x0004];        /* 0x01F8 */
  int16_t         b_label;                /* 0x01FC */
  char            unk_05_[0x0064];        /* 0x01FE */
  struct
  {
    uint8_t       unk_00_;
    uint8_t       b_button;
    uint8_t       unk_01_;
    uint8_t       bottles;
    uint8_t       trade_items;
    uint8_t       hookshot;
    uint8_t       ocarina;
    uint8_t       warp_songs;
    uint8_t       suns_song;
    uint8_t       farores_wind;
    uint8_t       dfnl;
    uint8_t       all;
  }               restriction_flags;      /* 0x0262 */
  char            unk_06_[0x0002];        /* 0x026E */
                                          /* 0x0270 */
} z64_if_ctxt_t;

/* 80071D24 */
void f80071D24(z64_if_ctxt_t *if_ctxt, uint16_t action_idx, int button_idx)
{
80071d24: $sp := $sp + 0xffc8
80071d28: w.[$sp + 0x0028] := $s0
80071d2c: w.[$sp + 0x003c] := $a1
80071d30: $a1 := $a1 & 0xffff
80071d34: $s0 := $a0
80071d38: w.[$sp + 0x002c] := $ra
80071d3c: w.[$sp + 0x0040] := $a2
80071d44: if ($a1 < 0x001d) {
80071d48:   $v0 := $a1
          }
          else {
80071d4c:   $v0 := 0xA
          }
80071d60: if (z64_file.language != 0) {
80071d68:   $v0 := $v0 + 0x001d & 0xffff
          }
80071d6c: if ($v0 == 0xA || $v0 == 0x27) {
80071e18:   w.[0x80120c54] := sx(w.[$s0 + 0x0134]) + 0x80000000
80071e20:   $v0 := sx(w.[0x800f7588 + sx(h.[$sp + 0x0042]) * 0x00000004])
80071e4c:   $a0 := ($v0 & 0x00ffffff) + sx(w.[0x80120c38 + $v0 * 0x00000010 / 0x10000000 * 0x00000004]) + 0x80000000
80071e54:   $a1 := 0x00000060
80071e50:   call 0x80071cec
          }
          else {
80071d7c:   $a0 := $s0 + 0x01c0
80071d80:   $a1 := $s0 + 0x01d8
80071d84:   $a2 := 0x00000001
80071d88:   w.[$sp + 0x0034] := $v0
80071d90:   w.[$sp + 0x0030] := $a0
80071d8c:   call 0x80004220
80071db4:   w.[$sp + 0x0018] := 0
80071db8:   w.[$sp + 0x0010] := 0
80071dbc:   $a0 := $s0 + 0x0160
80071dc0:   $a3 := 0x00000180
80071dc4:   w.[$sp + 0x0014] := sx(w.[$sp + 0x0030])
80071dcc:   $a1 := sx(w.[$s0 + 0x0134]) + sx(h.[$sp + 0x0042]) * 0x00000180
80071ddc:   $a2 := sx(w.[$sp + 0x0034]) * 0x00000180 + 0x008e0000
80071de0:   call 0x80000d28
80071de8:   $a0 := sx(w.[$sp + 0x0030])
80071dec:   $a1 := 0
80071df4:   $a2 := 0x00000001
80071df0:   call 0x80002030
          }
}

/* 80071E6C */
void f80071E6C(z64_game_t *game, uint16_t action_idx)
{
  z64_if_ctxt_t *if_ctxt = &game.if_ctxt;
  if (action_idx != if_ctxt->action_idx) {
    if_ctxt->action_idx = action_idx;
    if_ctxt->h1EC = 1;
    if_ctxt->f1F4 = 0.;
    f80071D24(if_ctxt, action_idx, 1);
    if (game->h10934 != 0)
      if_ctxt->h1EC = 3;
  }
}
