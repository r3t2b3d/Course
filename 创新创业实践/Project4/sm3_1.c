#include <stdio.h>
#include <string.h>
#include<stdint.h>

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

#define P0(x) ((x) ^ ROTATE_LEFT((x), 9) ^ ROTATE_LEFT((x), 17))
#define P1(x) ((x) ^ ROTATE_LEFT((x), 15) ^ ROTATE_LEFT((x), 23))

#define FF0(x, y, z) ((x) ^ (y) ^ (z))
#define FF1(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))

#define GG0(x, y, z) ((x) ^ (y) ^ (z))
#define GG1(x, y, z) (((x) & (y)) | ((~(x)) & (z)))

#define GET_UINT32_BE(n, b, i)                 \
    {                                          \
        (n) = ((uint32_t)(b)[(i) + 3] << 0)     \
            | ((uint32_t)(b)[(i) + 2] << 8)     \
            | ((uint32_t)(b)[(i) + 1] << 16)    \
            | ((uint32_t)(b)[(i) + 0] << 24);   \
    }

#define PUT_UINT32_BE(n, b, i)                 \
    {                                          \
        (b)[(i) + 3] = (uint8_t)((n) >> 0);     \
        (b)[(i) + 2] = (uint8_t)((n) >> 8);     \
        (b)[(i) + 1] = (uint8_t)((n) >> 16);    \
        (b)[(i) + 0] = (uint8_t)((n) >> 24);    \
    }

typedef struct {
    uint32_t total[2];
    uint32_t state[8];
    uint8_t buffer[64];
} sm3_context;

void sm3_init(sm3_context *ctx);
void sm3_update(sm3_context *ctx, const uint8_t *input, size_t ilen);
void sm3_final(sm3_context *ctx, uint8_t output[32]);

static void sm3_process(sm3_context *ctx, const uint8_t data[64]);

const uint8_t padding[64] = {0x80, 0};

void sm3_init(sm3_context *ctx) {
    ctx->total[0] = 0;
    ctx->total[1] = 0;

    ctx->state[0] = 0x7380166f;
    ctx->state[1] = 0x4914b2b9;
    ctx->state[2] = 0x172442d7;
    ctx->state[3] = 0xda8a0600;
    ctx->state[4] = 0xa96f30bc;
    ctx->state[5] = 0x163138aa;
    ctx->state[6] = 0xe38dee4d;
    ctx->state[7] = 0xb0fb0e4e;
}

void sm3_update(sm3_context *ctx, const uint8_t *input, size_t ilen) {
    size_t fill;
    uint32_t left;

    if (ilen == 0) return;

    left = ctx->total[0] & 0x3F;
    fill = 64 - left;

    ctx->total[0] += ilen;
    ctx->total[0] &= 0xFFFFFFFF;

    if (ctx->total[0] < ilen) ctx->total[1]++;

    if (left && ilen >= fill) {
        memcpy((void *)(ctx->buffer + left), (void *)input, fill);
        sm3_process(ctx, ctx->buffer);
        input += fill;
        ilen -= fill;
        left = 0;
    }

    while (ilen >= 64) {
        sm3_process(ctx, input);
        input += 64;
        ilen -= 64;
    }

    if (ilen > 0) {
        memcpy((void *)(ctx->buffer + left), (void *)input, ilen);
    }
}

void sm3_final(sm3_context *ctx, uint8_t output[32]) {
    uint32_t last, padn;
    uint32_t high, low;
    uint8_t msglen[8];

    high = (ctx->total[0] >> 29) | (ctx->total[1] << 3);
    low = (ctx->total[0] << 3);

    PUT_UINT32_BE(high, msglen, 0);
    PUT_UINT32_BE(low, msglen, 4);

    last = ctx->total[0] & 0x3F;
    padn = (last < 56) ? (56 - last) : (120 - last);

    sm3_update(ctx, (uint8_t *)padding, padn);
    sm3_update(ctx, msglen, 8);

    PUT_UINT32_BE(ctx->state[0], output, 0);
    PUT_UINT32_BE(ctx->state[1], output, 4);
    PUT_UINT32_BE(ctx->state[2], output, 8);
    PUT_UINT32_BE(ctx->state[3], output, 12);
    PUT_UINT32_BE(ctx->state[4], output, 16);
    PUT_UINT32_BE(ctx->state[5], output, 20);
    PUT_UINT32_BE(ctx->state[6], output, 24);
    PUT_UINT32_BE(ctx->state[7], output, 28);
}

