
typedef struct
{
  /* which channels to use */
  uint16_t          channel_enable;           /* 0x0000 */
  /* which channels to disable initially */
  uint16_t          channel_mask;             /* 0x0002 */
  /* channel parameter command list */
  /* 3 bytes per command: ccppvv */
  /* c: channel (0xFF to terminate command list) */
  /* p: parameter id */
  /* v: parameter value */
  uint8_t           params[0x64];             /* 0x0004 */
                                              /* 0x0068 */
} z64_night_sfx_t;

/* 801019E8 */  z64_night_sfx_t   z64_night_sfx[20];

/* 80101964 */  uint8_t           z64_seq_flags[?];

/* 800CAA70 */  z64_audio_cmd(uint32_t cmd);

/* 800C7C90 */
void f800C7C90(uint8_t sfx_idx)
{
  uint16_t seq_idx = z64_seq_ctl[0].seq_idx;
  if (seq_idx == 0xFFFF || !(z64_seq_flags[(uint8_t)seq_idx] & 0x80)) {
    z64_night_sfx_t *night_sfx = &z64_night_sfx[sfx_idx];
    f800C7B8C(night_sfx->channel_enable, night_sfx->channel_mask);
    for (int i = 0; i < 0x64; i += 3) {
      uint8_t c = night_sfx->params[i + 0];
      if (c == 0xFF)
        break;
      uint8_t p = night_sfx->params[i + 1];
      uint8_t v = night_sfx->params[i + 2];
      z64_audio_cmd(0x80000000 | (p << 16) | (c << 8) | v);
    }
    z64_audio_cmd(0x80070D00 | b.[0x80101914]);
  }
}

/* 800C7B8C */
void f800C7B8C(uint16_t channel_enable, uint16_t channel_mask)
{
  if (f800CAB18(0) == 0x4C)
    f800C4C84(0x0F);
  else {
    z64_audio_cmd(0x70000001);
    z64_audio_cmd(0x70040000 | ((channel_enable >> 8) & 0xFF));
    z64_audio_cmd(0x70050000 | ((channel_enable >> 0) & 0xFF));
    /* set sequencer volume */
    f800CACA4(0, 0, 0x7F, 0x01);
    z64_audio_cmd(0x00000001);
    for (int8_t i = 0; i < 0x10; ++i) {
      uint16_t bit = 1 << i;
      if ((channel_enable & bit) && !(channel_mask & bit))
        z64_audio_cmd(0x80010001 | (i << 8));
    }
  }
}

/* 800CAB18 */
uint16_t f800CAB18(uint8_t sequencer_idx)
{
  if (afx.seq[sequencer_idx].flags.bit0)
    return z64_seq_ctl[sequencer_idx].seq_idx;
  else
    return 0xFFFF;
}

/* 800C4C84 */
void f800C4C84(uint8_t arg0)
{
  if (z64_sfx_mute[0] != 1) {
    z64_audio_cmd(0x0300006D);
    z64_audio_cmd(0x83000000 | arg0);
  }
}

/* 800CACA4 */
void f800CACA4(uint8_t sequencer_idx, uint8_t vol_idx, uint8_t vol, uint8_t time)
{
  z64_seq_ctl_t *seq_ctl = &z64_seq_ctl[sequencer_idx];
  seq_ctl->vp_factors[vol_idx] = vol & 0x7F;
  if (time == 0) {
    float fvol = 1.f;
    for (uint8_t i = 0; i < 4; ++i)
      fvol *= seq_ctl->vp_factors[i] / 127.f;
    int vol = fvol * 127.f;
    if (vol < 0)
      vol = 0xFFFF;
    z64_execute_audio_cmd(0x40000000 | (sequencer_idx << 24) | (time << 16) | (vol & 0xFF));
  }
  else {
    seq_ctl->vp_start = 1;
    seq_ctl->vp_time = time;
  }
}

/* 800C9FAC */  void z64_execute_audio_cmd(uint32_t cmd);
/* 800C7A78 */  idk what to call this (set night sfx channel parameters)

/* 800619CC */
void f800619CC(z64_game_t *game)
{
  switch (game->day_phase) {
    case 0: /* 80061A0C */
      f800C7A78(0x56, 0x01, 0x00);
      /* play day music */
      if (game->rain_effect_1 == 0 && game->rain_effect_2 == 0)
        f800C6264(game->b7A4);
      ++game->day_phase;
      break;

    case 1: /* 80061A54 */
      if (z64_file.day_time >= 0xB71D) {
        if (game->rain_effect_1 == 0 && game->rain_effect_2 == 0)
          z64_audio_cmd(0x10F000FF);
        ++game->day_phase;
      }
      break;

    case 2: /* 80061AAC */
      if (z64_file.day_time >= 0xC001) {
        /* play wolf howl */
        f80064738(0x28AE);
        ++game->day_phase;
      }
      break;

    case 3: /* 80061AE8 */
      if (game->rain_effect_1 == 0 && game->rain_effect_2 == 0) {
        /* play night sfx */
        f800C7C90(game->b7A5);
        f800C7A78(0x01, 0x01, 0x01);
      }
      ++game->day_phase;
      break;

    case 4: /* 80061B30 */
      if (z64_file.day_time >= 0xCAAC)
        ++game->day_phase;
      break;

    case 5: /* 80061B5C */
      f800C7A78(0x01, 0x01, 0x00);
      if (game->rain_effect_1 == 0 && game->rain_effect_2 == 0)
        f800C7A78(0x24, 0x01, 0x01);
      ++game->day_phase;
      break;

    case 6: /* 80061BAC */
      if (z64_file.day_time >= 0x4556 && z64_file.day_time < 0xCAAC) {
        z64_file.b141C = 1;
        ++z64_file.w14;
        ++z64_file.w18;
        /* play cucco crow */
        f800646F0(0x2813);
        /* hatch chickens */
        if (f800719E0(game, 0x0021, 0x0022) != 0 || f800719E0(game, 0x002D, 0x002E) != 0) {
          /* show textbox */
          if (game->b1D6C == 0 && f8007943C(game) == 0)
            f800DCE14(game, 0x3066, 0);
        }
        ++game->day_phase;
      }
      break;

    case 7: /* 80061C60 */
      f800C7A78(0x24, 0x01, 0x00);
      if (game->rain_effect_1 == 0 && game->rain_effect_2 == 0)
        f800C7A78(0x56, 0x01, 0x01);
      ++game->day_phase;
      break;

    case 8: /* 80061CB0 */
      if (z64_file.day_time >= 0x4AAC)
        game->day_phase = 0;
      break;
  }
}
