#pragma once

#include <vector>
#include <string>
#include <array>
using std::string;
using std::array;
using std::vector;

namespace lib
{
    namespace algs
    {
        namespace hash 
        {
            using code_type = uint32_t;
            
            class md5 final 
            {
            private:
                md5(md5 const&) = delete;
                md5& operator=(md5 const&) = delete;

                short static const Buffs_size = ((short)4);
                short static const Const_shift_size = ((short)64);

                static array<code_type, Buffs_size>  buffs;
                static array<code_type, Const_shift_size> consts;
                static array<code_type, Const_shift_size> shifts;

                code_type static const Block_in_bits = 512U;
                code_type static const Step = Block_in_bits / 8U;

                struct md5_context
                {
                    vector<code_type> int_array;
                    size_t length_in_bits;
                    array<code_type, Buffs_size> hash_buffer;
                };

            public:
                static string hash(string const& str);

            private:
                static void context_setup(md5_context& md5_cntx);
                static void str_to_intarray(string const& str, md5_context& md5_cntx);
                static void add_zeros_to_intarray(md5_context& md5_cntx);
                static void add_length_to_end_of_intarray(md5_context& md5_cntx);
                static void md5_hash(md5_context& md5_cntx);
                static string hash_to_string(md5_context& md5_cntx);
            };
        }
    }
}