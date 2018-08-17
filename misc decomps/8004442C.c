
typedef struct
{
  int8_t        marker;           /* 0x0000 */  /* -1 = terminate */
  int8_t        roll;             /* 0x0001 */
  uint16_t      n_frames;         /* 0x0002 */
  float         angle;            /* 0x0004 */  /* fov? */
  z64_xyz_t     xyz;              /* 0x0008 */
  char          unk_00_[0x0002];  /* 0x000E */
                                  /* 0x0010 */
} z64_cs_cam_t;

typedef struct
{
  z64_xyzf_t    xyz;              /* 0x0000 */
  float         roll;             /* 0x000C */
  float         angle;            /* 0x0010 */
                                  /* 0x0014 */
} z64_cs_camf_t;

typedef struct
{
  int16_t       h0;       /* 0x0000 */
  float         time;     /* 0x0004 */
  int16_t       idx;      /* 0x0008 */

  z64_xyzf_t    xyzf50;   /* 0x0050 */
  z64_xyzf_t    xyzf5C;   /* 0x005C */

  z64_xyzf_t    xyzf74;   /* 0x0074 */

  void         *p90;      /* 0x0090 */

  float         fDC;      /* 0x00DC */

  float         fFC;      /* 0x00FC */

  z64_cs_cam_t *p124;     /* 0x0124 */
  z64_cs_cam_t *p128;     /* 0x0128 */
  int16_t       h12C;     /* 0x012C */

  int16_t       h142;     /* 0x0142 */
  int16_t       h144;     /* 0x0144 */

  int16_t       h15A;     /* 0x015A */

  int16_t       h15E;     /* 0x015E */
} z64_camera2_t;

/* 8004442C */
int f8004442C(z64_camera2_t *arg0)
{
80044470: arg0->h0 = h.[w.[w.[0x800EEE74 + arg0->h142 * 8] + arg0->h144 * 8 + 4]];

8004447c: w.[0x800EF1A8] = arg0->h0

80044488: if (arg0->h15E == 0) {
800444a8:   arg0->idx = 0;
800444ac:   arg0->time = 0.;
800444b8:   ++arg0->h15E;
          }
80044490: else if (arg0->h15E != 1)
            return 1;

          float local4C;
          xyzf_t local50;
          xyzf_t local5C;

800444f0: if (   f800999A4(&local5C, &local4C, &arg0->fFC, arg0->p128, &arg0->idx, &arg0->time)
              || f800999A4(&local50, &local4C, &arg0->fFC, arg0->p124, &arg0->idx, &arg0->time))
          {
80044520:   ++arg0->h15E;
          }
80044530: if (arg0->h12C == 0) {
80044584:   z64_Vec3Copy(&local5C, &arg0->xyzf74);
80044598:   z64_Vec3Copy(&local50, &arg0->xyzf50);
          }
          else if (arg0->p90 != 0 && sx(w.[arg0->p90 + 0x0130]) != 0) {
80044550:   f800226A4(&local68, arg0->p90);
80044560:   f800443D0(&local68, &local5C, &arg0->xyzf74);
80044570:   f800443D0(&local68, &local50, &arg0->xyzf50);
          }
          arg0->xyzf5C = arg0->xyzf74;
800445e4: arg0->h15A = local4C * 256.;
800445e8: arg0->fDC = z64_Vec3Distance(&arg0->xyzf50, &arg0->xyzf5C);

          return 1;
}

/* 800999A4 */
int f800999A4(xyzf_t *xyz, float *roll, float *angle, z64_cs_cam_t *list, int16_t *p_idx, float *p_time)
{
          float time;
800999d8: if (*p_idx >= 0)
800999dc:   time = *p_time;
          else
800999e0:   time = 0.;

800999f0: z64_cs_cam_t *cam = &list[*p_idx];
800999f8: if (cam[0].marker == -1 || cam[1].marker == -1 || cam[2].marker == -1)
            return 1;

          z64_cs_camf_t *camf[4];
          for (int i = 0; i < 4; ++i) {
            camf[i].xyz.x = cam[i].xyz.x;
            camf[i].xyz.y = cam[i].xyz.y;
            camf[i].xyz.z = cam[i].xyz.z;
            camf[i].roll = cam[i].roll;
            camf[i].angle = cam[i].angle;
          }
          
          int result = 0;
80099b30: f80099790(time, xyz, roll, angle, &camf[0], &camf[1], &camf[2], &camf[3]);

          float localF14;
80099b64: if (cam[1].n_frames == 0)
80099b60:   localF14 = 0.;
          else
80099b90:   localF14 = 1. / cam[1].n_frames;

          float localF4;
80099b9c: if (cam[2].n_frames == 0)
80099ba0:   localF4 = 0. - localF14;
          else
80099bc8:   localF4 = 1. / cam[2].n_frames - localF14;

          float localF8;
80099bd4: float localF0 = *p_time * localF4 + localF14;
80099be0: if (localF0 < 0.)
80099bf0:   localF8 = *p_time;
          else
80099be4:   localF8 = *p_time + localF0;
80099bfc: *p_time = localF8;

80099c0c: if (*p_time >= 1.) {
80099c1c:   ++*p_idx;
80099c30:   if (cam[4].marker == -1) {
80099c38:     *p_idx = 0;
              result = 1;
            }
80099c48:   --*p_time;
          }
          
          return result;
}

/* 80099790 */
void f80099790(float x, z64_xyzf_t *xyz, float *roll, float *angle,
               z64_cs_camf_t *p0, z64_cs_camf_t *p1, z64_cs_camf_t *p2, z64_cs_camf_t *p3)
{
  if (x > 1.)
    x = 1.;
  float i = 1. - x;

  /* (1-x)^3/6 */
  float f0 = i*i*i/6.;
  /* x^3/2 - x^2 + 4/6 */
  float f1 = x*x*x/2. - x*x + 0.6666667; /* 80108C90 */
  /* -x^3/2 + x^2/2 + 1/6 */
  float f2 = -x*x*x/2. + x*x/2. + x/2. + 0.16666667; /* 80108C94 */
  /* x^3/6 */
  float f3 = x*x*x/6.;

  xyz->x = f0 * p0->xyz.x + f1 * p1->xyz.x + f2 * p2->xyz.x + f3 * p3->xyz.x;
  xyz->y = f0 * p0->xyz.y + f1 * p1->xyz.y + f2 * p2->xyz.y + f3 * p3->xyz.y;
  xyz->z = f0 * p0->xyz.z + f1 * p1->xyz.z + f2 * p2->xyz.z + f3 * p3->xyz.z;
  *roll  = f0 * p0->roll  + f1 * p1->roll  + f2 * p2->roll  + f3 * p3->roll;
  *angle = f0 * p0->angle + f1 * p1->angle + f2 * p2->angle + f3 * p3->angle;
}

/* 80067580 */
/* returns |a-b| */
float z64_Vec3Distance(z64_xyzf_t *a, z64_xyzf_t *b)
{
  float dx = a->x - b->x;
  float dy = a->y - b->y;
  float dz = a->z - b->z;
  return sqrtf(dx * dx + dy * dy + dz * dz);
}

/* 800443B0 */
void z64_Vec3Copy(z64_xyzf_t *src, z64_xyzf_t *dst)
{
  *dst = *src;
}
