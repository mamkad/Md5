#include "Md5.hpp"

#include <algorithm>

using std::copy_n;
using std::fill_n;
using std::for_each_n;

namespace Lib
{
    namespace Algs
    {
        namespace Hash 
        {
            string Md5::hash(string const& str)
            {
                HashContext context;

                setup(context);
                convertStringToIntArray(str, context);
                addZerosToIntArray(context);
                addLengthToEndOfIntArray(context);
                calculateHash(context);
                
                string outHash;
                convertHashToString(context, outHash);

                return outHash;
            }

            void Md5::setup(HashContext& context)
            {
                context.IntArray.reserve(ShiftSize);

                copy_n(Buffs.begin(), BuffSize, context.HashBuff.begin());
            }

            void Md5::convertStringToIntArray(string const& str, HashContext& context)
            {
                for (char chr : str) {
                    context.IntArray.push_back(unsigned(chr));
                    context.LengthInBits += 8U;
                }

                context.IntArray.push_back(1U);
                context.LengthInBits += 8U;
            }

            void Md5::addZerosToIntArray(HashContext& context)
            {
                unsigned const remainder = 448U % 512U;

                while ((context.LengthInBits % remainder) != 0U) {
                    context.IntArray.push_back(0U);
                    context.LengthInBits += 8U;
                }

                 context.IntArray.push_back(0U);
                    context.LengthInBits += 8U;
            }

            void Md5::addLengthToEndOfIntArray(HashContext& context)
            {
                uint64_t lenInBits64 = context.LengthInBits;
                uint32_t low = lenInBits64 & 0xffffffff;
                uint32_t high = lenInBits64 >> 32U;

                context.IntArray.push_back(low);
                context.IntArray.push_back(high);
                context.LengthInBits += 64U;
            }

            void Md5::calculateHash(HashContext& context)
            {
                array<TCode, 5> hashBuff;
                size_t word = 0ULL;
                size_t countOfBlocks = context.IntArray.size() / BlockInBits;

                countOfBlocks = (context.IntArray.size() % BlockInBits) != 0ULL ? countOfBlocks + 1ULL : countOfBlocks;
            
                for (size_t block = 0ULL; block < countOfBlocks; block += Step) {
                    copy_n(Buffs.begin(), BuffSize, hashBuff.begin());
                   
                    for (size_t i = 0ULL; i < 64ULL; ++i) {
                        hashBuff[4] = 0U;
                        word = 0ULL;

                        if (i <= 15ULL) {
                            hashBuff[4] = (hashBuff[1] & hashBuff[2]) | ((~hashBuff[1]) & hashBuff[3]);
                            word = i; 
                        }
                        else if (i <= 31ULL) {
                            hashBuff[4] = (hashBuff[3] & hashBuff[1]) | ((~hashBuff[3]) & hashBuff[2]);
                            word = (5ULL * i + 1ULL) % 16ULL;
                        }
                        else if (i <= 47ULL) {
                            hashBuff[4] = hashBuff[1] ^ hashBuff[2] ^ hashBuff[3];
                            word = (3ULL * i + 5ULL) % 16ULL;
                        }
                        else if (i <= 63ULL) {
                            hashBuff[4] =  hashBuff[2] ^ (hashBuff[1] | (~hashBuff[3]));
                            word = (7ULL * i) % 16ULL;
                        }

                        hashBuff[4] = hashBuff[4] + hashBuff[0] + Consts[i] + context.IntArray[word];

                        hashBuff[0] = hashBuff[3];
                        hashBuff[3] = hashBuff[2];
                        hashBuff[2] = hashBuff[1];
                        hashBuff[1] = hashBuff[1] + rotateToLeft(hashBuff[4], Shifts[i]);
                    }

                    for (size_t i = 0; i < BuffSize; ++i) {
                        context.HashBuff[i] += hashBuff[i];
                    }
                }
            }

            Md5::TCode Md5::rotateToLeft(TCode number, TCode shift)
            {
                return (number << shift) | (number >> ((TCode)32 - shift));
            }

            void Md5::convertHashToString(HashContext& context, string& outHash)
            {
                short const hashSize = ((short)33);

                outHash.resize(hashSize);
                fill_n(outHash.begin(), hashSize, '\0');

                std::snprintf((char*)outHash.c_str(), hashSize, "%x%x%x%x", 
                    context.HashBuff[0], context.HashBuff[1], context.HashBuff[2], context.HashBuff[3]);
            }


            array<Md5::TCode, Md5::Const::BuffSize>  Md5::Buffs =
            {
                1732584193, 4023233417, 2562383102, 271733878
            };

            array<Md5::TCode, Md5::Const::ShiftSize> Md5::Consts = 
            {
                3614090360, 3905402710, 606105819, 3250441966, 4118548399, 1200080426, 2821735955, 4249261313,
                1770035416, 2336552879, 4294925233,2304563134, 1804603682, 4254626195, 2792965006, 1236535329,
                4129170786, 3225465664, 643717713, 3921069994, 3593408605, 38016083,   3634488961, 3889429448,
                568446438,  3275163606, 4107603335, 1163531501, 2850285829,4243563512, 1735328473, 2368359562,
                4294588738, 2272392833, 1839030562, 4259657740, 2763975236, 1272893353, 4139469664,3200236656,
                681279174,  3936430074, 3572445317, 76029189,   3654602809, 3873151461, 530742520, 3299628645,
                4096336452, 1126891415, 2878612391, 4237533241, 1700485571, 2399980690, 4293915773,2240044497,
                1873313359, 4264355552, 2734768916, 1309151649, 4149444226, 3174756917, 718787259, 3951481745
            };

            array<Md5::TCode, Md5::Const::ShiftSize> Md5::Shifts = 
            {
                7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
                5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
                4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
                6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
            };
        }
    }
}