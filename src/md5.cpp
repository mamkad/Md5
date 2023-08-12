#include "headers/md5.hpp"

#include <iostream>
using std::cout;

namespace lib
{
    namespace algs
    {
        namespace hash
        {
            void md5::context_setup(md5_context& md5_cntx)
            {
                md5_cntx.int_array.reserve(Const_shift_size);
                md5_cntx.length_in_bits = 0U;

                for (size_t i = 0; i < Buffs_size; ++i) {
                    md5_cntx.hash_buffer[i] = buffs[i];
                }
            }

            string md5::hash(string const& str)
            {
                md5_context md5_cntx;
                context_setup(md5_cntx);
                str_to_intarray(str, md5_cntx);
                add_zeros_to_intarray(md5_cntx);
                add_length_to_end_of_intarray(md5_cntx);
                md5_hash(md5_cntx);
                return hash_to_string(md5_cntx);
            }

            void md5::str_to_intarray(string const& str, md5_context& md5_cntx)
            {
                for (char chr : str) {
                    md5_cntx.int_array.push_back(unsigned(chr));
                    md5_cntx.length_in_bits += 8;
                }
                md5_cntx.int_array.push_back(1U);
                md5_cntx.length_in_bits += 8U;
            }

            void md5::add_zeros_to_intarray(md5_context& md5_cntx)
            {
                unsigned const remainder = 448U % 512U;
                while ((md5_cntx.length_in_bits % remainder) != 0U) {
                    md5_cntx.int_array.push_back(0U);
                    md5_cntx.length_in_bits += 8U;
                }
            }

            void md5::add_length_to_end_of_intarray(md5_context& md5_cntx)
            {
                uint64_t len_in_bits64 = md5_cntx.length_in_bits;
                uint32_t low = len_in_bits64 & 0xffffffff;
                uint32_t high = len_in_bits64 >> 32U;

                md5_cntx.int_array.push_back(low);
                md5_cntx.int_array.push_back(high);
                md5_cntx.length_in_bits += 64U;
            }

            void md5::md5_hash(md5_context& md5_cntx)
            {
                code_type A, B, C, D, F;
                size_t word;
                size_t count_of_blocks = md5_cntx.int_array.size() / Block_in_bits;

                count_of_blocks = (md5_cntx.int_array.size() % Block_in_bits) != 0ULL ? count_of_blocks + 1ULL : count_of_blocks;
            
                for (size_t block = 0ULL; block < count_of_blocks; block += Step ) {
                    A = buffs[0];
                    B = buffs[1];
                    C = buffs[2];
                    D = buffs[3];

                    for (size_t i = 0ULL; i < 64ULL; ++i) {
                        F = 0U;
                        word = 0ULL;

                        if (i <= 15ULL) {
                            F = (B & C) | ((~B) & D);
                            word = i; 
                        }
                        else if (i <= 31ULL) {
                            F = (D & B) | ((~D) & C);
                            word = (5ULL * i + 1ULL) % 16ULL;
                        }
                        else if (i <= 47ULL) {
                            F = B ^ C ^ D;
                            word = (3ULL * i + 5ULL) % 16ULL;
                        }
                        else if (i <= 63ULL) {
                            F = C ^ (B | (~D));
                            word = (7ULL * i) % 16ULL;
                        }

                        F = F + A + consts[i] + md5_cntx.int_array[word];
                        A = D;
                        D = C;
                        C = B;
                        B = B + (F << shifts[i]);
                    }

                    md5_cntx.hash_buffer[0] = (md5_cntx.hash_buffer[0] + A);
                    md5_cntx.hash_buffer[1] = (md5_cntx.hash_buffer[1] + B);
                    md5_cntx.hash_buffer[2] = (md5_cntx.hash_buffer[2] + C);
                    md5_cntx.hash_buffer[3] = (md5_cntx.hash_buffer[3] + D);
                }
            }

            string md5::hash_to_string(md5_context& md5_cntx)
            {
                short const hash_size = ((short)32);

                string hash;
                hash.resize(hash_size);

                std::snprintf((char*)hash.c_str(), hash_size, "%x%x%x%x", 
                    md5_cntx.hash_buffer[0], md5_cntx.hash_buffer[1], md5_cntx.hash_buffer[2], md5_cntx.hash_buffer[3]);

                return hash;
            }   
        }
    }
}