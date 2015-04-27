/*
        This should be following elypter's "The .mmp file standard" closely:
                http://gtamp.com/forum/viewtopic.php?f=4&t=188

        MMP-file:
                [MapFiles]
                GMPFile=test.gmp
                Description=test

        Read:
                mmp_read(mmp, "MapFiles/Description");

*/

typedef struct mmp_key_t {
  struct mmp_key_t *next;
  char *key;
  char *value;
} mmp_key_t;

typedef struct mmp_t {
  struct mmp_t *next;
  mmp_key_t *data;
  char *source;
} mmp_t;

mmp_t *mmp_init(const char *path);
mmp_t *mmp_load(char *filename);
char *mmp_read(mmp_t *mmp, char *key);
void mmp_cleanup(mmp_t *mmp);