static void sm3_process(sm3_context *ctx, const uint8_t data[64]) {
    uint32_t SS1, SS2, TT1, TT2, W[68], W1[64];
    uint32_t A, B, C, D, E, F, G, H;
    uint32_t T[64];
    uint32_t Temp1, Temp2, Temp3, Temp4, Temp5;
    int j;

    for (j = 0; j < 16; j++) {
        T[j] = 0x79CC4519;
    }
    for (j = 16; j < 64; j++) {
        T[j] = 0x7A879D8A;
    }

    GET_UINT32_BE(W[0], data, 0);
    GET_UINT32_BE(W[1], data, 4);
    GET_UINT32_BE(W[2], data, 8);
    GET_UINT32_BE(W[3], data, 12);
    GET_UINT32_BE(W[4], data, 16);
    GET_UINT32_BE(W[5], data, 20);
    GET_UINT32_BE(W[6], data, 24);
    GET_UINT32_BE(W[7], data, 28);
    GET_UINT32_BE(W[8], data, 32);
    GET_UINT32_BE(W[9], data, 36);
    GET_UINT32_BE(W[10], data, 40);
    GET_UINT32_BE(W[11], data, 44);
    GET_UINT32_BE(W[12], data, 48);
    GET_UINT32_BE(W[13], data, 52);
    GET_UINT32_BE(W[14], data, 56);
    GET_UINT32_BE(W[15], data, 60);

    for (j = 16; j < 68; j++) {
        W[j] = P1(W[j - 16] ^ W[j - 9] ^ ROTATE_LEFT(W[j - 3], 15)) ^ ROTATE_LEFT(W[j - 13], 7) ^ W[j - 6];
    }

    for (j = 0; j < 64; j++) {
        W1[j] = W[j] ^ W[j + 4];
    }

    A = ctx->state[0];
    B = ctx->state[1];
    C = ctx->state[2];
    D = ctx->state[3];
    E = ctx->state[4];
    F = ctx->state[5];
    G = ctx->state[6];
    H = ctx->state[7];

    for (j = 0; j < 16; j++) {
        SS1 = ROTATE_LEFT((ROTATE_LEFT(A, 12) + E + ROTATE_LEFT(T[j], j)), 7);
        SS2 = SS1 ^ ROTATE_LEFT(A, 12);
        TT1 = FF0(A, B, C) + D + SS2 + W1[j];
        TT2 = GG0(E, F, G) + H + SS1 + W[j];
        D = C;
        C = ROTATE_LEFT(B, 9);
        B = A;
        A = TT1;
        H = G;
        G = ROTATE_LEFT(F, 19);
        F = E;
        E = P0(TT2);
    }

    for (j = 16; j < 64; j++) {
        SS1 = ROTATE_LEFT((ROTATE_LEFT(A, 12) + E + ROTATE_LEFT(T[j], j % 32)), 7);
        SS2 = SS1 ^ ROTATE_LEFT(A, 12);
        TT1 = FF1(A, B, C) + D + SS2 + W1[j];
        TT2 = GG1(E, F, G) + H + SS1 + W[j];
        D = C;
        C = ROTATE_LEFT(B, 9);
        B = A;
        A = TT1;
        H = G;
        G = ROTATE_LEFT(F, 19);
        F = E;
        E = P0(TT2);
    }

    ctx->state[0] ^= A;
    ctx->state[1] ^= B;
    ctx->state[2] ^= C;
    ctx->state[3] ^= D;
    ctx->state[4] ^= E;
    ctx->state[5] ^= F;
    ctx->state[6] ^= G;
    ctx->state[7] ^= H;
}

#define GET_UINT32_BE(n, b, i)                 \
    {                                          \
        (n) = ((uint32_t)(b)[(i) + 3] << 0)     \
            | ((uint32_t)(b)[(i) + 2] << 8)     \
            | ((uint32_t)(b)[(i) + 1] << 16)    \
            | ((uint32_t)(b)[(i) + 0] << 24);   \
    }

#define PUT_UINT32_BE(n, b, i)                 \
    {                                          \
        (b)[(i) + 3] = (uint8_t)((n) >> 0);     \
        (b)[(i) + 2] = (uint8_t)((n) >> 8);     \
        (b)[(i) + 1] = (uint8_t)((n) >> 16);    \
        (b)[(i) + 0] = (uint8_t)((n) >> 24);    \
    }

int main() {
    uint8_t input[] = "hash_hash";
    uint8_t output[32];
    sm3_context ctx;

    sm3_init(&ctx);
    sm3_update(&ctx, input, strlen((char *)input));
    sm3_final(&ctx, output);

    printf("Hashed : ");
    for (int i = 0; i < 32; i++) {
        printf("%02x", output[i]);
    }
    printf("\n");

    return 0;
}