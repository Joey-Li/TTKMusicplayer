#include "musiccryptographichash.h"

#define XXTEA_MX (z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + (k[p & 3 ^ e] ^ z)
#define XXTEA_DELTA 0x9E3779B9

const MString base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

MusicCryptographicHash::MusicCryptographicHash()
{

}

QString MusicCryptographicHash::encrypt(const QString &data, const QString &key)
{
    return xxteaEncrypt(data, key).toUtf8().toBase64();
}

QString MusicCryptographicHash::decrypt(const QString &data, const QString &key)
{
    return xxteaDecrypt(QByteArray::fromBase64(data.toUtf8()), key);
}

MString MusicCryptographicHash::xxteaEncrypt(const MString &data, const MString &key)
{
    const MString &raw = QString(QString(data.c_str()).toUtf8()).toStdString();

    uchar dataCopy[1024];
    strcpy((char*)dataCopy, (const char *)raw.c_str());

    uchar keyCopy[1024];
    strcpy((char*)keyCopy, (const char *)key.c_str());

    xxtea_uint s[1];
    uchar *encrypt = xxteaEncrypt(dataCopy, strlen((const char *)dataCopy), keyCopy, strlen((const char *)keyCopy), s);
    const MString &encode = base64Encode(encrypt, s[0]);
    free(encrypt);

    return encode;
}

MString MusicCryptographicHash::xxteaDecrypt(const MString &data, const MString &key)
{
    const MString &decode = base64Decode(data);
    if(decode.empty())
    {
        return MString("");
    }

    uchar dataCopy[1024];
    memcpy(dataCopy, decode.c_str(), decode.length());

    dataCopy[decode.length()] = '\0';
    uchar keyCopy[1024];
    strcpy((char*)keyCopy, (const char *)key.c_str());

    xxtea_uint s[1];
    uchar *encrypt = xxteaDecrypt(dataCopy, decode.length(), keyCopy, strlen((const char *)keyCopy), s);
    if(!encrypt)
    {
        return MString("false_false");
    }

    MString raw = (char*)encrypt;
    raw = QString::fromUtf8(raw.c_str()).toStdString();
    free(encrypt);

    return raw;
}

QString MusicCryptographicHash::xxteaEncrypt(const QString &data, const QString &key)
{
    return xxteaEncrypt(data.toStdString(), key.toStdString()).c_str();
}

QString MusicCryptographicHash::xxteaDecrypt(const QString &data, const QString &key)
{
    return xxteaDecrypt(data.toStdString(), key.toStdString()).c_str();
}

bool MusicCryptographicHash::isBase64(uchar c)
{
    return (isalnum(c) || (c == '+') || (c == '/'));
}

MString MusicCryptographicHash::base64Encode(uchar const* bytes, uint in_len)
{
    MString ret;
    int i = 0, j = 0;
    uchar char_array_3[3], char_array_4[4];

    while(in_len--)
    {
        char_array_3[i++] = *(bytes++);
        if(i == 3)
        {
            char_array_4[0] = (char_array_3[0] & 0xFC) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xF0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0F) << 2) + ((char_array_3[2] & 0xC0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3F;

            for(i = 0; (i <4) ; i++)
            {
                ret += base64_chars[char_array_4[i]];
            }
            i = 0;
        }
    }

    if(i)
    {
        for(j = i; j < 3; j++)
        {
            char_array_3[j] = '\0';
        }

        char_array_4[0] = (char_array_3[0] & 0xFC) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xF0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0F) << 2) + ((char_array_3[2] & 0xC0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3F;

        for(j = 0; (j < i + 1); j++)
        {
            ret += base64_chars[char_array_4[j]];
        }

        while((i++ < 3))
        {
            ret += '=';
        }

    }
    return ret;
}

MString MusicCryptographicHash::base64Decode(const MString &bytes)
{
    int in_len = bytes.size();
    int i = 0, j = 0, in_ = 0;
    uchar char_array_4[4], char_array_3[3];
    MString ret;

    while(in_len-- && ( bytes[in_] != '=') && isBase64(bytes[in_]))
    {
        char_array_4[i++] = bytes[in_]; in_++;
        if(i ==4)
        {
            for(i = 0; i <4; i++)
            {
                char_array_4[i] = base64_chars.find(char_array_4[i]);
            }

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xF) << 4) + ((char_array_4[2] & 0x3C) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for(i = 0; (i < 3); i++)
            {
                ret += char_array_3[i];
            }
            i = 0;
        }
    }

    if(i)
    {
        for(j = i; j <4; j++)
        {
            char_array_4[j] = 0;
        }

        for(j = 0; j <4; j++)
        {
            char_array_4[j] = base64_chars.find(char_array_4[j]);
        }

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xF) << 4) + ((char_array_4[2] & 0x3C) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for(j = 0; (j < i - 1); j++)
        {
            ret += char_array_3[j];
        }
    }

    return ret;
}

