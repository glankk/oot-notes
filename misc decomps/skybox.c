
/* game + 0x1F78 */
typedef struct
{
  char      unk_00_[0x0128];  /* 0x0000 */
  char     *textures[2];      /* 0x0128 */
  char     *palettes;         /* 0x0130 */
  Gfx      *gfx;              /* 0x0134 */
  char      unk_01_[0x0004];  /* 0x0138 */
  char     *vtx;              /* 0x013C */
  int16_t   mode;             /* 0x0140 */
  char      unk_02_[0x0006];  /* 0x0142 */
  float     f148;             /* 0x0148 */
  char      unk_03_[0x0004];  /* 0x014C */
                              /* 0x0150 */
} z64_sky_ctxt_t;

typedef struct
{
  /* time-of-day values */
  uint16_t  start;            /* 0x0000 */
  uint16_t  end;              /* 0x0002 */
  uint8_t   b4;               /* 0x0004 */
  uint8_t   image_idx[2];     /* 0x0005 */
  char      pad_00_[0x0001];  /* 0x0007 */
                              /* 0x0008 */
} z64_sky_phase_t;

typedef struct
{
  /* vrom addresses */
  uint32_t  tex_start;        /* 0x0000 */
  uint32_t  tex_end;          /* 0x0004 */
  uint32_t  pal_start;        /* 0x0008 */
  uint32_t  pal_end;          /* 0x000C */
                              /* 0x0010 */
} z64_sky_image_t;

/* 800F172C */  z64_sky_phase_t sky_phases[2][9];
/* 800F184C */  z64_sky_image_t sky_images[9];

/* 80095CEC */
void f80095CEC(z64_game_t *game, z64_sky_ctxt_t *sky_ctxt, int skybox_type)
{
  uint32_t file_vrom;
  uint32_t file_size;
  uint8_t image_idx[2];
  z64_sky_image_t *image[2];

  switch (skybox_type) {
    case 0x0001: /* 80095D38 */
      /* normal sky */
          int localV1 = 0;
80095d4c: if (b.[0x8011B993] != 0 && sx(w.[0x8011B930]) < 4 && b.[0x800F1640] > 0 && b.[0x800F1640] < 6)
80095d7c:   localV1 = 1;
80095d90: z64_sky_phase_t *phase_list = sky_phases[localV1];
          for (int16_t i = 0; i < 9; ++i) {
80095da4:   z64_sky_phase_t *phase = &phase_list[i];
80095db8:   if (z64_file.h141A >= phase->start && (z64_file.h141A < phase->end || phase->end == 0xFFFF)) {
80095de4:     image_idx[0] = game->b10A34 = phase->image_idx[0];
80095df8:     image_idx[1] = game->b10A35 = phase->image_idx[1];
80095e0c:     if (phase->b4) {
                /* interpolate */
                float f = f8005C690(phase->end, phase->begin, z64_file.h141A) * 255.;
                /* what the hell is this bullshit */
                feholdexcept(&localT2);
                fesetround(FE_TOWARDZERO);
                $f8 = (int)f;
                if (fetestexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW)) {
                  $f8 = f - 2147483648.;
                  feclearexcept(FE_ALL_EXCEPT);
                  $f8 = (int)$f8
                  if (fetestexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW)) {
                    $t3 = -1;
                  }
                  else {
                    $t3 = $f8 | 0x80000000
                  }
                }
                else {
                  $t3 = $f8
80095eac:         if ($t3 < 0) {
80095ea0:           $t3 = -1;
                  }
                }
                fesetenv(&localT2);
80095ec4:       b.[game + 0x10A37] = $t3
              }
              else
                b.[game + 0x10A37] = 0
              break;
            }
          }

80095eec: image[0] = &sky_images[image_idx[0]];
80095f04: file_size = image[0]->tex_end - image[0]->tex_start;
80095f14: sky_ctxt->textures[0] = z64_HeapAllocHi(game + 0x0074, file_size);
80095f20: z64_LoadFile(sky_ctxt->textures[0], image[0]->tex_start, file_size);

80095f44: image[1] = &sky_images[image_idx[1]];
80095f54: file_size = image[1]->tex_end - image[1]->tex_start;
80095f5c: sky_ctxt->textures[1] = z64_HeapAllocHi(game + 0x0074, file_size);
80095f68: z64_LoadFile(sky_ctxt->textures[1], image[1]->tex_start, file_size);

