typedef struct 
{
    UINT32 h0,h1,h2,h3,h4;
    UINT32  nblocks;
    unsigned char buf[64];
    int  count;
} SHA1_CONTEXT;


void sha1_init( SHA1_CONTEXT *hd );

void sha1_write( SHA1_CONTEXT *hd, unsigned char *inbuf, size_t inlen);

void sha1_final(SHA1_CONTEXT *hd);