void MusicCryptographicHash::xxteaUintEncrypt(xxtea_uint *v, xxtea_uint len, xxtea_uint *k)
{
    xxtea_uint n = len - 1;
    xxtea_uint z = v[n], y = v[0], p, q = 6 + 52 / (n + 1), sum = 0, e;

    if(n < 1)
    {
        return;
    }

    while(0 < q--)
    {
        sum += XXTEA_DELTA;
        e = sum >> 2 & 3;
        for(p = 0; p < n; p++)
        {
            y = v[p + 1];
            z = v[p] += XXTEA_MX;
        }
        y = v[0];
        z = v[n] += XXTEA_MX;
    }
}

void MusicCryptographicHash::xxteaUintDecrypt(xxtea_uint *v, xxtea_uint len, xxtea_uint *k)
{
    xxtea_uint n = len - 1;
    xxtea_uint z = v[n], y = v[0], p, q = 6 + 52 / (n + 1), sum = q * XXTEA_DELTA, e;

    if(n < 1)
    {
        return;
    }

    while(sum != 0)
    {
        e = sum >> 2 & 3;
        for(p = n; p > 0; p--)
        {
            z = v[p - 1];
            y = v[p] -= XXTEA_MX;
        }
        z = v[n];
        y = v[0] -= XXTEA_MX;
        sum -= XXTEA_DELTA;
    }
}

uchar *MusicCryptographicHash::fixKeyLength(uchar *key, xxtea_uint keyLength)
{
    uchar *tmp = (uchar *)malloc(16);
    memcpy(tmp, key, keyLength);
    memset(tmp + keyLength, '\0', 16 - keyLength);
    return tmp;
}

xxtea_uint *MusicCryptographicHash::xxteaToUintArray(uchar *data, xxtea_uint len, int includeLength, xxtea_uint *retLength)
{
    xxtea_uint i, n, *result;

    n = len >> 2;
    n = (((len & 3) == 0) ? n : n + 1);

    if(includeLength)
    {
        result = (xxtea_uint *)malloc((n + 1) << 2);
        result[n] = len;
        *retLength = n + 1;
    } else
    {
        result = (xxtea_uint *)malloc(n << 2);
        *retLength = n;
    }

    memset(result, 0, n << 2);
    for(i = 0; i < len; i++)
    {
        result[i >> 2] |= (xxtea_uint)data[i] << ((i & 3) << 3);
    }

    return result;
}

uchar *MusicCryptographicHash::xxteaToByteArray(xxtea_uint *data, xxtea_uint len, int includeLength, xxtea_uint *retLength)
{
    xxtea_uint i, n, m;
    uchar *result;

    n = len << 2;
    if(includeLength)
    {
        m = data[len - 1];
        if((m < n - 7) || (m > n - 4))
        {
            return nullptr;
        }
        n = m;
    }

    result = (uchar *)malloc(n + 1);
    for(i = 0; i < n; i++)
    {
        result[i] = (uchar)((data[i >> 2] >> ((i & 3) << 3)) & 0xFF);
    }

    result[n] = '\0';
    *retLength = n;

    return result;
}

uchar *MusicCryptographicHash::doXxteaEncrypt(uchar *data, xxtea_uint len, uchar *key, xxtea_uint *retLength)
{
    uchar *result;
    xxtea_uint *v, *k, vlen, klen;

    v = xxteaToUintArray(data, len, 1, &vlen);
    k = xxteaToUintArray(key, 16, 0, &klen);

    xxteaUintEncrypt(v, vlen, k);
    result = xxteaToByteArray(v, vlen, 0, retLength);

    free(v);
    free(k);

    return result;
}

uchar *MusicCryptographicHash::doXxteaDecrypt(uchar *data, xxtea_uint len, uchar *key, xxtea_uint *retLength)
{
    uchar *result;
    xxtea_uint *v, *k, vlen, klen;

    v = xxteaToUintArray(data, len, 0, &vlen);
    k = xxteaToUintArray(key, 16, 0, &klen);

    xxteaUintDecrypt(v, vlen, k);
    result = xxteaToByteArray(v, vlen, 1, retLength);

    free(v);
    free(k);

    return result;
}

uchar *MusicCryptographicHash::xxteaEncrypt(uchar *data, xxtea_uint dataLength, uchar *key, xxtea_uint keyLength, xxtea_uint *retLength)
{
    uchar *result;
    *retLength = 0;

    if(keyLength < 16)
    {
        uchar *key2 = fixKeyLength(key, keyLength);
        result = doXxteaEncrypt(data, dataLength, key2, retLength);
        free(key2);
    }
    else
    {
        result = doXxteaEncrypt(data, dataLength, key, retLength);
    }

    return result;
}

uchar *MusicCryptographicHash::xxteaDecrypt(uchar *data, xxtea_uint dataLength, uchar *key, xxtea_uint keyLength, xxtea_uint *retLength)
{
    uchar *result;
    *retLength = 0;

    if(keyLength < 16)
    {
        uchar *key2 = fixKeyLength(key, keyLength);
        result = doXxteaDecrypt(data, dataLength, key2, retLength);
        free(key2);
    }
    else
    {
        result = doXxteaDecrypt(data, dataLength, key, retLength);
    }

    return result;
}
