#include <string.h>
#include <immintrin.h>

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

void sm3_hash(const void *message, size_t message_len, uint32_t digest[8])
{
    uint32_t IV[8] = {
        0x7380166f, 0x4914b2b9, 0x172442d7, 0xda8a0600,
        0xa96f30bc, 0x163138aa, 0xe38dee4d, 0xb0fb0e4e
    };

    const uint8_t *msg = message;
    size_t blocks = (message_len + 63) / 64;

    __m256i T = _mm256_set_epi32(0x79CC4519, 0x7A879D8A, 0x00000000, 0x00000000);

    for (size_t i = 0; i < blocks; i++) {
        uint32_t W[68] = {0};


        for (size_t j = 0; j < 16 && i * 64 + j * 4 < message_len; j++) {
            W[j] = msg[i * 64 + j * 4] << 24 |
                   msg[i * 64 + j * 4 + 1] << 16 |
                   msg[i * 64 + j * 4 + 2] << 8 |
                   msg[i * 64 + j * 4 + 3];
        }


        for (size_t j = 16; j < 68; j++) {
            W[j] = ROTATE_LEFT(W[j - 16] ^ W[j - 9] ^ ROTATE_LEFT(W[j - 3], 15), 7) ^
                   ROTATE_LEFT(W[j - 13] ^ W[j - 6] ^ ROTATE_LEFT(W[j - 2], 15), 11) ^
                   ROTATE_LEFT(W[j - 10] ^ W[j - 8] ^ ROTATE_LEFT(W[j - 1], 15), 13) ^
                   ROTATE_LEFT(W[j - 16], 18);
        }


        __m256i A = _mm256_set_epi32(IV[7], IV[6], IV[5], IV[4]);
        __m256i B = _mm256_set_epi32(IV[3], IV[2], IV[1], IV[0]);
        __m256i C = _mm256_set_epi32(IV[7], IV[6], IV[5], IV[4]);
        __m256i D = _mm256_set_epi32(IV[3], IV[2], IV[1], IV[0]);
        __m256i E = _mm256_set_epi32(IV[7], IV[6], IV[5], IV[4]);
        __m256i F = _mm256_set_epi32(IV[3], IV[2], IV[1], IV[0]);
        __m256i G = _mm256_set_epi32(IV[7], IV[6], IV[5], IV[4]);
        __m256i H = _mm256_set_epi32(IV[3], IV[2], IV[1], IV[0]);


        for (size_t j = 0; j < 64; j++) {
            __m256i SS1 = _mm256_or_si256(_mm256_slli_epi32(ROTATE_LEFT(A, 12), 7),
                                          _mm256_srli_epi32(A, 32 - 7));
            __m256i SS2 = _mm256_or_si256(_mm256_slli_epi32(ROTATE_LEFT(E, 12), 7),
                                          _mm256_srli_epi32(E, 32 - 7));
            __m256i TT1 = _mm256_add_epi32(_mm256_add_epi32(_mm256_add_epi32(H, SS1),
                                                            _mm256_add_epi32(T, _mm256_set1_epi32(j))),
                                           _mm256_set1_epi32(W[j]));
            __m256i TT2 = _mm256_add_epi32(_mm256_add_epi32(SS2, _mm256_or_si256(_mm256_and_si256(A, B),
                                                                                _mm256_and_si256(_mm256_and_si256(A, C),
                                                                                                 _mm256_and_si256(B, C)))),
                                           _mm256_set1_epi32(W[j + 4]));

            H = G;
            G = F;
            F = E;
            E = _mm256_add_epi32(D, TT1);
            D = C;
            C = B;
            B = A;
            A = _mm256_add_epi32(TT1, TT2);
        }


        IV[0] ^= A.m256i_u32[0];
        IV[1] ^= A.m256i_u32[1];
        IV[2] ^= A.m256i_u32[2];
        IV[3] ^= A.m256i_u32[3];
        IV[4] ^= B.m256i_u32[0];
        IV[5] ^= B.m256i_u32[1];
        IV[6] ^= B.m256i_u32[2];
        IV[7] ^= B.m256i_u32[3];
    }


    digest[0] = IV[0];
    digest[1] = IV[1];
    digest[2] = IV[2];
    digest[3] = IV[3];
    digest[4] = IV[4];
    digest[5] = IV[5];
    digest[6] = IV[6];
    digest[7] = IV[7];
}