80095f84: if ((image_idx[0] & 1) == (image_idx[0] & 4) >> 2) {
80095ff0:   file_size = image[0]->pal_end - image[0]->pal_start;
80096004:   sky_ctxt->palettes = z64_HeapAllocHi(game + 0x0074, file_size * 2);
80096010:   z64_LoadFile(sky_ctxt->palettes, image[1]->pal_start, file_size);
80096028:   z64_LoadFile(sky_ctxt->palettes + file_size, image[0]->pal_start, file_size);
          }
          else {
80095f9c:   file_size = image[0]->pal_end - image[0]->pal_start;
80095fb0:   sky_ctxt->palettes = z64_HeapAllocHi(game + 0x0074, file_size * 2);
80095fbc:   z64_LoadFile(sky_ctxt->palettes, image[0]->pal_start, file_size);
80095fd4:   z64_LoadFile(sky_ctxt->palettes + file_size, image[1]->pal_start, file_size);
          }
          break;

    case 0x0002: /* 80096038 */
      /* bazaar */
      sky_ctxt->mode = 1;
      file_size = 0x00020000;
      file_vrom = 0x01CA2000;
      sky_ctxt->textures[0] = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->textures[0], file_vrom, file_size);
      file_size = 0x00000400;
      file_vrom = 0x01CC2000;
      sky_ctxt->palettes = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->palettes, file_vrom, file_size);
      sky_ctxt->f148 = 0.8;  /* 80108C3C */
      break;

    case 0x0003: /* 800960D0 */
      /* overcast sunset */
      file_size = 0x0000C000;
      file_vrom = 0x01A8E000;
      sky_ctxt->textures[0] = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->textures[0], file_vrom, file_size);
      sky_ctxt->textures[1] = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->textures[1], file_vrom, file_size);
      file_size = 0x00000100;
      file_vrom = 0x01A9A000;
      sky_ctxt->palettes = z64_HeapAllocHi(game + 0x0074, file_size * 2);
      z64_LoadFile(sky_ctxt->palettes, file_vrom, file_size);
      z64_LoadFile(sky_ctxt->palettes + file_size, file_vrom, file_size);
      break;

    case 0x0004: /* 8009618C */
      /* market ruins */
      sky_ctxt->mode = 1;
      file_size = 0x00040000;
      file_vrom = 0x01B4C000;
      sky_ctxt->textures[0] = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->textures[0], file_vrom, file_size);
      file_size = 0x00000800;
      file_vrom = 0x01B8C000;
      sky_ctxt->palettes = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->palettes, file_vrom, file_size);
      break;

    case 0x0005: /* 8009621C */
      /* cutscene map */
      file_size = 0x00010000;
      file_vrom = 0x01AA8000;
      sky_ctxt->textures[0] = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->textures[0], file_vrom, file_size);
      file_size = 0x00010000;
      file_vrom = 0x01AB9000;
      sky_ctxt->textures[1] = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->textures[1], file_vrom, file_size);
      file_size = 0x00000100;
      file_vrom = 0x01AB8000;
      sky_ctxt->palettes = z64_HeapAllocHi(game + 0x0074, file_size * 2);
      z64_LoadFile(sky_ctxt->palettes, file_vrom, file_size);
      file_vrom = 0x01AC9000;
      z64_LoadFile(sky_ctxt->palettes + file_size, file_vrom, file_size);
      break;

    case 0x0007: /* 800962F8 */
      /* link's house */
      sky_ctxt->mode = 1;
      file_size = 0x00040000;
      file_vrom = 0x01B8D000;
      sky_ctxt->textures[0] = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->textures[0], file_vrom, file_size);
      file_size = 0x00000800;
      file_vrom = 0x01BCD000;
      sky_ctxt->palettes = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->palettes, file_vrom, file_size);
      break;

    case 0x0009: /* 80096388 */
      /* market day */
      sky_ctxt->mode = 1;
      file_size = 0x00040000;
      file_vrom = 0x01ACA000;
      sky_ctxt->textures[0] = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->textures[0], file_vrom, file_size);
      file_size = 0x00000800;
      file_vrom = 0x01B0A000;
      sky_ctxt->palettes = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->palettes, file_vrom, file_size);
      break;

    case 0x000A: /* 80096418 */
      /* market night */
      sky_ctxt->mode = 1;
      file_size = 0x00040000;
      file_vrom = 0x01B0B000;
      sky_ctxt->textures[0] = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->textures[0], file_vrom, file_size);
      file_size = 0x00000800;
      file_vrom = 0x01B4B000;
      sky_ctxt->palettes = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->palettes, file_vrom, file_size);
      break;

    case 0x000B: /* 800964A8 */
      /* happy mask shop */
      sky_ctxt->mode = 1;
      file_size = 0x00020000;
      file_vrom = 0x01EEF000;
      sky_ctxt->textures[0] = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->textures[0], file_vrom, file_size);
      file_size = 0x00000400;
      file_vrom = 0x01F0F000;
      sky_ctxt->palettes = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->palettes, file_vrom, file_size);
      sky_ctxt->f148 = 0.8;  /* 80108C40 */
      break;

    case 0x000C: /* 80096540 */
      /* know-it-all brothers' house */
      sky_ctxt->mode = 1;
      file_size = 0x00040000;
      file_vrom = 0x01BCE000;
      sky_ctxt->textures[0] = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->textures[0], file_vrom, file_size);
      file_size = 0x00000800;
      file_vrom = 0x01C0E000;
      sky_ctxt->palettes = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->palettes, file_vrom, file_size);
      break;

    case 0x000E: /* 800965D0 */
      /* house of twins */
      sky_ctxt->mode = 2;
      file_size = 0x00030000;
      file_vrom = 0x01C0F000;
      sky_ctxt->textures[0] = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->textures[0], file_vrom, file_size);
      file_size = 0x00000600;
      file_vrom = 0x01C3F000;
      sky_ctxt->palettes = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->palettes, file_vrom, file_size);
      break;

    case 0x000F: /* 8009665C */
      /* stable */
      sky_ctxt->mode = 1;
      file_size = 0x00040000;
      file_vrom = 0x01CC3000;
      sky_ctxt->textures[0] = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->textures[0], file_vrom, file_size);
      file_size = 0x00000800;
      file_vrom = 0x01D03000;
      sky_ctxt->palettes = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->palettes, file_vrom, file_size);
      break;

    case 0x0010: /* 800966E8 */
      /* carpenter's house */
      sky_ctxt->mode = 1;
      file_size = 0x00040000;
      file_vrom = 0x01D04000;
      sky_ctxt->textures[0] = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->textures[0], file_vrom, file_size);
      file_size = 0x00000800;
      file_vrom = 0x01D44000;
      sky_ctxt->palettes = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->palettes, file_vrom, file_size);
      break;

    case 0x0011: /* 80096774 */
      /* kokiri shop */
      sky_ctxt->mode = 1;
      file_size = 0x00020000;
      file_vrom = 0x01DB7000;
      sky_ctxt->textures[0] = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->textures[0], file_vrom, file_size);
      file_size = 0x00000400;
      file_vrom = 0x01DD7000;
      sky_ctxt->palettes = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->palettes, file_vrom, file_size);
      sky_ctxt->f148 = 0.8;  /* 80108C44 */
      break;

    case 0x0013: /* 80096808 */
      /* goron shop */
      sky_ctxt->mode = 1;
      file_size = 0x00020000;
      file_vrom = 0x01DD8000;
      sky_ctxt->textures[0] = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->textures[0], file_vrom, file_size);
      file_size = 0x00000400;
      file_vrom = 0x01DF8000;
      sky_ctxt->palettes = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->palettes, file_vrom, file_size);
      sky_ctxt->f148 = 0.8;  /* 80108C48 */
      break;

    case 0x0014: /* 8009689C */
      /* zora shop */
      sky_ctxt->mode = 1;
      file_size = 0x00020000;
      file_vrom = 0x01DF9000;
      sky_ctxt->textures[0] = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->textures[0], file_vrom, file_size);
      file_size = 0x00000400;
      file_vrom = 0x01E19000;
      sky_ctxt->palettes = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->palettes, file_vrom, file_size);
      sky_ctxt->f148 = 0.8;  /* 80108C4C */
      break;

    case 0x0016: /* 80096930 */
      /* kakariko potion shop */
      sky_ctxt->mode = 1;
      file_size = 0x00020000;
      file_vrom = 0x01E1A000;
      sky_ctxt->textures[0] = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->textures[0], file_vrom, file_size);
      file_size = 0x00000400;
      file_vrom = 0x01E3A000;
      sky_ctxt->palettes = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->palettes, file_vrom, file_size);
      sky_ctxt->f148 = 0.8;  /* 80108C50 */
      break;

    case 0x0017: /* 800969C4 */
      /* market potion shop */
      sky_ctxt->mode = 1;
      file_size = 0x00020000;
      file_vrom = 0x01E3B000;
      sky_ctxt->textures[0] = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->textures[0], file_vrom, file_size);
      file_size = 0x00000400;
      file_vrom = 0x01E5B000;
      sky_ctxt->palettes = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->palettes, file_vrom, file_size);
      sky_ctxt->f148 = 0.8;  /* 80108C54 */
      break;

    case 0x0018: /* 80096A58 */
      /* bombchu shop */
      sky_ctxt->mode = 1;
      file_size = 0x00020000;
      file_vrom = 0x01E5C000;
      sky_ctxt->textures[0] = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->textures[0], file_vrom, file_size);
      file_size = 0x00000400;
      file_vrom = 0x01E7C000;
      sky_ctxt->palettes = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->palettes, file_vrom, file_size);
      sky_ctxt->f148 = 0.8;  /* 80108C58 */
      break;

    case 0x001A: /* 80096AEC */
      /* richard's house */
      sky_ctxt->mode = 1;
      file_size = 0x00040000;
      file_vrom = 0x01D76000;
      sky_ctxt->textures[0] = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->textures[0], file_vrom, file_size);
      file_size = 0x00000800;
      file_vrom = 0x01DB6000;
      sky_ctxt->palettes = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->palettes, file_vrom, file_size);
      break;

    case 0x001B: /* 80096B78 */
      /* impa's house */
      sky_ctxt->mode = 1;
      file_size = 0x00040000;
      file_vrom = 0x01E7D000;
      sky_ctxt->textures[0] = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->textures[0], file_vrom, file_size);
      file_size = 0x00000800;
      file_vrom = 0x01EBD000;
      sky_ctxt->palettes = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->palettes, file_vrom, file_size);
      break;

    case 0x001C: /* 80096C04 */
      /* carpenter's tent */
      sky_ctxt->mode = 2;
      file_size = 0x00030000;
      file_vrom = 0x01EBE000;
      sky_ctxt->textures[0] = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->textures[0], file_vrom, file_size);
      file_size = 0x00000600;
      file_vrom = 0x01EEE000;
      sky_ctxt->palettes = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->palettes, file_vrom, file_size);
      break;

    case 0x0020: /* 80096C90 */
      /* mido's house */
      sky_ctxt->mode = 2;
      file_size = 0x00030000;
      file_vrom = 0x01C40000;
      sky_ctxt->textures[0] = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->textures[0], file_vrom, file_size);
      file_size = 0x00000600;
      file_vrom = 0x01C70000;
      sky_ctxt->palettes = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->palettes, file_vrom, file_size);
      break;

    case 0x0021: /* 80096D1C */
      /* saria's house */
      sky_ctxt->mode = 2;
      file_size = 0x00030000;
      file_vrom = 0x01C71000;
      sky_ctxt->textures[0] = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->textures[0], file_vrom, file_size);
      file_size = 0x00000600;
      file_vrom = 0x01CA1000;
      sky_ctxt->palettes = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->palettes, file_vrom, file_size);
      break;

    case 0x0022: /* 80096DA8 */
      /* guy's house */
      sky_ctxt->mode = 2;
      file_size = 0x00030000;
      file_vrom = 0x01D45000;
      sky_ctxt->textures[0] = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->textures[0], file_vrom, file_size);
      file_size = 0x00000600;
      file_vrom = 0x01D75000;
      sky_ctxt->palettes = z64_HeapAllocHi(game + 0x0074, file_size);
      z64_LoadFile(sky_ctxt->palettes, file_vrom, file_size);
      break;
  }
}
